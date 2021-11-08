//
//  emuone/Main.cpp
//
//  EmuOne entry point
//
//////////
#include "emuone/API.hpp"

//////////
//  EmuOne entry point
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    util::PluginManager::loadPlugins();

    /*
    util::Sha1MessageDigest::Builder * mdb = util::Sha1MessageDigest::instance()->createBuilder();
    mdb->digestBytes("abc", 3);
    qDebug() << mdb->digestString();

    mdb->reset();
    mdb->digestBytes("", 0);
    qDebug() << mdb->digestString();

    mdb->reset();
    mdb->digestBytes("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);
    qDebug() << mdb->digestString();

    mdb->reset();
    mdb->digestBytes("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", 112);
    qDebug() << mdb->digestString();

    mdb->reset();
    mdb->digestBytes(QString(1000000, QChar('a')).toLatin1().data(), 1000000);
    qDebug() << mdb->digestString();
    */

    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}

//  End of emuone/Main.cpp
