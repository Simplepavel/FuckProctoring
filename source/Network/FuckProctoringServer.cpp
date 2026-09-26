#include "Network.hpp"

Network::FuckProctoringServer::FuckProctoringServer(boost::asio::io_context &_ioc, unsigned short port) : Network::NetworkEntity(_ioc), server_socket(_ioc), ep(boost::asio::ip::address_v4::any(), port)
{

    server_socket.open(protocol);
    server_socket.bind(ep);
    server_socket.listen();
    server_socket.async_accept(connection_socket, [this](const boost::system::error_code &ec)
                               { this->accept_callback(ec); });
}
void Network::FuckProctoringServer::read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
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
        if (result.type == DataType::ACCEPT)
        {
            emit show_chat();
            result.own = true;
        }
        else if (result.type == DataType::TEXT)
        {
            emit get_message(result.data);
            result.own = true;
        }
        read();
        return;
    }

    auto lambda = [this, session_ptr](const boost::system::error_code &ec, size_t bytes)
    {
        this->read_data_callback(bytes, ec, session_ptr);
    };
    session_ptr->update_mbs();
    connection_socket.async_read_some(session_ptr->mutable_buffer_sequence, lambda);
}
void Network::FuckProctoringServer::accept_callback(const boost::system::error_code &ec)
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
    emit on_accept();
}
void Network::FuckProctoringServer::accept()
{
    server_socket.async_accept(connection_socket, [this](const boost::system::error_code &ec)
                               { this->accept_callback(ec); }); // сразу начинаем слушать
}
void Network::FuckProctoringServer::server_cancel()
{
    server_socket.cancel();
}
void Network::FuckProctoringServer::on_user_response(bool ans)
{
    if (!ans)
    {
        connection_socket.shutdown(boost::asio::socket_base::shutdown_send);
        connection_socket.close();
        server_socket.async_accept(connection_socket, [this](const boost::system::error_code &ec)
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
