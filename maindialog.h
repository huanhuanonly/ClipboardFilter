#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QClipboard>
#include <QApplication>
#include <QSet>
#include <QStandardItemModel>

#include "StringReplaceListView.h"
#include "Titlebar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT
    
public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();
    
    QString filter(const QString& text) const noexcept;
    
private slots:
    void on_clipboardChange();
    void on_stateButton_clicked();
    
    void on_characterListBox_textChanged(const QString &arg1);
    
    void on_clearButton_clicked();
    
    void on_insertButton_clicked();
    
    void on_removeButton_clicked();
    
    void on_updataButton_clicked();
    
private:
    Ui::MainDialog *ui;
    
    QClipboard* cb;
    
    enum class State: int { Running, Stopping } state = State::Running;
    
    QSet<QChar> set_char { };
    StringReplaceListView* replaceListBox;
    TitleBar* titlebar;
};
#endif // MAINDIALOG_H
