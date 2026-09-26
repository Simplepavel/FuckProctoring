#include "Network.hpp"

Network::NetworkApi::NetworkApi(unsigned int port) : work(ioc.get_executor()), server(ioc, port), client(ioc)
{
    connect();
    ioc_thread = std::move(std::thread([this]()
                                       { ioc.run(); }));
}
void Network::NetworkApi::connect()
{
    QObject::connect(&server, &FuckProctoringServer::on_accept, this, [this]()
                     { emit on_accept(); });
    QObject::connect(&client, &FuckProctoringClient::on_success_connect, this, [this]()
                     { emit on_success_connect(); });

    QObject::connect(&server, &FuckProctoringServer::shutdown, this, [this]()
                     { emit shutdown(); });
    QObject::connect(&client, &FuckProctoringClient::shutdown, this, [this]()
                     { emit shutdown(); });

    QObject::connect(&server, &FuckProctoringServer::show_chat, this, [this]()
                     { emit show_chat(Network::Source::Server); });
    QObject::connect(&client, &FuckProctoringClient::show_chat, this, [this]()
                     { emit show_chat(Network::Source::Client); });

    QObject::connect(&server, &FuckProctoringServer::show_menu, this, [this]()
                     { emit show_menu(); });
    QObject::connect(&client, &FuckProctoringClient::show_menu, this, [this]()
                     { emit show_menu(); });

    QObject::connect(&server, &FuckProctoringServer::get_message, this, [this](const QString &message)
                     { emit get_message(Network::Source::Server, message); });

    QObject::connect(&client, &FuckProctoringClient::get_message, this, [this](const QString &message)
                     { emit get_message(Network::Source::Client, message); });
}
Network::FuckProctoringServer &Network::NetworkApi::get_Server() { return server; }
Network::FuckProctoringClient &Network::NetworkApi::get_Client() { return client; }
Network::NetworkApi::~NetworkApi()
{
    work.reset();
    client.cancel();
    server.cancel();
    server.server_cancel();
    ioc_thread.join();
}
