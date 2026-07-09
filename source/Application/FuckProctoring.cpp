#include "FuckProctoring.hpp"

FuckProctoringApp::FuckProctoringApp(int argc, char *argv[]) : app(argc, argv)
{
       
} // запуск слушаешего сокета

void FuckProctoringApp::connect()
{
    QObject::connect(&window.get_main_ConnectBttn(), &QPushButton::clicked, this, &FuckProctoringApp::on_connect);
}

void FuckProctoringApp::on_connect()
{
    qDebug() << "Button clicked";
}

int FuckProctoringApp::start()
{
    return app.exec();
}
