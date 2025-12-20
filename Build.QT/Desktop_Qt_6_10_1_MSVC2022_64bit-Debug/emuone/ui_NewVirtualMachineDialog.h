/********************************************************************************
** Form generated from reading UI file 'NewVirtualMachineDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWVIRTUALMACHINEDIALOG_H
#define UI_NEWVIRTUALMACHINEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

namespace emuone {

class Ui_NewVirtualMachineDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *architectureLabel;
    QComboBox *architectureComboBox;
    QLabel *stereotypeLabel;
    QComboBox *stereotypeComboBox;
    QLabel *templateLabel;
    QComboBox *templateComboBox;
    QLabel *locationLabel;
    QLineEdit *locationLineEdit;
    QPushButton *browsePushButton;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *emuone__NewVirtualMachineDialog)
    {
        if (emuone__NewVirtualMachineDialog->objectName().isEmpty())
            emuone__NewVirtualMachineDialog->setObjectName("emuone__NewVirtualMachineDialog");
        emuone__NewVirtualMachineDialog->resize(407, 236);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/NewLarge.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        emuone__NewVirtualMachineDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(emuone__NewVirtualMachineDialog);
        gridLayout->setObjectName("gridLayout");
        nameLabel = new QLabel(emuone__NewVirtualMachineDialog);
        nameLabel->setObjectName("nameLabel");

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        nameLineEdit = new QLineEdit(emuone__NewVirtualMachineDialog);
        nameLineEdit->setObjectName("nameLineEdit");

        gridLayout->addWidget(nameLineEdit, 0, 1, 1, 2);

        architectureLabel = new QLabel(emuone__NewVirtualMachineDialog);
        architectureLabel->setObjectName("architectureLabel");

        gridLayout->addWidget(architectureLabel, 1, 0, 1, 1);

        architectureComboBox = new QComboBox(emuone__NewVirtualMachineDialog);
        architectureComboBox->setObjectName("architectureComboBox");

        gridLayout->addWidget(architectureComboBox, 1, 1, 1, 2);

        stereotypeLabel = new QLabel(emuone__NewVirtualMachineDialog);
        stereotypeLabel->setObjectName("stereotypeLabel");

        gridLayout->addWidget(stereotypeLabel, 2, 0, 1, 1);

        stereotypeComboBox = new QComboBox(emuone__NewVirtualMachineDialog);
        stereotypeComboBox->setObjectName("stereotypeComboBox");

        gridLayout->addWidget(stereotypeComboBox, 2, 1, 1, 2);

        templateLabel = new QLabel(emuone__NewVirtualMachineDialog);
        templateLabel->setObjectName("templateLabel");

        gridLayout->addWidget(templateLabel, 3, 0, 1, 1);

        templateComboBox = new QComboBox(emuone__NewVirtualMachineDialog);
        templateComboBox->setObjectName("templateComboBox");

        gridLayout->addWidget(templateComboBox, 3, 1, 1, 2);

        locationLabel = new QLabel(emuone__NewVirtualMachineDialog);
        locationLabel->setObjectName("locationLabel");

        gridLayout->addWidget(locationLabel, 4, 0, 1, 1);

        locationLineEdit = new QLineEdit(emuone__NewVirtualMachineDialog);
        locationLineEdit->setObjectName("locationLineEdit");
        locationLineEdit->setMinimumSize(QSize(200, 0));
        locationLineEdit->setReadOnly(true);

        gridLayout->addWidget(locationLineEdit, 4, 1, 1, 1);

        browsePushButton = new QPushButton(emuone__NewVirtualMachineDialog);
        browsePushButton->setObjectName("browsePushButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/emuone/Resources/Images/Actions/BrowseSmall.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        browsePushButton->setIcon(icon1);

        gridLayout->addWidget(browsePushButton, 4, 2, 1, 1);

        line = new QFrame(emuone__NewVirtualMachineDialog);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 5, 0, 1, 3);

        buttonBox = new QDialogButtonBox(emuone__NewVirtualMachineDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 6, 0, 1, 3);


        retranslateUi(emuone__NewVirtualMachineDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, emuone__NewVirtualMachineDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, emuone__NewVirtualMachineDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(emuone__NewVirtualMachineDialog);
    } // setupUi

    void retranslateUi(QDialog *emuone__NewVirtualMachineDialog)
    {
        emuone__NewVirtualMachineDialog->setWindowTitle(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "New virtual machine", nullptr));
        nameLabel->setText(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "Name:", nullptr));
        architectureLabel->setText(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "Architecture:", nullptr));
        stereotypeLabel->setText(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "Stereotype:", nullptr));
        templateLabel->setText(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "Template:", nullptr));
        locationLabel->setText(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "Location:", nullptr));
        browsePushButton->setText(QCoreApplication::translate("emuone::NewVirtualMachineDialog", "Browse", nullptr));
    } // retranslateUi

};

} // namespace emuone

namespace emuone {
namespace Ui {
    class NewVirtualMachineDialog: public Ui_NewVirtualMachineDialog {};
} // namespace Ui
} // namespace emuone

#endif // UI_NEWVIRTUALMACHINEDIALOG_H
