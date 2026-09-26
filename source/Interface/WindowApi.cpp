#include "Window.hpp"

Window::WindowApi::WindowApi(unsigned short port, QWidget *parent) : QWidget(parent), menu_window(port), server_chat_window(this), client_chat_window(this), listOfWidget(new QStackedLayout(this))
{
    connect();
    listOfWidget->addWidget(&server_chat_window.get_chat_Widget());
    listOfWidget->addWidget(&client_chat_window.get_chat_Widget());
    listOfWidget->addWidget(&menu_window.get_menu_Widget());
    listOfWidget->setCurrentWidget(&menu_window.get_menu_Widget());
    show();
};
void Window::WindowApi::connect()
{
    // ToDo: заменить явные функции на lambda
    QObject::connect(&menu_window.get_menu_ConnectBttn(), &QPushButton::clicked, this, [this]()
                     { emit response_to_connect(); });

    QObject::connect(&server_chat_window.get_chat_SendPushButton(), &QPushButton::clicked, this, [this]()
                     { emit send_message(Source::Server); });

    QObject::connect(&client_chat_window.get_chat_SendPushButton(), &QPushButton::clicked, this, [this]()
                     { emit send_message(Source::Client); });

    QObject::connect(&server_chat_window.get_chat_ClosePushButton(), &QPushButton::clicked, this, [this]()
                     { emit close_message(Window::Source::Server); });
    QObject::connect(&client_chat_window.get_chat_ClosePushButton(), &QPushButton::clicked, this, [this]()
                     { emit close_message(Window::Source::Client); });

    QObject::connect(&server_chat_window.get_chat_SendStreamButton(), &QPushButton::clicked, this, [this]()
                     { emit send_stream(); });
}

Window::MenuWindow &Window::WindowApi::get_MenuWindow()
{
    return menu_window;
}
Window::ServerChatWindow &Window::WindowApi::get_ServerChatWindow() { return server_chat_window; }
Window::ClientChatWindow &Window::WindowApi::get_ClientChatWindow() { return client_chat_window; }
bool Window::WindowApi::make_dialog(const QString &txt, QWidget *parent)
{
    QDialog *dialog_window = new QDialog(parent);
    dialog_window->setWindowTitle("Incomming connection");
    dialog_window->setModal(true);

    QLabel *lbl = new QLabel(txt, dialog_window);
    QPushButton *ok = new QPushButton("OK", dialog_window);
    QPushButton *cancel = new QPushButton("Cancel", dialog_window);

    QHBoxLayout *inner = new QHBoxLayout;
    inner->addWidget(ok);
    inner->addWidget(cancel);

    QVBoxLayout *outer = new QVBoxLayout;

    outer->addWidget(lbl);
    outer->addLayout(inner);

    dialog_window->setLayout(outer);

    QObject::connect(ok, &QPushButton::clicked, dialog_window, &QDialog::accept);
    QObject::connect(cancel, &QPushButton::clicked, dialog_window, &QDialog::reject);

    dialog_window->show();

    int ans = dialog_window->exec();
    delete dialog_window;
    return (ans == QDialog::Accepted);
}
void Window::WindowApi::show_menu()
{
    listOfWidget->setCurrentWidget(&menu_window.get_menu_Widget());
}
void Window::WindowApi::show_chat(Window::Source src)
{
    switch (src)
    {
    case (Window::Source::Server):
        listOfWidget->setCurrentWidget(&server_chat_window.get_chat_Widget());
        break;
    case (Window::Source::Client):
        listOfWidget->setCurrentWidget(&client_chat_window.get_chat_Widget());
        break;
    default:
        break;
    }
}
void Window::WindowApi::closeEvent(QCloseEvent *event)
{
    emit close();
    event->accept();
}
