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

    main_PortLineEdit = new QLineEdit;
    main_PortLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    main_PortLineEdit->setPlaceholderText("port");

    main_ConnectBttn = new QPushButton;
    main_ConnectBttn->setText("connect");
    main_ConnectBttn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    main_Layout->addWidget(main_IpAddressLineEdit);
    main_Layout->addWidget(main_PortLineEdit);
    main_Layout->addWidget(main_ConnectBttn);

    main_Layout->setAlignment(Qt::AlignCenter);
    main_Widget->setLayout(main_Layout);
    // Главное меню
}

const QPushButton &Window::get_main_ConnectBttn()
{
    return *main_ConnectBttn;
}

const QLineEdit &Window::get_main_IpAddressLineEdit()
{
    return *main_IpAddressLineEdit;
}

const QLineEdit &Window::get_main_PortLineEdit()
{
    return *main_PortLineEdit;
}

void Window::make_dialog(const QString &txt)
{
    QDialog *dialog_window = new QDialog(main_Widget);
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
    emit user_response(ans == QDialog::Accepted);
}
