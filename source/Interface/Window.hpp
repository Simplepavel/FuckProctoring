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

    void draw_menu();
signals:
    void user_response(bool);

public slots:
    void make_dialog(const QString &);

public:
    Window(QWidget *parent = nullptr);
    const QPushButton &get_main_ConnectBttn();
    const QLineEdit &get_main_IpAddressLineEdit();
    const QLineEdit &get_main_PortLineEdit();
};

#endif