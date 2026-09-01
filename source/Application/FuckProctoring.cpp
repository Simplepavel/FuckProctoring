#include "FuckProctoring.hpp"

FuckProctoringApp::FuckProctoringApp(int argc, char *argv[], unsigned short port) : app(argc, argv), work(ioc.get_executor()), server(ioc, port), client(ioc)
{
    connect();
    window.connect();
    ioc_thread = std::move(std::thread([this]()
                                       { ioc.run(); }));
}

FuckProctoringApp::~FuckProctoringApp()
{
    work.reset();
    client.cancel();
    server.cancel();
    server.connection_with_client_cancel();
    ioc_thread.join();
}

void FuckProctoringApp::connect()
{
    QObject::connect(&window.get_main_ConnectBttn(), &QPushButton::clicked, this, &FuckProctoringApp::connect_to_server);

    QObject::connect(&server, &FuckProctoringServer::on_accept, this, &FuckProctoringApp::on_accept);
    QObject::connect(&client, &FuckProctoringClient::on_connect, this, &FuckProctoringApp::on_connect);

    QObject::connect(&server, &FuckProctoringServer::show_chat, this, &FuckProctoringApp::show_chat);
    QObject::connect(&server, &FuckProctoringServer::show_main, this, &FuckProctoringApp::show_main);

    QObject::connect(&client, &FuckProctoringClient::show_chat, this, &FuckProctoringApp::show_chat);
    QObject::connect(&client, &FuckProctoringClient::show_main, this, &FuckProctoringApp::show_main);

    QObject::connect(&window.get_chat_SendPushButton(), &QPushButton::clicked, this, &FuckProctoringApp::send_message);
    QObject::connect(&window.get_chat_ClosePushButton(), &QPushButton::clicked, this, &FuckProctoringApp::close_message);

    QObject::connect(&server, &FuckProctoringServer::shutdown, this, &FuckProctoringApp::on_shutdown);
    QObject::connect(&client, &FuckProctoringClient::shutdown, this, &FuckProctoringApp::on_shutdown);
    QObject::connect(&window, &Window::close, this, &FuckProctoringApp::on_shutdown);

    QObject::connect(&server, &FuckProctoringServer::get_message, this, &FuckProctoringApp::get_message);
    QObject::connect(&client, &FuckProctoringClient::get_message, this, &FuckProctoringApp::get_message);
}

// public slots
void FuckProctoringApp::connect_to_server()
{
    const QString &raw_ip = window.get_main_IpAddressLineEdit().text();
    unsigned int port = window.get_main_PortLineEdit().text().toUInt();
    client.connect(raw_ip, port);
}

void FuckProctoringApp::send_message()
{
    Mark1 result;
    result.data = window.get_chat_LineEdit().text().toStdString().data();
    result.type = DataType::TEXT;
    result.length = window.get_chat_LineEdit().text().toStdString().size() + 1;

    std::unique_ptr<char[]> prepare_message;
    prepare_message.reset(result.serialize());

    if (server.active())
    {
        server.write(std::move(prepare_message), result.fullsize());
    }
    else if (client.active())
    {
        client.write(std::move(prepare_message), result.fullsize());
    }
    window.get_chat_PlainText().appendPlainText("You: " + window.get_chat_LineEdit().text());
    window.get_chat_LineEdit().setText("");
}

void FuckProctoringApp::close_message() // мы иницировали разрыв
{
    if (client.active()) // мы в роли клиент
    {
        client.write(nullptr, 0);
        server.accept();
    }
    else if (server.active()) // мы в роли сервера
    {
        server.write(nullptr, 0);
        server.accept();
    }
    window.show_main();
}

void FuckProctoringApp::on_accept()
{
    window.get_chat_PlainText().clear();
    window.get_chat_LineEdit().clear();
    server.on_user_response(window.make_dialog("Connection from..."));
}

void FuckProctoringApp::on_connect()
{
    /*
    Отмена слушающего интерфейса
    */
    window.get_chat_PlainText().clear();
    window.get_chat_LineEdit().clear();
    server.cancel();
}

void FuckProctoringApp::show_main()
{
    window.get_main_IpAddressLineEdit().clear();
    window.get_main_PortLineEdit().clear();
    window.show_main();
}

void FuckProctoringApp::show_chat()
{
    window.show_chat();
}

void FuckProctoringApp::on_shutdown() // другая сторона иницировала разрыв
{
    window.show_main();
    server.accept();
}

void FuckProctoringApp::get_message(const QString &message)
{
    window.get_chat_PlainText().appendPlainText("Anonym: " + message);
}

// public
int FuckProctoringApp::start()
{
    return app.exec();
}
