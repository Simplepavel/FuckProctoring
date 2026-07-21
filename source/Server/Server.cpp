#include "Server.hpp"

FuckProctoringServer::FuckProctoringServer(boost::asio::io_context &_ioc, unsigned short port) : ioc(_ioc), ep(boost::asio::ip::address_v4::any(), port), protocol(boost::asio::ip::tcp::v4()), server_socket(ioc), connection_with_client_socket(ioc)
{
    server_socket.open(protocol);
    server_socket.bind(ep);
    server_socket.listen();
    server_socket.async_accept(connection_with_client_socket, [this](const boost::system::error_code &ec)
                               { this->accept_callback(ec); });
}

// callback

void FuckProctoringServer::accept_callback(const boost::system::error_code &ec)
{
    if (ec.value() != 0)
    {
        if (ec != boost::asio::error::operation_aborted)
        {
            // std::cout << "Something went wrong in async_connect";
            // std::cout << ec.message();
        }
        return;
    }
    emit on_accept(QString::fromStdString("Connection from..."));
}

void FuckProctoringServer::read_capacity_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
{
    if (ec.value() != 0)
    {
        if (ec.value() == boost::asio::error::eof)
            connection_with_client_socket.close();
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
        connection_with_client_socket.async_read_some(data_session->mutable_buffer_sequence, lambda);

        return;
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_capacity_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    connection_with_client_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}

void FuckProctoringServer::read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
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
        // if (result.type == DataType::ACCEPT)
        // {
        //     std::cout << "Client is ready to communicate\n";
        // }
        // return;
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_data_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    connection_with_client_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}

void FuckProctoringServer::write_callback(const boost::system::error_code &ec, size_t bytes, std::shared_ptr<WriteSession> session_ptr)
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
    connection_with_client_socket.async_write_some(session_ptr->const_buffer_sequence, lambda);
}

// public slots
void FuckProctoringServer::on_user_response(bool ans)
{
    if (!ans)
    {
        connection_with_client_socket.shutdown(boost::asio::socket_base::shutdown_send);
        connection_with_client_socket.close();
        server_socket.async_accept(connection_with_client_socket, [this](const boost::system::error_code &ec)
                                   { this->accept_callback(ec); });
    }
    else
    {
        Mark1 data;
        data.type = DataType::ACCEPT;
        size_t data_length = data.fullsize();
        std::unique_ptr<char[]> to_send;
        to_send.reset(data.serialize());
        write(std::move(to_send), data_length); // говорим клиенту, что готовы отправлять данные
        read();
    }
}

void FuckProctoringServer::cancel()
{
    server_socket.cancel();
}

void FuckProctoringServer::read()
{
    std::shared_ptr<ReadSession> capacity_session(new ReadSession(4)); // сессия для чтения длины
    auto lambda = [this, capacity_session](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_capacity_callback(bytes, ec, capacity_session);
    };
    connection_with_client_socket.async_read_some(capacity_session->mutable_buffer_sequence, lambda);
}

void FuckProctoringServer::write(std::unique_ptr<char[]> data, size_t dl)
{
    std::shared_ptr<WriteSession> session_ptr = std::make_shared<WriteSession>(std::move(data), dl);
    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->write_callback(ec, bytes, session_ptr);
    };
    connection_with_client_socket.async_write_some(session_ptr->const_buffer_sequence, lambda);
}