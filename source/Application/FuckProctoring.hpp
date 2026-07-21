#ifndef FUCK
#define FUCK

#include <QApplication>
#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <iostream>
#include <QString>
#include <chrono>

#include "../Interface/Window.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"


class FuckProctoringApp : public QObject
{
    Q_OBJECT

    boost::asio::io_context ioc;
    std::thread ioc_thread;
    boost::asio::executor_work_guard<decltype(ioc.get_executor())> work;

    QApplication app;
    Window window;
    FuckProctoringServer server;
    FuckProctoringClient client;

    void connect();

signals:
    void signal1(const QString &raw_ip, unsigned short port);

public slots:
    void get_server_endpoint();

public:
    FuckProctoringApp(int argc, char *argv[], unsigned short port);
    ~FuckProctoringApp();
    int start();
};

#endif