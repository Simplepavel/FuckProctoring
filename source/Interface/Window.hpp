#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedLayout>
#include <QObject>
#include <QMetaObject>
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QCloseEvent>
#include <QFileDialog>
#include <iostream>
#include <future>

#ifndef WINDOW
#define WINDOW

class Window : public QWidget
{
    Q_OBJECT
    // Вспомогательные элементы
    QStackedLayout *listOfWidget;
    // Вспомогательные элементы

    // Элементы главного меню.
    QWidget *main_Widget;
    QVBoxLayout *main_Layout;
    QLineEdit *main_IpAddressLineEdit;
    QLineEdit *main_PortLineEdit;
    QPushButton *main_ConnectBttn;
    // Элементы главного меню

    // Мини чат
    QWidget *chat_Widget;
    QVBoxLayout *chat_Layout;
    QPlainTextEdit *chat_PlainText;

    QHBoxLayout *chat_InnerHLayout;
    QLineEdit *chat_LineEdit;
    QPushButton *chat_OpenFileButton;

    QPushButton *chat_SendPushButton;
    QPushButton *chat_ClosePushButton;
    //

    void draw_main();
    void draw_chat();

    void closeEvent(QCloseEvent *event) override;

    /*
    те сигналы и слоты, которые не касаются сервера или клиента
    */

private slots:
    void open_file();

signals:
    void close();

public:
    Window(QWidget *parent = nullptr);
    // main
    QPushButton &get_main_ConnectBttn();
    QLineEdit &get_main_IpAddressLineEdit();
    QLineEdit &get_main_PortLineEdit();
    // main

    // chat
    QLineEdit &get_chat_LineEdit();
    QPushButton &get_chat_SendPushButton();
    QPlainTextEdit &get_chat_PlainText();
    QPushButton &get_chat_ClosePushButton();
    QPushButton &get_chat_OpenFileButton();
    // chat

    bool make_dialog(const QString &);
    void show_main();
    void show_chat();
    void connect();
};

#endif