#ifndef FUCK
#define FUCK

#include <QApplication>
#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <iostream>
#include <fstream>
#include <QString>
#include <chrono>
#include "../Interface/Window.hpp"
#include "../Network/Network.hpp"
#include <QBuffer>

using namespace Window;
using namespace Network;

class FuckProctoringApp : public QObject
{
    Q_OBJECT

    QApplication app;
    QByteArray video;
    QBuffer qbuffer;

    WindowApi window;
    NetworkApi network;

    void connect();

public slots:
    void connect_to_server();
    void send_message(Window::Source src);
    void send_stream();
    void close_message(Window::Source src);
    void on_accept();
    void on_success_connect();
    void show_menu();
    void show_chat(Network::Source);
    void on_shutdown();
    void get_message(Network::Source, const QString &);

public:
    FuckProctoringApp(int argc, char *argv[], unsigned short port);
    int start();
};

#endif