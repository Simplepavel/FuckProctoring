#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedLayout>
#include <QObject>

class Window : public QWidget
{

    // Вспомогательные элементы
    QStackedLayout *listOfWidget;
    // Вспомогательные элементы

    // Элементы главного меню.
    QWidget *main_Widget;
    QVBoxLayout *main_Layout;
    QLineEdit *main_IpAddressLineEdit;
    QPushButton *main_ConnectBttn;
    // Элементы главного меню

    void draw_menu();

public:
    Window(QWidget *parent = nullptr);
    const QPushButton &get_main_ConnectBttn();
    
};