#include "FuckProctoring.hpp"

FuckProctoringApp::FuckProctoringApp(int argc, char *argv[], unsigned short port) : app(argc, argv), work(ioc.get_executor()), server(ioc, port), client(ioc)
{
    connect();
    ioc_thread = std::move(std::thread([this]()
                                       { ioc.run(); }));
}

FuckProctoringApp::~FuckProctoringApp()
{
    work.reset();
    client.cancel();
    server.cancel();
    ioc_thread.join();
}

void FuckProctoringApp::connect()
{
    QObject::connect(&window.get_main_ConnectBttn(), &QPushButton::clicked, this, &FuckProctoringApp::get_server_endpoint);
    QObject::connect(&server, &FuckProctoringServer::on_accept, &window, &Window::make_dialog);
    QObject::connect(&window, &Window::user_response, &server, &FuckProctoringServer::on_user_response);
}

// public slots
void FuckProctoringApp::get_server_endpoint()
{
    const QString &raw_ip = window.get_main_IpAddressLineEdit().text();
    unsigned int port = window.get_main_PortLineEdit().text().toUInt();
    client.connect(raw_ip, port);
}

// public
int FuckProctoringApp::start()
{
    return app.exec();
}
