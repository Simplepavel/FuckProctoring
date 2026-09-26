#include "Window.hpp"

Window::ServerChatWindow::ServerChatWindow(QWidget *parent) : ChatWindow(parent)
{
    draw_chat();
}
void Window::ServerChatWindow::draw_chat()
{
    chat_SendStreamButton = new QPushButton;
    chat_SendStreamButton->setText("...");
    chat_Layout->addWidget(chat_SendStreamButton, 1, 0);
}

QPushButton &Window::ServerChatWindow::get_chat_SendStreamButton()
{
    return *chat_SendStreamButton;
}
