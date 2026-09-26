#include "FuckProctoring.hpp"

FuckProctoringApp::FuckProctoringApp(int argc, char *argv[], unsigned short port) : app(argc, argv), window(port), network(port)
{
    connect();
}

void FuckProctoringApp::connect()
{
    QObject::connect(&window, &WindowApi::response_to_connect, this, &FuckProctoringApp::connect_to_server); //+
    QObject::connect(&window, &WindowApi::send_message, this, &FuckProctoringApp::send_message);             //+
    QObject::connect(&window, &WindowApi::close_message, this, &FuckProctoringApp::close_message);           // +
    QObject::connect(&window, &WindowApi::send_stream, this, &FuckProctoringApp::send_stream);
    QObject::connect(&window, &WindowApi::close, this, &FuckProctoringApp::on_shutdown); // +

    QObject::connect(&network, &NetworkApi::on_accept, this, &FuckProctoringApp::on_accept);                   //+
    QObject::connect(&network, &NetworkApi::on_success_connect, this, &FuckProctoringApp::on_success_connect); //+
    QObject::connect(&network, &NetworkApi::shutdown, this, &FuckProctoringApp::on_shutdown);                  //+
    QObject::connect(&network, &NetworkApi::show_chat, this, &FuckProctoringApp::show_chat);                   //+
    QObject::connect(&network, &NetworkApi::show_menu, this, &FuckProctoringApp::show_menu);                   //+
    QObject::connect(&network, &NetworkApi::get_message, this, &FuckProctoringApp::get_message);
}

// public slots
void FuckProctoringApp::connect_to_server()
{
    const QString &raw_ip = window.get_MenuWindow().get_menu_IpAddressLineEdit().text();
    const QString &port = window.get_MenuWindow().get_menu_PortLineEdit().text();
    if (raw_ip.isEmpty() || port.isEmpty())
    {
        std::cout << "Empty ip or port field\n";
        return;
    }
    FuckProctoringClient &client = network.get_Client();
    client.connect(raw_ip, port.toUInt());
}

void FuckProctoringApp::send_message(Window::Source src)
{
    ChatWindow *sender;
    NetworkEntity *net_sender;
    switch (src)
    {
    case (Window::Source::Client):
        sender = &window.get_ClientChatWindow();
        net_sender = &network.get_Client();
        break;
    case (Window::Source::Server):
        sender = &window.get_ServerChatWindow();
        net_sender = &network.get_Server();
    default:
        break;
    }
    Mark1 result;
    result.data = sender->get_chat_LineEdit().text().toStdString().data();
    result.type = DataType::TEXT;
    result.length = sender->get_chat_LineEdit().text().toStdString().size() + 1;
    std::unique_ptr<char[]> prepare_message;
    prepare_message.reset(result.serialize());
    net_sender->write(std::move(prepare_message), result.fullsize());
    sender->get_chat_PlainText().appendPlainText("You: " + sender->get_chat_LineEdit().text());
    sender->get_chat_LineEdit().setText("");
}

void FuckProctoringApp::close_message(Window::Source src) // мы иницировали разрыв
{
    // NetworkEntity *sender;
    FuckProctoringServer &server = network.get_Server();
    FuckProctoringClient &client = network.get_Client();
    switch (src)
    {
    case (Window::Source::Server):
        server.write(nullptr, 0);
        break;
    case (Window::Source::Client):
        client.write(nullptr, 0);
        break;
    default:
        break;
    }
    server.accept();
    window.show_menu();
}

void FuckProctoringApp::on_accept() // Серверная сторона принимает соединение от клиент
{
    FuckProctoringServer &server = network.get_Server();
    bool ans = window.make_dialog("Connection from...", &window.get_MenuWindow().get_menu_Widget());
    server.on_user_response(ans);
    if (ans)
    {
        ServerChatWindow &server_chat_window = window.get_ServerChatWindow();
        server_chat_window.get_chat_PlainText().clear();
        server_chat_window.get_chat_LineEdit().clear();
        window.show_chat(Window::Source::Server);
    }
}

void FuckProctoringApp::on_success_connect() // Клиентская сторона инициирует соединение
{
    FuckProctoringServer &server = network.get_Server();
    ClientChatWindow &client_chat_window = window.get_ClientChatWindow();
    client_chat_window.get_chat_PlainText().clear();
    client_chat_window.get_chat_LineEdit().clear();
    server.server_cancel(); // Отмена слушающего интерфейса
}

void FuckProctoringApp::on_shutdown() // другая сторона иницировала разрыв
{
    FuckProctoringServer &server = network.get_Server();
    window.show_menu();
    server.accept();
}

void FuckProctoringApp::send_stream() // была нажата кнопка отправить видео
{
    std::cout << "Server want to send stream\n";
}

void FuckProctoringApp::show_chat(Network::Source src)
{
    switch (src)
    {
    case (Network::Source::Server):
        window.show_chat(Window::Source::Server);
        break;
    case (Network::Source::Client):
        window.show_chat(Window::Source::Client);
        break;
    default:
        break;
    }
}

void FuckProctoringApp::show_menu()
{
    MenuWindow &menuWindow = window.get_MenuWindow();
    menuWindow.get_menu_IpAddressLineEdit().clear();
    menuWindow.get_menu_PortLineEdit().clear();
    window.show_menu();
}

void FuckProctoringApp::get_message(Network::Source src, const QString &message)
{
    ChatWindow *sender;
    switch (src)
    {
    case (Network::Source::Server):
        sender = &window.get_ServerChatWindow();
        break;
    case (Network::Source::Client):
        sender = &window.get_ClientChatWindow();
        break;
    default:
        break;
    }
    sender->get_chat_PlainText().appendPlainText("Anonym: " + message);
}

// void FuckProctoringApp::client_get_video(const QByteArray &_video)
// {
//     video = _video;
//     QMediaPlayer &chat_MediaPlayer = client_window.get_chat_MediaPlayer();
//     chat_MediaPlayer.stop();
//     chat_MediaPlayer.setSourceDevice(nullptr, QUrl());
//     if (qbuffer.isOpen())
//     {
//         qbuffer.close();
//     }
//     qbuffer.setBuffer(&video);
//     qbuffer.open(QBuffer::ReadOnly);
//     chat_MediaPlayer.setSourceDevice(&qbuffer, QUrl("memory://video.mp4"));
//     chat_MediaPlayer.play();
// }

// public
int FuckProctoringApp::start()
{
    return app.exec();
}
