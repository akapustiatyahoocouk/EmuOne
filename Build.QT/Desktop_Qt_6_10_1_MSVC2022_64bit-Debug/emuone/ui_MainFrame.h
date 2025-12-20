/********************************************************************************
** Form generated from reading UI file 'MainFrame.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRAME_H
#define UI_MAINFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

namespace emuone {

class Ui_MainFrame
{
public:
    QAction *actionExit;
    QAction *actionNewVirtualMachine;
    QAction *actionOpenVirtualMachine;
    QAction *actionCloseVirtualMachine;
    QAction *actionAbout;
    QAction *actionPreferences;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionSuspend;
    QAction *actionResume;
    QAction *actionReset;
    QAction *actionConfigure;
    QAction *actionFullScreen;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *homeTab;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuVm;
    QMenu *menuHelp;
    QMenu *menuTools;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *emuone__MainFrame)
    {
        if (emuone__MainFrame->objectName().isEmpty())
            emuone__MainFrame->setObjectName("emuone__MainFrame");
        emuone__MainFrame->resize(355, 234);
        actionExit = new QAction(emuone__MainFrame);
        actionExit->setObjectName("actionExit");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/ExitSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionExit->setIcon(icon);
        actionNewVirtualMachine = new QAction(emuone__MainFrame);
        actionNewVirtualMachine->setObjectName("actionNewVirtualMachine");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/NewSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionNewVirtualMachine->setIcon(icon1);
        actionOpenVirtualMachine = new QAction(emuone__MainFrame);
        actionOpenVirtualMachine->setObjectName("actionOpenVirtualMachine");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/OpenSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionOpenVirtualMachine->setIcon(icon2);
        actionCloseVirtualMachine = new QAction(emuone__MainFrame);
        actionCloseVirtualMachine->setObjectName("actionCloseVirtualMachine");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/CloseSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionCloseVirtualMachine->setIcon(icon3);
        actionAbout = new QAction(emuone__MainFrame);
        actionAbout->setObjectName("actionAbout");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/AboutSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionAbout->setIcon(icon4);
        actionPreferences = new QAction(emuone__MainFrame);
        actionPreferences->setObjectName("actionPreferences");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/PreferencesSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionPreferences->setIcon(icon5);
        actionStart = new QAction(emuone__MainFrame);
        actionStart->setObjectName("actionStart");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/StartSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionStart->setIcon(icon6);
        actionStop = new QAction(emuone__MainFrame);
        actionStop->setObjectName("actionStop");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/StopSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionStop->setIcon(icon7);
        actionSuspend = new QAction(emuone__MainFrame);
        actionSuspend->setObjectName("actionSuspend");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/SuspendSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionSuspend->setIcon(icon8);
        actionResume = new QAction(emuone__MainFrame);
        actionResume->setObjectName("actionResume");
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/ResumeSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionResume->setIcon(icon9);
        actionReset = new QAction(emuone__MainFrame);
        actionReset->setObjectName("actionReset");
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/ResetSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionReset->setIcon(icon10);
        actionConfigure = new QAction(emuone__MainFrame);
        actionConfigure->setObjectName("actionConfigure");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/ConfigureSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionConfigure->setIcon(icon11);
        actionFullScreen = new QAction(emuone__MainFrame);
        actionFullScreen->setObjectName("actionFullScreen");
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/MakeFullScreenSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionFullScreen->setIcon(icon12);
        centralwidget = new QWidget(emuone__MainFrame);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        homeTab = new QWidget();
        homeTab->setObjectName("homeTab");
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/emuone/Resources/Images/Misc/HomeSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        tabWidget->addTab(homeTab, icon13, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        emuone__MainFrame->setCentralWidget(centralwidget);
        menubar = new QMenuBar(emuone__MainFrame);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 355, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuVm = new QMenu(menubar);
        menuVm->setObjectName("menuVm");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        menuTools = new QMenu(menubar);
        menuTools->setObjectName("menuTools");
        emuone__MainFrame->setMenuBar(menubar);
        statusbar = new QStatusBar(emuone__MainFrame);
        statusbar->setObjectName("statusbar");
        emuone__MainFrame->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuVm->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNewVirtualMachine);
        menuFile->addAction(actionOpenVirtualMachine);
        menuFile->addSeparator();
        menuFile->addAction(actionCloseVirtualMachine);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuVm->addAction(actionStart);
        menuVm->addAction(actionStop);
        menuVm->addSeparator();
        menuVm->addAction(actionSuspend);
        menuVm->addAction(actionResume);
        menuVm->addSeparator();
        menuVm->addAction(actionReset);
        menuVm->addSeparator();
        menuVm->addAction(actionConfigure);
        menuVm->addSeparator();
        menuVm->addAction(actionFullScreen);
        menuHelp->addAction(actionAbout);
        menuTools->addAction(actionPreferences);

        retranslateUi(emuone__MainFrame);
        QObject::connect(actionExit, SIGNAL(triggered()), emuone__MainFrame, SLOT(_onActionExit()));
        QObject::connect(actionNewVirtualMachine, SIGNAL(triggered()), emuone__MainFrame, SLOT(_onActionNewVirtualMachine()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(emuone__MainFrame);
    } // setupUi

    void retranslateUi(QMainWindow *emuone__MainFrame)
    {
        emuone__MainFrame->setWindowTitle(QCoreApplication::translate("emuone::MainFrame", "EmuOne", nullptr));
        actionExit->setText(QCoreApplication::translate("emuone::MainFrame", "E&xit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNewVirtualMachine->setText(QCoreApplication::translate("emuone::MainFrame", "&New virtual machine", nullptr));
#if QT_CONFIG(shortcut)
        actionNewVirtualMachine->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpenVirtualMachine->setText(QCoreApplication::translate("emuone::MainFrame", "&Open virtual machine", nullptr));
#if QT_CONFIG(shortcut)
        actionOpenVirtualMachine->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCloseVirtualMachine->setText(QCoreApplication::translate("emuone::MainFrame", "C&lose virtual machine", nullptr));
#if QT_CONFIG(shortcut)
        actionCloseVirtualMachine->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+F4", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("emuone::MainFrame", "A&bout", nullptr));
#if QT_CONFIG(shortcut)
        actionAbout->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+F1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPreferences->setText(QCoreApplication::translate("emuone::MainFrame", "&Preferences", nullptr));
#if QT_CONFIG(shortcut)
        actionPreferences->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStart->setText(QCoreApplication::translate("emuone::MainFrame", "S&tart", nullptr));
#if QT_CONFIG(shortcut)
        actionStart->setShortcut(QCoreApplication::translate("emuone::MainFrame", "F5", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStop->setText(QCoreApplication::translate("emuone::MainFrame", "&Stop", nullptr));
#if QT_CONFIG(shortcut)
        actionStop->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Esc, Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSuspend->setText(QCoreApplication::translate("emuone::MainFrame", "S&uspend", nullptr));
#if QT_CONFIG(shortcut)
        actionSuspend->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionResume->setText(QCoreApplication::translate("emuone::MainFrame", "R&esume", nullptr));
#if QT_CONFIG(shortcut)
        actionResume->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionReset->setText(QCoreApplication::translate("emuone::MainFrame", "&Reset", nullptr));
#if QT_CONFIG(shortcut)
        actionReset->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+Alt+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionConfigure->setText(QCoreApplication::translate("emuone::MainFrame", "&Configure", nullptr));
#if QT_CONFIG(shortcut)
        actionConfigure->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFullScreen->setText(QCoreApplication::translate("emuone::MainFrame", "&Full screen", nullptr));
#if QT_CONFIG(shortcut)
        actionFullScreen->setShortcut(QCoreApplication::translate("emuone::MainFrame", "Ctrl+Return", nullptr));
#endif // QT_CONFIG(shortcut)
        tabWidget->setTabText(tabWidget->indexOf(homeTab), QCoreApplication::translate("emuone::MainFrame", "Home", nullptr));
        menuFile->setTitle(QCoreApplication::translate("emuone::MainFrame", "&File", nullptr));
        menuVm->setTitle(QCoreApplication::translate("emuone::MainFrame", "&VM", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("emuone::MainFrame", "&Help", nullptr));
        menuTools->setTitle(QCoreApplication::translate("emuone::MainFrame", "&Tools", nullptr));
    } // retranslateUi

};

} // namespace emuone

namespace emuone {
namespace Ui {
    class MainFrame: public Ui_MainFrame {};
} // namespace Ui
} // namespace emuone

#endif // UI_MAINFRAME_H
