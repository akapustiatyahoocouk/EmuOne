#include "emuone/API.hpp"
using namespace emuone;

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));    //  TODO what about Linux?
    return app.exec();
}
