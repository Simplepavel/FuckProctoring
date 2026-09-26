#include "Window.hpp"

Window::MenuWindow::MenuWindow(unsigned short port, QWidget *parent) : QWidget(parent)
{
    draw_menu(port);
}
void Window::MenuWindow::draw_menu(unsigned short port)
{
    menu_Widget = new QWidget(this);
    menu_Layout = new QVBoxLayout;

    menu_PortLabel = new QLabel;
    menu_PortLabel->setText(QString("Port: ") + QString::number(port));
    menu_PortLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    menu_PortLabel->setAlignment(Qt::AlignCenter);

    menu_IpAddressLineEdit = new QLineEdit;
    menu_IpAddressLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    menu_IpAddressLineEdit->setPlaceholderText("ip adress");

    menu_PortLineEdit = new QLineEdit;
    menu_PortLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    menu_PortLineEdit->setPlaceholderText("port");

    menu_ConnectBttn = new QPushButton;
    menu_ConnectBttn->setText("connect");
    menu_ConnectBttn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    menu_Layout->addWidget(menu_PortLabel);
    menu_Layout->addWidget(menu_IpAddressLineEdit);
    menu_Layout->addWidget(menu_PortLineEdit);
    menu_Layout->addWidget(menu_ConnectBttn);

    menu_Layout->setAlignment(Qt::AlignCenter);
    menu_Widget->setLayout(menu_Layout);
}
QWidget &Window::MenuWindow::get_menu_Widget() { return *menu_Widget; }
QPushButton &Window::MenuWindow::get_menu_ConnectBttn() { return *menu_ConnectBttn; }
QLineEdit &Window::MenuWindow::get_menu_IpAddressLineEdit() { return *menu_IpAddressLineEdit; }
QLineEdit &Window::MenuWindow::get_menu_PortLineEdit() { return *menu_PortLineEdit; }
