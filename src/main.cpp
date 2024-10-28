/**
 * @file VariableParser.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @implements main() function
 * 
 * @ingroup huanhuan::main
 */

#include "maindialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

#include <QMessageBox>

#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "ClipboardFilter_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    
    int fontId = QFontDatabase::addApplicationFont(":/Font/CascadiaCode.ttf");
    QApplication::setFont(QFont(QFontDatabase::applicationFontFamilies(fontId).at(0), 8));
    
    a.setStyleSheet(
R"(*
{
	color: rgb(229, 229, 229);
	background-color: rgb(30, 30, 30);
})");
    
    try
    {
        MainDialog w;
        w.show();
        return a.exec();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(
            nullptr,
            "ClipboardFilter Error",
            QObject::tr(R"(
Error: Throwing an exception in main()
What: %0
)").arg(e.what())
        );
        
        return EXIT_FAILURE;
    }
    
    return 0;
}
