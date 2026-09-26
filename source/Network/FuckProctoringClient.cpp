#include "Network.hpp"

Network::FuckProctoringClient::FuckProctoringClient(boost::asio::io_context &_ioc) : Network::NetworkEntity(_ioc) {};
void Network::FuckProctoringClient::connect_callback(const boost::system::error_code &ec)
{
    if (ec.value() != 0)
    {
        std::lock_guard<std::mutex> lock_mutex(mtx);
        connection_socket.close();
        return;
    }
    else
    {
        read();
        emit on_success_connect();
    }
}
void Network::FuckProctoringClient::read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr)
{
    if (ec.value() != 0)
    {
        return;
    }
    session_ptr->have_read += bytes;
    if (session_ptr->have_read == session_ptr->data_length)
    {
        Mark1 result = Mark1::deserialize(session_ptr->buffer.get());
        if (result.type == DataType::ACCEPT)
        {
            Mark1 data;
            data.type = DataType::ACCEPT;
            size_t data_length = data.fullsize();
            std::unique_ptr<char[]> to_send;
            to_send.reset(data.serialize());
            write(std::move(to_send), data_length); // говорим серверу, что готовы принимать данные
            emit show_chat();
            result.own = true;
        }

        else if (result.type == DataType::TEXT)
        {
            QString message(result.data);
            emit get_message(message);
            result.own = true;
        }

        else if (result.type == DataType::VIDEO)
        {

            QByteArray video(result.data, result.length);
            emit get_video(video);
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
void Network::FuckProctoringClient::connect(const QString &raw_ip, unsigned short port)
{
    boost::asio::ip::tcp::endpoint server_ep(boost::asio::ip::make_address_v4(raw_ip.toStdString()), port);
    {
        std::lock_guard<std::mutex> lock_guard(mtx);
        if (!connection_socket.is_open())
        {
            connection_socket.open(protocol);
            connection_socket.async_connect(server_ep, [this](const boost::system::error_code &ec)
                                            { this->connect_callback(ec); });
        }
    }
}
