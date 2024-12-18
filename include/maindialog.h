/**
 * @file MainDialog.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Main UI
 * 
 * @ingroup huanhuan::ui
 * 
 * @include
 *     @class MainDialog
 */

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QClipboard>
#include <QApplication>
#include <QStandardItemModel>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QSet>

#include "TextReplaceRuleListView.h"
#include "Titlebar.h"

#include "TextReplacer.h"

#include "Logger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT
    
public:
    
    MainDialog(QWidget* parent = nullptr);
    ~MainDialog();
    
    void loadConfigure();
    void saveConfigure() const;
    
    void updateClipboard();
    
    void initTray();
    void initLogBrowser();
    
    static void variableParsingFailed(QStringView variable, const char* what)
    {
        huanhuan::slog << tr("Failed from variable ${ %0 }: %1").arg(variable).arg(what)
                       << huanhuan::endl;
    }
    
    friend class MainFloatingWindow;
    
signals:
    
    void runningChanged(bool is_running);
    
private slots:
    
    void onClipboardDataChanged();
    
    void on_stateButton_clicked();
    void on_resetButton_clicked();
    void on_insertButton_clicked();
    void on_removeButton_clicked();
    void on_updataButton_clicked();
    void on_enableVariablesCheckBox_stateChanged(int);
    void on_parseButton_clicked();
    void on_clearButton_clicked();
    
protected:
    
    Ui::MainDialog* ui;
    
    QClipboard* cb;
    QSystemTrayIcon* _M_tray;
    
    enum class RunState: int { Running, Stopping } state = RunState::Running;
    
    TextReplaceRuleListView* _M_ui_replacementListView;
    TitleBar* _M_ui_titlebar;
    
    std::shared_ptr<huanhuan::TextReplacer> _M_replacer;
    
    bool _M_clipboard_flag = false;
};
#endif // MAINDIALOG_H
