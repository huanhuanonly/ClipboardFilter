#include "maindialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ClipboardFilter_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    int fontId = QFontDatabase::addApplicationFont(":/CascadiaCode.ttf");
    QApplication::setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).at(0), 8));
    
    a.setStyleSheet(
R"(*
{
	color: rgb(229, 229, 229);
	background-color: rgb(30, 30, 30);
})");
    
    MainDialog w;
    w.show();
    return a.exec();
}
