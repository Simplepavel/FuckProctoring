#include <QApplication>
#include "../Window/Window.hpp"

class FuckProctoringApp : public QObject
{
    QApplication app;
    Window window;
    void connect();

private slots:
    void on_connect();

public:
    FuckProctoringApp(int argc, char *argv[]);
    int start();
};