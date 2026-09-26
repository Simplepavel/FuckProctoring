#include "Application/FuckProctoring.hpp"
int main(int argc, char *argv[])
{
    unsigned short port;
    std::cout << "port: ";
    std::cin >> port;
    FuckProctoringApp app(argc, argv, port);
    return app.start();
}
