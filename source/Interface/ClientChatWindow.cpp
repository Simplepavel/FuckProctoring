#include "Window.hpp"
Window::ClientChatWindow::ClientChatWindow(QWidget *parent) : ChatWindow(parent)
{
    draw_chat();
}
void Window::ClientChatWindow::draw_chat()
{
    chat_VideoWidget = new QVideoWidget;
    chat_MediaPlayer = new QMediaPlayer;
    chat_MediaPlayer->setVideoOutput(chat_VideoWidget);
    chat_Layout->addWidget(chat_VideoWidget, 0, 1);
}
