/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_characterList;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *characterListBox;
    QWidget *widget_escapeCharacter;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QCheckBox *escapeCharactersCheckBox;
    QGroupBox *groupBox_replaceList;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget_InsertAndRemove;
    QHBoxLayout *horizontalLayout;
    QLabel *labelCopyright;
    QPushButton *insertButton;
    QPushButton *removeButton;
    QWidget *widget_controlButton;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *clearButton;
    QPushButton *stateButton;
    QPushButton *updataButton;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QString::fromUtf8("MainDialog"));
        MainDialog->resize(554, 356);
        MainDialog->setStyleSheet(QString::fromUtf8("*\n"
"{\n"
"	color: rgb(229, 229, 229);\n"
"	background-color: rgb(30, 30, 30);\n"
"}\n"
"\n"
"QToolTip\n"
"{\n"
"	border: 1px solid rgb(34, 255, 229);\n"
"	border-radius: 5px;\n"
"	color: rgb(229, 229, 229);\n"
"	background-color: rgb(30, 30, 30);\n"
"}\n"
"\n"
"QPushButton\n"
"{\n"
"	border: 1px solid rgb(172, 172, 172);\n"
"	border-radius: 12px;\n"
"	padding: 5px 5px 5px 5px;\n"
"	background-color: rgb(51, 51, 51);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	border: 1px solid rgb(0, 168, 168);\n"
"	color: rgb(0, 255, 0);\n"
"	background-color: rgb(65, 97, 97);\n"
"}\n"
"QPushButton:focus\n"
"{\n"
"	border: 1px solid rgb(0, 255, 255);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"	border: 1px inset rgb(0, 255, 255);\n"
"	color: rgb(0, 255, 0);\n"
"	background-color: rgb(0, 121, 181);\n"
"}\n"
"\n"
"QCheckBox:indicator\n"
"{\n"
"	border: 1px solid rgb(255, 255, 255);\n"
"	border-radius: 6px;\n"
"}\n"
"QCheckBox:indicator:hover, QCheckBox:indicator:focus\n"
"{\n"
"	border-color: rgb(0, 255, 255);\n"
"}\n"
"QCheckBox:indi"
                        "cator:checked\n"
"{\n"
"	background-color: rgb(0, 255, 255);\n"
"}\n"
"\n"
"QTableView\n"
"{\n"
"	alternate-background-color: rgb(60, 90, 90);\n"
"	gridline-color: rgb(77, 116, 116);\n"
"	border: 1px solid rgb(170, 255, 255);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"QHeaderView::section\n"
"{\n"
"	border: 1px dished rgb(85, 255, 255);\n"
"    color: rgb(255, 255, 255);\n"
"    background-color: rgb(0, 55, 56);\n"
"    text-align: center;\n"
"}\n"
"QTableView::item\n"
"{\n"
"    selection-background-color: rgb(30, 60, 90);\n"
"}\n"
"\n"
"QSlider::handle\n"
"{\n"
"	border: 1px solid rgb(0, 255, 127);\n"
"	border-radius: 5px;\n"
"	background-color: rgba(85, 255, 255, 165);\n"
"}\n"
"QSlider::handle:horizontal:hover\n"
"{\n"
"	border: 1px solid rgb(0, 255, 127);\n"
"	background-color: rgb(85, 255, 255);\n"
"}\n"
"QSlider::sub-page\n"
"{\n"
"	background: rgb(235,97,0);\n"
"	border-radius: 0px;\n"
"	margin-top:8px;\n"
"	margin-bottom:8px;\n"
"}"));
        verticalLayout = new QVBoxLayout(MainDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(11, 0, 11, 0);
        groupBox_characterList = new QGroupBox(MainDialog);
        groupBox_characterList->setObjectName(QString::fromUtf8("groupBox_characterList"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_characterList->sizePolicy().hasHeightForWidth());
        groupBox_characterList->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox_characterList);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 5, 0, 0);
        characterListBox = new QLineEdit(groupBox_characterList);
        characterListBox->setObjectName(QString::fromUtf8("characterListBox"));
        characterListBox->setStyleSheet(QString::fromUtf8("QLineEdit\n"
"{\n"
"	border: 1px solid rgb(106, 106, 106);\n"
"	border-radius: 5px;\n"
"	background-color: rgb(63, 63, 63);\n"
"	selection-color: rgb(255, 0, 0);\n"
"	selection-background-color: rgb(85, 255, 255);\n"
"}\n"
"QLineEdit:hover\n"
"{\n"
"	border: 1px solid rgb(14, 213, 0);\n"
"	background-color: rgb(53, 53, 53);\n"
"}\n"
"QLineEdit:focus\n"
"{\n"
"	border: 2px solid rgb(14, 213, 0);\n"
"}"));
        characterListBox->setFrame(true);
        characterListBox->setClearButtonEnabled(true);

        verticalLayout_2->addWidget(characterListBox);

        widget_escapeCharacter = new QWidget(groupBox_characterList);
        widget_escapeCharacter->setObjectName(QString::fromUtf8("widget_escapeCharacter"));
        horizontalLayout_5 = new QHBoxLayout(widget_escapeCharacter);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(149, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        escapeCharactersCheckBox = new QCheckBox(widget_escapeCharacter);
        escapeCharactersCheckBox->setObjectName(QString::fromUtf8("escapeCharactersCheckBox"));

        horizontalLayout_5->addWidget(escapeCharactersCheckBox);


        verticalLayout_2->addWidget(widget_escapeCharacter);


        verticalLayout->addWidget(groupBox_characterList);

        groupBox_replaceList = new QGroupBox(MainDialog);
        groupBox_replaceList->setObjectName(QString::fromUtf8("groupBox_replaceList"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_replaceList);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, 0, 0);

        verticalLayout->addWidget(groupBox_replaceList);

        widget_InsertAndRemove = new QWidget(MainDialog);
        widget_InsertAndRemove->setObjectName(QString::fromUtf8("widget_InsertAndRemove"));
        sizePolicy.setHeightForWidth(widget_InsertAndRemove->sizePolicy().hasHeightForWidth());
        widget_InsertAndRemove->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget_InsertAndRemove);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelCopyright = new QLabel(widget_InsertAndRemove);
        labelCopyright->setObjectName(QString::fromUtf8("labelCopyright"));
        labelCopyright->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(labelCopyright);

        insertButton = new QPushButton(widget_InsertAndRemove);
        insertButton->setObjectName(QString::fromUtf8("insertButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(insertButton->sizePolicy().hasHeightForWidth());
        insertButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(insertButton);

        removeButton = new QPushButton(widget_InsertAndRemove);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        sizePolicy1.setHeightForWidth(removeButton->sizePolicy().hasHeightForWidth());
        removeButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(removeButton);


        verticalLayout->addWidget(widget_InsertAndRemove);

        widget_controlButton = new QWidget(MainDialog);
        widget_controlButton->setObjectName(QString::fromUtf8("widget_controlButton"));
        sizePolicy.setHeightForWidth(widget_controlButton->sizePolicy().hasHeightForWidth());
        widget_controlButton->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(widget_controlButton);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, -1);
        clearButton = new QPushButton(widget_controlButton);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        sizePolicy1.setHeightForWidth(clearButton->sizePolicy().hasHeightForWidth());
        clearButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(clearButton);

        stateButton = new QPushButton(widget_controlButton);
        stateButton->setObjectName(QString::fromUtf8("stateButton"));

        horizontalLayout_4->addWidget(stateButton);

        updataButton = new QPushButton(widget_controlButton);
        updataButton->setObjectName(QString::fromUtf8("updataButton"));
        sizePolicy1.setHeightForWidth(updataButton->sizePolicy().hasHeightForWidth());
        updataButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(updataButton);


        verticalLayout->addWidget(widget_controlButton);


        retranslateUi(MainDialog);

        stateButton->setDefault(true);


        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        groupBox_characterList->setTitle(QCoreApplication::translate("MainDialog", "CharacterList", nullptr));
#if QT_CONFIG(tooltip)
        characterListBox->setToolTip(QCoreApplication::translate("MainDialog", "<html><head/><body><p>These characters will be <span style=\" font-weight:700;\">removed</span> from the <span style=\" font-style:italic;\">clipboard</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        escapeCharactersCheckBox->setToolTip(QCoreApplication::translate("MainDialog", "This escape character is '\\'", nullptr));
#endif // QT_CONFIG(tooltip)
        escapeCharactersCheckBox->setText(QCoreApplication::translate("MainDialog", "&Use escape characters in C language", nullptr));
        groupBox_replaceList->setTitle(QCoreApplication::translate("MainDialog", "ReplaceList", nullptr));
#if QT_CONFIG(tooltip)
        labelCopyright->setToolTip(QCoreApplication::translate("MainDialog", "<html><head/><body><p><span style=\" font-weight:700;\">The ClipboardFilter</span>, Written by <span style=\" font-style:italic;\">YangHuanhuan</span></p><p>in <span style=\" text-decoration: underline;\">December 2023</span> for <span style=\" font-style:italic;\">YuFeixia</span>. All rights reserved.</p><p>---</p><p>E-mail: <span style=\" font-weight:700;\">3347484963@qq.com</span></p><p>Github: <a href=\"https://github.com/huanhuanonly\"><span style=\" font-weight:700; text-decoration: underline; color:#8ab4f8;\">https://github.com/huanhuanonly</span></a></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        labelCopyright->setText(QCoreApplication::translate("MainDialog", "<html><head/><body><p><span style=\" font-size:6pt; font-weight:700;\">The ClipboardFilter</span><span style=\" font-size:6pt;\">, Written by </span><span style=\" font-size:6pt;\">YangHuanhuan</span></p><p><span style=\" font-size:6pt;\">in December 2023 for </span><span style=\" font-size:6pt;\">YuFeixia</span><span style=\" font-size:6pt;\">. All rights reserved.</span></p></body></html>", nullptr));
#if QT_CONFIG(tooltip)
        insertButton->setToolTip(QCoreApplication::translate("MainDialog", "<html><head/><body><p><span style=\" font-weight:700;\">Insert</span> a new <span style=\" text-decoration: underline;\">row</span> below the selection line in <span style=\" font-style:italic;\">ReplaceList</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        insertButton->setText(QCoreApplication::translate("MainDialog", "&Insert", nullptr));
#if QT_CONFIG(tooltip)
        removeButton->setToolTip(QCoreApplication::translate("MainDialog", "<html><head/><body><p><span style=\" font-weight:700;\">Remove</span> the current selection line in <span style=\" font-style:italic;\">ReplaceList</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        removeButton->setText(QCoreApplication::translate("MainDialog", "Remove", nullptr));
#if QT_CONFIG(tooltip)
        clearButton->setToolTip(QCoreApplication::translate("MainDialog", "<html><head/><body><p><span style=\" font-weight:700;\">Clear</span> the text of <span style=\" font-style:italic;\">CharacterList</span> and the all row <span style=\" font-style:italic;\">ReplaceList</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        clearButton->setText(QCoreApplication::translate("MainDialog", "&Clear", nullptr));
#if QT_CONFIG(tooltip)
        stateButton->setToolTip(QCoreApplication::translate("MainDialog", "<html><head/><body><p>This switch to <span style=\" font-weight:700;\">run</span> or <span style=\" font-weight:700;\">stop</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        stateButton->setText(QCoreApplication::translate("MainDialog", "&Stop", nullptr));
        updataButton->setText(QCoreApplication::translate("MainDialog", "Updata", nullptr));
        (void)MainDialog;
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
