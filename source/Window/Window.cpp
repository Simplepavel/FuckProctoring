#include "Window.hpp"

Window::Window(QWidget *parent) : QWidget(parent)
{
    listOfWidget = new QStackedLayout(this);

    // Главное меню
    draw_menu();

    // Добавление всех дочерних виджетов в стек виджетов
    listOfWidget->addWidget(main_Widget);
    listOfWidget->setAlignment(Qt::AlignCenter);

    // Отрисовка
    resize(640, 480);
    show();
}

void Window::draw_menu()
{
    // Главное меню
    main_Widget = new QWidget(this);
    main_Layout = new QVBoxLayout;

    main_IpAddressLineEdit = new QLineEdit;
    main_IpAddressLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    main_IpAddressLineEdit->setPlaceholderText("ip adress");

    main_ConnectBttn = new QPushButton;
    main_ConnectBttn->setText("connect");
    main_ConnectBttn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    main_Layout->addWidget(main_IpAddressLineEdit);
    main_Layout->addWidget(main_ConnectBttn);

    main_Layout->setAlignment(Qt::AlignCenter);
    main_Widget->setLayout(main_Layout);
    // Главное меню
}

const QPushButton &Window::get_main_ConnectBttn()
{
    return *main_ConnectBttn;
}