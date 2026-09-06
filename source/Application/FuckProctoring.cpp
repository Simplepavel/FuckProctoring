#include "FuckProctoring.hpp"

FuckProctoringApp::FuckProctoringApp(int argc, char *argv[], unsigned short port) : work(ioc.get_executor()), app(argc, argv), window(port), server(ioc, port), client(ioc)
{

    // QMediaPlayer &chat_MediaPlayer = window.get_chat_MediaPlayer();
    // chat_MediaPlayer.setSourceDevice(&qbuffer);

    connect();
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
    QObject::connect(&window.get_chat_OpenFileButton(), &QPushButton::clicked, this, &FuckProctoringApp::send_video);

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

    QObject::connect(&server, &FuckProctoringServer::get_video, this, &FuckProctoringApp::get_video);
    QObject::connect(&client, &FuckProctoringClient::get_video, this, &FuckProctoringApp::get_video);
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

void FuckProctoringApp::send_video()
{
    QList<QString> files = window.make_file_dialog();
    if (files.empty())
        return;
    QString filename = files[0];
    std::ifstream video(filename.toStdString(), std::ios_base::binary);
    video.seekg(0, std::ios_base::end);
    std::size_t length = (std::size_t)video.tellg();
    video.seekg(0, std::ios_base::beg);

    Mark1 result;
    result.data = new char[length];
    result.type = DataType::VIDEO;
    result.length = length;
    result.own = true;

    video.read(result.data, result.length);
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

void FuckProctoringApp::get_video(const QByteArray &_video)
{
    video = _video;
    QMediaPlayer &chat_MediaPlayer = window.get_chat_MediaPlayer();
    chat_MediaPlayer.stop();
    chat_MediaPlayer.setSourceDevice(nullptr, QUrl());
    if (qbuffer.isOpen())
    {
        qbuffer.close();
    }
    qbuffer.setBuffer(&video);
    qbuffer.open(QBuffer::ReadOnly);
    chat_MediaPlayer.setSourceDevice(&qbuffer, QUrl("memory://video.mp4"));
    chat_MediaPlayer.play();
}

// public
int FuckProctoringApp::start()
{
    return app.exec();
}
