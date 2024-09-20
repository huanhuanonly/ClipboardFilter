#include "maindialog.h"
#include "ui_maindialog.h"

#include <QDir>
#include <QSettings>
#include <QStandardItemModel>

#include <QSignalBlocker>

#include <QDebug>

#include "TextReplaceRuleListView.h"

#if !defined STR
#define STR(str) #str
#endif

#if !defined STRVALUE
#define STRVALUE(str) STR(str)
#endif

MainDialog::MainDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , cb(QGuiApplication::clipboard())
    , _M_replacer(new huanhuan::TextReplacer())
{
    ui->setupUi(this);
    
    setWindowTitle(tr("The Clipboard Filter %0, Hello @%1").arg(STRVALUE(__VERSION__)).arg(QDir::home().dirName()));
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    
    // Create title bar
    _M_ui_titlebar = new TitleBar(this);
    
    // Set the font of the `Copyright`
    QFont itFont(ui->labelCopyright->font());
    itFont.setItalic(true);
    itFont.setPointSize(6);
    ui->labelCopyright->setFont(itFont);
    
    // If the connection type is not set to Qt::QueuedConnection,
    // then dataChanged() will be sent one additional time and
    // the clipboard will be set to empty.
    connect(cb, &QClipboard::dataChanged, this, &MainDialog::on_clipboardChange, Qt::QueuedConnection);
    
    // Configure logBrowser
    ui->logBrowser->document()->setMaximumBlockCount(520);
    
    ui->logBrowser->setOpenExternalLinks(true);
    
    ui->logBrowser->append(QString(R"-(<style type="text/css">a{color:#66FFFF;}.name{text-decoration:none;color:#FF8000}</style> <p style="color: rgb(255,255,0)"><b>%0</b> <i>%1</i></p>)-")
            .arg(QDateTime::currentDateTime().toString("[t MMM dd hh:mm:ss:zzz]").toHtmlEscaped())
            .arg(tr(R"-(Hello @%0, Welcome to <a href="https://github.com/huanhuanonly/ClipboardFilter">ClipboardFilter</a>(%1). By <a href="https://codeforces.com/profile/huanhuanonly"><span class="name">YangHuanhuan</span></a> (3347484963@qq.com) in December 2023 for <span class="name">YuFeixia</span>. All rights reserved. Last update in September 2024.)-")
                .arg(QDir::home().dirName().toHtmlEscaped())
                .arg(STRVALUE(__VERSION__)))
        );
    
    connect(&huanhuan::slog, &huanhuan::Logger::itemAdded, this, [this, cnt = 0](const huanhuan::Logger::Item& item) mutable -> void
    {
        ui->logBrowser->append(QString(R"-(<p%0><i><b>%1 [%2]</b></i> %3</p>)-")
                            .arg(cnt % 2
                                ? R"-( style="color: rgb(255,51,255)")-"
                                : R"-( style="color: rgb(51,255,255)")-")
                            .arg(item.index)
                            .arg(item.time.toString("MMM dd hh:mm:ss:zzz"))
                            .arg(item.text.toHtmlEscaped()));
        QTextCursor tc = ui->logBrowser->textCursor();
        tc.movePosition(QTextCursor::End);
        
        ui->logBrowser->setTextCursor(tc);
        ++cnt;
    });
    
    // Load last configuration files if it's exist
    if (QFile::exists(QDir::tempPath() + R"(/ClipboardFilter.ini)"))
    {
        QSettings ini(QDir::tempPath() + R"(/ClipboardFilter.ini)", QSettings::Format::IniFormat);
        ui->characterListBox->setText(
                    ini.value("CharactersSet", ui->characterListBox->text()).toString());
        ui->enableVariablesCheckBox->setChecked(
                    ini.value("EnableVariables", ui->enableVariablesCheckBox->isChecked()).toBool());
        
        if (ui->enableVariablesCheckBox->isChecked() == false)
        {
            ui->parseButton->setEnabled(false);
        }
        
        QList<TextReplaceRuleListView::DataType> dataList;
        int size = ini.value("ReplaceRules/ItmeConut", 0).toInt();
        for (int i = 0; i < size; ++i)
        {
            dataList.push_back(
            {
                ini.value(QString("ReplaceRules/Item_%0/Pattern").arg(i), QString()),
                ini.value(QString("ReplaceRules/Item_%0/PatternEnableVariables").arg(i), false),
                ini.value(QString("ReplaceRules/Item_%0/Replacement").arg(i), QString()),
                ini.value(QString("ReplaceRules/Item_%0/ReplacementEnableVariables").arg(i), false),
                ini.value(QString("ReplaceRules/Item_%0/MatchOptions").arg(i), 0)
            });
        }
        _M_ui_replacementListView = new TextReplaceRuleListView(_M_replacer, dataList, this);
    }
    else
    {
        _M_ui_replacementListView = new TextReplaceRuleListView(_M_replacer, QList<TextReplaceRuleListView::DataType>(), this);
    }
    
    ui->groupBox_replaceList->layout()->addWidget(_M_ui_replacementListView);
    
    connect(ui->labelStatusBar, &StatusBarLabel::moreOpened, this, [this]() -> void
    {
        ui->groupBox_logger->show();
    });
    connect(ui->labelStatusBar, &StatusBarLabel::moreClosed, this, [this]() -> void
    {
        ui->groupBox_logger->hide();
    });
    
    ui->groupBox_logger->hide();
//    ui->labelStatusBar->setState(StatusBarLabel::Status::Waiting);
    
    // Update on startup
    ui->updataButton->click();
}

MainDialog::~MainDialog()
{
    // Save the current settings to configuration files
    
    QFile::remove(QDir::tempPath() + R"(/ClipboardFilter.ini)");
    
    QSettings ini(QDir::tempPath() + R"(/ClipboardFilter.ini)", QSettings::Format::IniFormat);
    ini.setValue("CharactersSet", ui->characterListBox->text());
    ini.setValue("EnableVariables", ui->enableVariablesCheckBox->isChecked());
    
    auto dataList = _M_ui_replacementListView->model()->dataList();
    ini.setValue("ReplaceRules/ItmeConut", dataList.size());
    for (int i = 0; i < dataList.size(); ++i)
    {
        ini.setValue(QString("ReplaceRules/Item_%0/Pattern").arg(i), dataList[i].pattern);
        ini.setValue(QString("ReplaceRules/Item_%0/Replacement").arg(i), dataList[i].replacement);
        ini.setValue(QString("ReplaceRules/Item_%0/PatternEnableVariables").arg(i), dataList[i].patternEnableVariables);
        ini.setValue(QString("ReplaceRules/Item_%0/ReplacementEnableVariables").arg(i), dataList[i].replacementEnableVariables);
        ini.setValue(QString("ReplaceRules/Item_%0/MatchOptions").arg(i), static_cast<int>(dataList[i].matchOptions));
    }
    
    delete ui;
}

void MainDialog::updateClipboard()
{
    if (_M_clipboard_flag == true)
    {
        _M_clipboard_flag = false;
        return;
    }
    
    QString text = cb->text();
    if (text.isEmpty())
    {
        return;
    }
    
    _M_clipboard_flag = true;
    
    ui->labelStatusBar->setState(StatusBarLabel::Status::Replacing);
    
    huanhuan::TextReplacer::CharacterSet cs;
    
    QString str = ui->characterListBox->text();
    if (ui->enableVariablesCheckBox->isChecked())
    {
        auto error_func = [](QStringView variable, const char* what) -> void
        {
            huanhuan::slog << tr("From variable ${ %0 }: %1").arg(variable).arg(what);
        };
        
        try
        {
            str = _M_replacer->variableParser().parse(str, error_func);
        }
        catch (const std::exception& e)
        {
            huanhuan::slog << tr("Variable parsing failed, In CharactersSet: %0").arg(e.what());
        }
    }
    
    for (const QChar& i : str)
    {
        cs.insert(i);
    }
    
    text = _M_replacer->filter(text, cs);
    cb->setText(_M_ui_replacementListView->filter(text));
    
    ui->labelStatusBar->setState(StatusBarLabel::Status::Replaced);
}

void MainDialog::on_clipboardChange()
{
    if (_M_clipboard_flag == true)
    {
        _M_clipboard_flag = false;
        return;
    }
    
    if (cb->text().isEmpty())
    {
        return;
    }
    
    ui->labelStatusBar->setState(StatusBarLabel::Status::Copied);
    
    updateClipboard();
}

void MainDialog::on_stateButton_clicked()
{
    // Stop
    if (state == RunState::Running)
    {
        ui->stateButton->setText(tr("&Start"));
        disconnect(cb, &QClipboard::dataChanged, this, &MainDialog::on_clipboardChange);
        state = RunState::Stopping;
    }
    // Start
    else
    {
        ui->stateButton->setText(tr("&Stop"));
        connect(cb, &QClipboard::dataChanged, this, &MainDialog::on_clipboardChange, Qt::QueuedConnection);
        state = RunState::Running;
    }
    return;
}

void MainDialog::on_clearButton_clicked()
{
    ui->characterListBox->clear();
    int lineCount = _M_ui_replacementListView->model()->rowCount();
    for (int i = 0; i < lineCount; ++i)
    {
        _M_ui_replacementListView->removeRow(0);
    }
}


void MainDialog::on_insertButton_clicked()
{
    QModelIndex index = _M_ui_replacementListView->currentIndex();
    
    _M_ui_replacementListView->insertRow(index.isValid() ? index.row() + 1 : 0);
    _M_ui_replacementListView->selectRow((index.isValid() ? index.row() : 0) + 1);
    _M_ui_replacementListView->edit(_M_ui_replacementListView->currentIndex());
}


void MainDialog::on_removeButton_clicked()
{
    _M_ui_replacementListView->removeRow(_M_ui_replacementListView->currentIndex().row());
}


void MainDialog::on_updataButton_clicked()
{
    updateClipboard();
}


void MainDialog::on_enableVariablesCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        ui->characterListBox->enableVariables();
        ui->parseButton->setEnabled(true);
    }
    else
    {
        ui->characterListBox->disableVariables();
        ui->parseButton->setEnabled(false);
    }
}


void MainDialog::on_parseButton_clicked()
{
    if (ui->enableVariablesCheckBox->isChecked())
    {
        ui->enableVariablesCheckBox->setChecked(false);
        
        QString text = ui->characterListBox->text();
        
        try
        {
            text = _M_replacer->variableParser().parse(text, [](QStringView variable, const char* what) -> void
            {
                huanhuan::slog << tr("From variable ${ %0 }: %1").arg(variable).arg(what);
            });
        }
        catch (const std::exception& e)
        {
            huanhuan::slog << tr("Variable parsing failed, In CharactersSet: %0").arg(e.what());
        }
        
        ui->characterListBox->setText(text);
    }
}

