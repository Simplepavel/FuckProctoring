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
#include <QString>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QList>
#include <iostream>
#include <future>

#ifndef WINDOW
#define WINDOW

namespace Window
{
    enum class Source
    {
        Client,
        Server
    };
    class MenuWindow : public QWidget
    {
        Q_OBJECT
    private:
        // Элементы главного меню.
        QWidget *menu_Widget;
        QVBoxLayout *menu_Layout;
        QLabel *menu_PortLabel;
        QLineEdit *menu_IpAddressLineEdit;
        QLineEdit *menu_PortLineEdit;
        QPushButton *menu_ConnectBttn;
        // Элементы главного меню

        void draw_menu(unsigned short port);

    public:
        MenuWindow(unsigned short port, QWidget *parent = nullptr);
        QWidget &get_menu_Widget();
        QPushButton &get_menu_ConnectBttn();
        QLineEdit &get_menu_IpAddressLineEdit();
        QLineEdit &get_menu_PortLineEdit();
    };
    class ChatWindow : public QWidget
    {

        Q_OBJECT
    protected:
        QWidget *chat_Widget;
        QGridLayout *chat_Layout;

        QPlainTextEdit *chat_PlainText;

        QLineEdit *chat_LineEdit;

        QPushButton *chat_SendPushButton;
        QPushButton *chat_ClosePushButton;

    public:
        ChatWindow(QWidget *parent = nullptr);
        void draw_chat();
        QWidget &get_chat_Widget();
        QLineEdit &get_chat_LineEdit();
        QPushButton &get_chat_SendPushButton();
        QPlainTextEdit &get_chat_PlainText();
        QPushButton &get_chat_ClosePushButton();
    };
    class ServerChatWindow : public ChatWindow
    {
        Q_OBJECT
        QPushButton *chat_SendStreamButton;

    public:
        ServerChatWindow(QWidget *parent = nullptr);
        void draw_chat();
        QPushButton &get_chat_SendStreamButton();
    };
    class ClientChatWindow : public ChatWindow
    {
        Q_OBJECT
        QVideoWidget *chat_VideoWidget;
        QMediaPlayer *chat_MediaPlayer;

    public:
        ClientChatWindow(QWidget *parent = nullptr);
        void draw_chat();
    };
    class WindowApi : public QWidget
    {
        Q_OBJECT
        MenuWindow menu_window;
        ServerChatWindow server_chat_window;
        ClientChatWindow client_chat_window;
        QStackedLayout *listOfWidget;
        void connect();
        void closeEvent(QCloseEvent *event) override;

    signals:
        void response_to_connect(); // нажата кнонка "Connect" главного меню
        void send_message(Source);
        void send_stream();
        void close_message(Source);
        void close();

    public:
        WindowApi(unsigned short port = 5000, QWidget *parent = nullptr);
        MenuWindow &get_MenuWindow();
        ServerChatWindow &get_ServerChatWindow();
        ClientChatWindow &get_ClientChatWindow();
        bool make_dialog(const QString &, QWidget *);

        // Замена виджета на вершине стека
        void show_menu();
        void show_chat(Source);
    };
};
#endif