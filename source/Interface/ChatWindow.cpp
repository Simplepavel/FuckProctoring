#include "Window.hpp"

Window::ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent)
{
    draw_chat();
}
void Window::ChatWindow::draw_chat()
{
    chat_Widget = new QWidget(this);
    chat_Layout = new QGridLayout;

    chat_PlainText = new QPlainTextEdit;
    chat_PlainText->setReadOnly(true);

    chat_LineEdit = new QLineEdit;
    chat_LineEdit->setPlaceholderText("Type a message...");

    chat_SendPushButton = new QPushButton;
    chat_SendPushButton->setText("Send");

    chat_ClosePushButton = new QPushButton;
    chat_ClosePushButton->setText("Close");

    chat_Layout->setColumnStretch(0, 3);
    chat_Layout->setColumnStretch(1, 7);
    chat_Layout->addWidget(chat_PlainText, 0, 0);

    chat_Layout->addWidget(chat_LineEdit, 1, 1);
    chat_Layout->addWidget(chat_SendPushButton, 2, 0, 1, 0);
    chat_Layout->addWidget(chat_ClosePushButton, 3, 0, 1, 0);

    chat_Layout->setAlignment(Qt::AlignCenter);
    chat_Widget->setLayout(chat_Layout);
}
QWidget &Window::ChatWindow::get_chat_Widget() { return *chat_Widget; }
QLineEdit &Window::ChatWindow::get_chat_LineEdit() { return *chat_LineEdit; }
QPushButton &Window::ChatWindow::get_chat_SendPushButton() { return *chat_SendPushButton; }
QPlainTextEdit &Window::ChatWindow::get_chat_PlainText() { return *chat_PlainText; }
QPushButton &Window::ChatWindow::get_chat_ClosePushButton() { return *chat_ClosePushButton; }
