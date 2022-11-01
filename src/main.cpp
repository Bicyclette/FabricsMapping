#include <QApplication>>
#include "window.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Window mapping;
    mapping.show();

    return app.exec();
}
