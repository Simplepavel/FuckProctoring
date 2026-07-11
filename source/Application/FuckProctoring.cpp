#include "FuckProctoring.hpp"

FuckProctoringApp::FuckProctoringApp(int argc, char *argv[], unsigned short port) : app(argc, argv), ep(boost::asio::ip::address_v4::any(), port), protocol(boost::asio::ip::tcp::v4()), server_socket(ioc), client_socket(ioc)
{
    qobject_connect();

    server_socket.open(protocol);
    server_socket.bind(ep);
    server_socket.listen();
    auto lambda = [this]()
    { std::cout << "wait for connection\n"; server_socket.accept(client_socket, ec); };
    server_thread = std::move(std::thread(lambda));
} // запуск нового потока, ожидающего подключения

FuckProctoringApp::~FuckProctoringApp()
{
    if (client_socket.is_open()) // в этом состоянии нет активного слущающего потока, он есть поток обмена информацией
    {
        client_socket.shutdown(boost::asio::socket_base::shutdown_send);
        client_socket.close();
    }
    else
    {
        server_socket.close();
        server_thread.join();
    }
}

void FuckProctoringApp::qobject_connect()
{
    QObject::connect(&window.get_main_ConnectBttn(), &QPushButton::clicked, this, &FuckProctoringApp::qobject_on_connect);
}

void FuckProctoringApp::qobject_on_connect()
{
    std::cout << "Button clicked";
}

int FuckProctoringApp::start()
{
    return app.exec();
}
