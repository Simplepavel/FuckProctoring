#include "Network.hpp"

Network::NetworkEntity::NetworkEntity(boost::asio::io_context &_ioc) : ioc(_ioc), protocol(boost::asio::ip::tcp::v4()), connection_socket(_ioc)
{
}
void Network::NetworkEntity::read_capacity_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
{
    if (ec.value() != 0)
    {
        if (ec.value() == boost::asio::error::eof)
        {
            connection_socket.close();
            emit shutdown();
        }
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
        connection_socket.async_read_some(data_session->mutable_buffer_sequence, lambda);

        return;
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_capacity_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    connection_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}
void Network::NetworkEntity::read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
{
    if (ec.value() != 0)
    {
        std::cout << ec.message();
        return;
    }
    session_ptr->have_read += bytes;
    if (session_ptr->have_read == session_ptr->data_length)
    {
        // Полезная нагрузка
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_data_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    connection_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}

void Network::NetworkEntity::write_callback(const boost::system::error_code &ec, size_t bytes, std::shared_ptr<WriteSession> session_ptr)
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
    connection_socket.async_write_some(session_ptr->const_buffer_sequence, lambda);
}
void Network::NetworkEntity::write(std::unique_ptr<char[]> data, size_t dl)
{
    if (data == nullptr)
    {
        connection_socket.shutdown(boost::asio::socket_base::shutdown_send);
        connection_socket.close();
        return;
    }
    std::shared_ptr<WriteSession> session_ptr = std::make_shared<WriteSession>(std::move(data), dl);
    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->write_callback(ec, bytes, session_ptr);
    };
    connection_socket.async_write_some(session_ptr->const_buffer_sequence, lambda);
}
void Network::NetworkEntity::read()
{
    std::shared_ptr<ReadSession> capacity_session(new ReadSession(4)); // сессия для чтения длины
    auto lambda = [this, capacity_session](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_capacity_callback(bytes, ec, capacity_session);
    };
    connection_socket.async_read_some(capacity_session->mutable_buffer_sequence, lambda);
}
void Network::NetworkEntity::cancel()
{
    if (connection_socket.is_open())
        connection_socket.cancel();
}
