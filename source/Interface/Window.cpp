#include "Window.hpp"

Window::Window(unsigned short port, QWidget *parent) : QWidget(parent)
{
    listOfWidget = new QStackedLayout(this);

    // Главное меню
    draw_main(port);
    // Для теста
    draw_chat();

    // Добавление всех дочерних виджетов в стек виджетов
    listOfWidget->addWidget(main_Widget);
    // Для теста
    listOfWidget->addWidget(chat_Widget);
    listOfWidget->setAlignment(Qt::AlignCenter);

    // Отрисовка
    resize(640, 480);
    show();
}

void Window::draw_main(unsigned short port)
{
    // Главное меню
    main_Widget = new QWidget(this);
    main_Layout = new QVBoxLayout;

    main_PortLabel = new QLabel;
    main_PortLabel->setText(QString("Port: ") + QString::number(port));
    main_PortLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    main_PortLabel->setAlignment(Qt::AlignCenter);

    main_IpAddressLineEdit = new QLineEdit;
    main_IpAddressLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    main_IpAddressLineEdit->setPlaceholderText("ip adress");

    main_PortLineEdit = new QLineEdit;
    main_PortLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    main_PortLineEdit->setPlaceholderText("port");

    main_ConnectBttn = new QPushButton;
    main_ConnectBttn->setText("connect");
    main_ConnectBttn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    main_Layout->addWidget(main_PortLabel);
    main_Layout->addWidget(main_IpAddressLineEdit);
    main_Layout->addWidget(main_PortLineEdit);
    main_Layout->addWidget(main_ConnectBttn);

    main_Layout->setAlignment(Qt::AlignCenter);
    main_Widget->setLayout(main_Layout);
    // Главное меню
}

void Window::draw_chat()
{
    chat_Widget = new QWidget(this);
    chat_Layout = new QGridLayout;

    chat_PlainText = new QPlainTextEdit;
    chat_PlainText->setReadOnly(true);

    chat_VideoWidget = new QVideoWidget;
    chat_MediaPlayer = new QMediaPlayer;

    chat_MediaPlayer->setVideoOutput(chat_VideoWidget);

    chat_LineEdit = new QLineEdit;
    chat_LineEdit->setPlaceholderText("Type a message...");

    chat_OpenFileButton = new QPushButton;
    chat_OpenFileButton->setText("...");

    chat_SendPushButton = new QPushButton;
    chat_SendPushButton->setText("Send");

    chat_ClosePushButton = new QPushButton;
    chat_ClosePushButton->setText("Close");

    chat_Layout->setColumnStretch(0, 3);
    chat_Layout->setColumnStretch(1, 7);
    chat_Layout->addWidget(chat_PlainText, 0, 0);
    // chat_Layout->addWidget(chat_VideoWidget, 0, 1);

    chat_Layout->addWidget(chat_LineEdit, 1, 1);
    chat_Layout->addWidget(chat_OpenFileButton, 1, 0);
    chat_Layout->addWidget(chat_SendPushButton, 2, 0, 1, 0);
    chat_Layout->addWidget(chat_ClosePushButton, 3, 0, 1, 0);

    chat_Layout->setAlignment(Qt::AlignCenter);
    chat_Widget->setLayout(chat_Layout);
}

void Window::closeEvent(QCloseEvent *event)
{
    /*
    по необходимости пишем дополнительный функционал
    */
    emit close();
    event->accept();
}

QPushButton &Window::get_main_ConnectBttn()
{
    return *main_ConnectBttn;
}
QLineEdit &Window::get_main_IpAddressLineEdit()
{
    return *main_IpAddressLineEdit;
}
QLineEdit &Window::get_main_PortLineEdit()
{
    return *main_PortLineEdit;
}

QLineEdit &Window::get_chat_LineEdit()
{
    return *chat_LineEdit;
}
QPushButton &Window::get_chat_SendPushButton()
{
    return *chat_SendPushButton;
}
QPlainTextEdit &Window::get_chat_PlainText() { return *chat_PlainText; }
QMediaPlayer &Window::get_chat_MediaPlayer() { return *chat_MediaPlayer; }
QPushButton &Window::get_chat_ClosePushButton() { return *chat_ClosePushButton; }
QPushButton &Window::get_chat_OpenFileButton() { return *chat_OpenFileButton; }

bool Window::make_dialog(const QString &txt)
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
    delete dialog_window;
    return (ans == QDialog::Accepted);
    // emit user_response(ans == QDialog::Accepted);
}

QList<QString> Window::make_file_dialog()
{
    QFileDialog *openfile_window = new QFileDialog(this);
    openfile_window->setNameFilter("(*.mp4 *.mov *.mpeg-1 *.mpeg-2 *.mpeg4 *.avi)");
    openfile_window->setFileMode(QFileDialog::ExistingFile);
    int ans = openfile_window->exec();
    QList<QString> result;
    if (ans == QFileDialog::Accepted)
    {
        result = openfile_window->selectedFiles();
    }
    delete openfile_window;
    return result;
}

void Window::show_chat()
{
    listOfWidget->setCurrentWidget(chat_Widget);
}

void Window::show_main()
{
    listOfWidget->setCurrentWidget(main_Widget);
}
