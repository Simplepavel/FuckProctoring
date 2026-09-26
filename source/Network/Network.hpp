#include <boost/asio.hpp>
#include <iostream>
#include <QObject>
#include <vector>
#include <memory>
#include <mutex>
#include <fstream>
#include <condition_variable>
#include "Protocol/Protocol.hpp"

#ifndef NETWORK
#define NETWORK

namespace Network
{
    enum class Source
    {
        Server,
        Client
    };
    class NetworkEntity : public QObject
    {
        Q_OBJECT
    protected:
        boost::system::error_code ec;
        boost::asio::io_context &ioc;
        boost::asio::ip::tcp protocol;
        boost::asio::ip::tcp::socket connection_socket;
        struct ReadSession
        {
            std::unique_ptr<char[]> buffer;
            std::vector<boost::asio::mutable_buffer> mutable_buffer_sequence;
            size_t data_length;
            size_t have_read;
            ReadSession(size_t dl) : data_length(dl), have_read(0)
            {
                buffer = std::make_unique<char[]>(data_length);
                mutable_buffer_sequence.push_back(boost::asio::mutable_buffer(buffer.get(), data_length));
            }

            void update_mbs()
            {
                mutable_buffer_sequence[0] = boost::asio::mutable_buffer(buffer.get() + have_read, data_length - have_read);
            }
        };
        struct WriteSession
        {
            std::unique_ptr<char[]> buffer;
            std::vector<boost::asio::const_buffer> const_buffer_sequence;
            size_t data_length;
            size_t have_write;
            WriteSession(std::unique_ptr<char[]> data, size_t dl) : have_write(0)
            {
                buffer = std::move(data);
                data_length = dl;
                const_buffer_sequence.push_back(boost::asio::const_buffer(buffer.get(), data_length));
            }
            void update_cbs()
            {
                const_buffer_sequence[0] = boost::asio::const_buffer(buffer.get() + have_write, data_length - have_write);
            }
        };

        void read_capacity_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr);
        virtual void read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr);
        void write_callback(const boost::system::error_code &ec, size_t bytes, std::shared_ptr<WriteSession> session);

    signals:
        void show_menu(); // запрос к графическому интерфейсу включить главное окно
        void show_chat(); // запрос к графическому интерфейсу включить чат
        void shutdown();
        void get_message(const QString &); // получили сообщение от собеседника
    public:
        NetworkEntity(boost::asio::io_context &_ioc);
        void write(std::unique_ptr<char[]> data, size_t dl);
        void read();
        void cancel(); // перегружать
    };

    class FuckProctoringServer : public NetworkEntity
    {
        Q_OBJECT
        boost::asio::ip::tcp::endpoint ep;
        boost::asio::ip::tcp::acceptor server_socket;
        void read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session_ptr);
        void accept_callback(const boost::system::error_code &ec);
    signals:
        void on_accept(); // non_blocking invoke
    public:
        FuckProctoringServer(boost::asio::io_context &_ioc, unsigned short port = 12345);
        void accept();
        void server_cancel();
        void on_user_response(bool ans);
    };

    class FuckProctoringClient : public NetworkEntity
    {
        Q_OBJECT
        std::mutex mtx;
        void connect_callback(const boost::system::error_code &ec);
        void read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session);
    signals:
        void on_success_connect();
        void get_video(const QByteArray &); // получили видео от собеседника
    public:
        FuckProctoringClient(boost::asio::io_context &_ioc);
        void connect(const QString &raw_ip, unsigned short port);
    };

    // Основной класс. Через него происходят все взаимодейстия
    class NetworkApi : public QObject
    {
        Q_OBJECT
        boost::asio::io_context ioc;
        FuckProctoringServer server;
        FuckProctoringClient client;
        boost::asio::executor_work_guard<decltype(ioc.get_executor())> work;
        std::thread ioc_thread;
        void connect();
    signals:
        void on_accept();
        void on_success_connect();
        void shutdown();
        void show_chat(Source);
        void show_menu();
        void get_message(Source, const QString &message);

    public:
        NetworkApi(unsigned int port = 5000);
        FuckProctoringServer &get_Server();
        FuckProctoringClient &get_Client();
        ~NetworkApi();
    };
};

#endif
