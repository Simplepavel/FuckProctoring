#include "Client.hpp"

FuckProctoringClient::FuckProctoringClient(boost::asio::io_context &_ioc) : ioc(_ioc), protocol(boost::asio::ip::tcp::v4()), client_socket(ioc)
{
}

// callback

void FuckProctoringClient::connect_callback(const boost::system::error_code &ec)
{
    if (ec.value() != 0)
    {
        std::cout << ec.message();
        return;
    }
    else
    {
        read();
    }
};

void FuckProctoringClient::read_capacity_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
{
    if (ec.value() != 0)
    {
        if (ec.value() == boost::asio::error::eof)
            client_socket.close();
        std::cout << ec.message() << '\n';
        return;
    }
    session_ptr->have_read += bytes;
    if (session_ptr->have_read == session_ptr->data_length)
    {
        uint32_t net_capacity;
        uint32_t capacity;
        memcpy(&net_capacity, session_ptr->buffer.get(), 4);
        capacity = ntohl(net_capacity);
        std::shared_ptr<ReadSession> data_session(new ReadSession(capacity));
        auto lambda = [this, data_session](const boost::system::error_code &ec, size_t bytes)
        {
            this->read_data_callback(bytes, ec, data_session);
        };
        client_socket.async_read_some(data_session->mutable_buffer_sequence, lambda);

        return;
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_capacity_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    client_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}

void FuckProctoringClient::read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
{
    if (ec.value() != 0)
    {
        std::cout << ec.message();
        return;
    }
    session_ptr->have_read += bytes;
    if (session_ptr->have_read == session_ptr->data_length)
    {
        Mark1 result = Mark1::deserialize(session_ptr->buffer.get());
        
        // здесь и происходит основная работа 
        // if (result.type == DataType::ACCEPT)
        // {
        //     std::cout << "Server is ready to communicate\n";
        //     Mark1 data;
        //     data.type = DataType::ACCEPT;
        //     size_t data_length = data.fullsize();
        //     std::unique_ptr<char[]> to_send;
        //     to_send.reset(data.serialize());
        //     write(std::move(to_send), data_length); // Говорим серверу, что клиент готов коммуницировать
        // }
        return;
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_data_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    client_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}

void FuckProctoringClient::write_callback(const boost::system::error_code &ec, size_t bytes, std::shared_ptr<WriteSession> session_ptr)
{
    if (ec.value() != 0)
    {
        return;
    }
    session_ptr->have_write += bytes;
    if (session_ptr->have_write == session_ptr->data_length)
    {
        return;
    }
    session_ptr->update_cbs();
    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->write_callback(ec, bytes, session_ptr);
    };
    client_socket.async_write_some(session_ptr->const_buffer_sequence, lambda);
}

// public slots

void FuckProctoringClient::connect(const QString &raw_ip, unsigned short port)
{
    boost::asio::ip::tcp::endpoint server_ep(boost::asio::ip::make_address_v4(raw_ip.toStdString()), port);
    client_socket.open(protocol);
    client_socket.async_connect(server_ep, [this](const boost::system::error_code &ec)
                                { this->connect_callback(ec); });
}

void FuckProctoringClient::cancel()
{
    if (client_socket.is_open())
        client_socket.cancel();
}

void FuckProctoringClient::read()
{
    std::shared_ptr<ReadSession> capacity_session(new ReadSession(4)); // сессия для чтения длины
    auto lambda = [this, capacity_session](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_capacity_callback(bytes, ec, capacity_session);
    };
    client_socket.async_read_some(capacity_session->mutable_buffer_sequence, lambda);
}

void FuckProctoringClient::write(std::unique_ptr<char[]> data, size_t dl)
{
    std::shared_ptr<WriteSession> session_ptr = std::make_shared<WriteSession>(std::move(data), dl);
    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->write_callback(ec, bytes, session_ptr);
    };
    client_socket.async_write_some(session_ptr->const_buffer_sequence, lambda);
}