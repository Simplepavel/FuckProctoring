#ifndef SERVER
#define SERVER
// Сервер - часть, которая дает доступ к управлению
#include "../Protocol/Protocol.hpp"
#include <boost/asio.hpp>
#include <QObject>
#include <QString>
class FuckProctoringServer : public QObject
{

    Q_OBJECT
    // для запуска сервера
    boost::system::error_code ec;
    boost::asio::io_context &ioc;
    boost::asio::ip::tcp::endpoint ep;
    boost::asio::ip::tcp protocol;
    boost::asio::ip::tcp::acceptor server_socket;
    // для взаимодействия с клиентов
    boost::asio::ip::tcp::socket connection_with_client_socket;

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

    void accept_callback(const boost::system::error_code &ec);
    void read_capacity_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session);
    void read_data_callback(size_t bytes, const boost::system::error_code &ec, std::shared_ptr<ReadSession> session);
    void write_callback(const boost::system::error_code &ec, size_t bytes, std::shared_ptr<WriteSession> session);

signals:
    void on_accept(const QString &); // non_blocking invoke
public slots:
    void on_user_response(bool ans);

public:
    FuckProctoringServer(boost::asio::io_context &_ioc, unsigned short port = 12345);
    void cancel();
    void read();
    void write(std::unique_ptr<char[]> data, size_t dl);
};

#endif