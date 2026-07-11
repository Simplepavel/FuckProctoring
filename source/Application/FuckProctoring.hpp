#include <QApplication>
#include "../Window/Window.hpp"
#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <iostream>

#define PORT 6767
#define AUTOMOC_MACRO_NAMES

class FuckProctoringApp : public QObject
{
    Q_OBJECT

    QApplication app;
    Window window;

    // для запуска сервера
    boost::system::error_code ec;
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::endpoint ep;
    boost::asio::ip::tcp protocol;
    boost::asio::ip::tcp::acceptor server_socket;
    boost::asio::ip::tcp::socket client_socket;
    std::thread server_thread;
    // для запуска сервера

    void qobject_connect();
private slots:
    void qobject_on_connect();

public:
    FuckProctoringApp(int argc, char *argv[], unsigned short port);
    ~FuckProctoringApp();
    int start();
};