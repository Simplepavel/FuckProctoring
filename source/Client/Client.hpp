// Клиент - та часть, что управляет удаленно

#ifndef CLIENT
#define CLIENT
#include <boost/asio.hpp>
#include <iostream>
#include <QObject>
#include <vector>
#include <memory>
#include <mutex>
#include <fstream>
#include <condition_variable>
#include "../Protocol/Protocol.hpp"
class FuckProctoringClient : public QObject
{
    Q_OBJECT
    boost::system::error_code ec;
    boost::asio::io_context &ioc;
    boost::asio::ip::tcp protocol;
    boost::asio::ip::tcp::socket client_socket;

    std::mutex mtx;

    bool is_active;

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

    void connect_callback(const boost::system::error_code &ec);
    void read_capacity_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session);
    void read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session);
    void write_callback(const boost::system::error_code &ec, size_t bytes, std::shared_ptr<WriteSession> session_ptr);

signals:
    void on_connect();
    void show_main(); // запрос к графическому интерфейсу включить главное окно
    void show_chat(); // запрос к графическому интерфейсу включить чат
    void shutdown();
    void get_message(const QString &); // получили сообщение от собеседника
    void get_video(const QByteArray&); // получили видео от собеседника

public:
    FuckProctoringClient(boost::asio::io_context &_ioc);
    void connect(const QString &raw_ip, unsigned short port);
    void cancel();
    void read();
    void write(std::unique_ptr<char[]> data, size_t dl);
    bool active();
};

#endif