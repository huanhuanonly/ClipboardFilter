#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QClipboard>
#include <QApplication>
#include <QStandardItemModel>
#include <QSet>

#include "TextReplaceRuleListView.h"
#include "Titlebar.h"

#include "TextReplacer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT
    
public:
    MainDialog(QWidget* parent = nullptr);
    ~MainDialog();
    
    void updateClipboard();
    
private slots:
    void on_clipboardChange();
    
    void on_stateButton_clicked();
    
    void on_clearButton_clicked();
    
    void on_insertButton_clicked();
    
    void on_removeButton_clicked();
    
    void on_updataButton_clicked();
    
    void on_enableVariablesCheckBox_stateChanged(int);
    
    void on_parseButton_clicked();
    
private:
    
    Ui::MainDialog* ui;
    
    QClipboard* cb;
    
    enum class RunState: int { Running, Stopping } state = RunState::Running;
    
    TextReplaceRuleListView* _M_ui_replacementListView;
    TitleBar* _M_ui_titlebar;
    
    std::shared_ptr<huanhuan::TextReplacer> _M_replacer;
    
    bool _M_clipboard_flag = false;
};
#endif // MAINDIALOG_H
