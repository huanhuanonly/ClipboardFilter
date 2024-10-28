/**
 * @file MainDialog.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Main UI
 * 
 * @ingroup huanhuan::ui
 */

#include "MainDialog.h"
#include "ui_maindialog.h"

#include <QDir>
#include <QSettings>
#include <QStandardItemModel>

#include <QSignalBlocker>

#include <QDebug>

#include "MainFloatingWindow.h"
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
    , _M_tray(new QSystemTrayIcon(QIcon(":/Icon/MainIcon.ico"), this))
    , _M_replacer(new huanhuan::TextReplacer())
{
    ui->setupUi(this);
    
    // Set windows flags
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    
    // Create title bar
    _M_ui_titlebar = new TitleBar(
                this,
                tr("ClipboardFilter %0, @%1")
                    .arg(STRVALUE(__VERSION__))
                    .arg(QDir::home().dirName()),
                new MainFloatingWindow(this));
    
    connect(qobject_cast<MainFloatingWindow*>(_M_ui_titlebar->floatingWindow()), &MainFloatingWindow::iconClicked, _M_ui_titlebar, &TitleBar::switchToMainWindow);
    
    // Set the font of the `Copyright`
    QFont itFont(ui->labelCopyright->font());
    itFont.setItalic(true);
    itFont.setPointSize(6);
    ui->labelCopyright->setFont(itFont);
    
    // If the connection type is not set to Qt::QueuedConnection,
    // then dataChanged() will be sent one additional time and
    // the clipboard will be set to empty.
    connect(cb, &QClipboard::dataChanged, this, &MainDialog::onClipboardDataChanged, Qt::QueuedConnection);
    
    huanhuan::VariableParser::global()->setOnFailure(&MainDialog::variableParsingFailed);
    
    // Configure to taskbar tray
    initTray();
    
    // Configure logBrowser
    initLogBrowser();
    
    // Load last configuration files if it's exist
    loadConfigure();
    
    // Update on startup
    if constexpr (SETTINGS__UPDATE_ON_STARTUP)
    {
        ui->updataButton->click();
    }
    else
    {
        ui->labelStatusBar->setState(StatusBarLabel::Status::Waiting);
    }
}

MainDialog::~MainDialog()
{
    // Save the current settings to configuration files
    saveConfigure();
    
    delete ui;
}

void MainDialog::updateClipboard()
{
    if (_M_clipboard_flag == true)
    {
        _M_clipboard_flag = false;
        return;
    }
    
    _M_replacer->buffer() = cb->text();
    if (_M_replacer->buffer().isEmpty())
    {
        return;
    }
    
    _M_clipboard_flag = true;
    
    ui->labelStatusBar->setState(StatusBarLabel::Status::Replacing);
    
    bool before = ui->beforeButton->isChecked();
    bool after  = ui->afterButton->isChecked();
    
    huanhuan::TextReplacer::CharacterSet cs;
    
    if (before || after)
    {
        QString csText = ui->characterListBox->text();
        if (ui->enableVariablesCheckBox->isChecked())
        {
            try
            {
                csText = _M_replacer->variableParser()->parse(csText);
            }
            catch (const std::exception& e)
            {
                huanhuan::slog << tr("Variable parsing failed, In CharactersSet: %0").arg(e.what());
            }
        }
        
        for (const QChar& i : csText)
        {
            cs.insert(i);
        }
    }
    
    if (before)
    {
        _M_replacer->buffer() = _M_replacer->filter(_M_replacer->buffer(), cs);
    }
    
    _M_ui_replacementListView->filter(_M_replacer->buffer());
    
    if (after)
    {
        _M_replacer->buffer() = _M_replacer->filter(_M_replacer->buffer(), cs);
    }
    
    cb->setText(_M_replacer->buffer());
    
    ui->labelStatusBar->setState(StatusBarLabel::Status::Replaced);
}

void MainDialog::loadConfigure()
{
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
        
        ui->beforeButton->setChecked(
                    ini.value("Before", ui->beforeButton->isChecked()).toBool());
        
        ui->afterButton->setChecked(
                    ini.value("After", ui->afterButton->isChecked()).toBool());
        
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
}

void MainDialog::saveConfigure() const
{
    QFile::remove(QDir::tempPath() + R"(/ClipboardFilter.ini)");
    
    QSettings ini(QDir::tempPath() + R"(/ClipboardFilter.ini)", QSettings::Format::IniFormat);
    ini.setValue("CharactersSet", ui->characterListBox->text());
    ini.setValue("EnableVariables", ui->enableVariablesCheckBox->isChecked());
    
    ini.setValue("Before", ui->beforeButton->isChecked());
    ini.setValue("After", ui->afterButton->isChecked());
    
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
}

void MainDialog::initTray()
{
    QMenu* menu = new QMenu(this);
    
    QAction* action;
    
    action = new QAction(tr("Version ") + STRVALUE(__VERSION__), this);
    action->setEnabled(false);
    menu->addAction(action);
    
    action = new QAction(tr("By huanhuanonly"), this);
    connect(action, &QAction::triggered, this, []() -> void
    {
        QDesktopServices::openUrl(QUrl("https://github.com/huanhuanonly"));
    });
    menu->addAction(action);
    
    menu->addSeparator();
    
    action = new QAction(tr("Github"), this);
    connect(action, &QAction::triggered, this, []() -> void
    {
        QDesktopServices::openUrl(QUrl("https://github.com/huanhuanonly/ClipboardFilter"));
    });
    menu->addAction(action);
    
    action = new QAction(tr("View Configuration file"), this);
    connect(action, &QAction::triggered, this, []() -> void
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::tempPath() + R"(/ClipboardFilter.ini)"));
    });
    menu->addAction(action);
    
    action = new QAction(tr("Exit"), this);
    connect(action, &QAction::triggered, this, []() -> void
    {
        QApplication::exit(0);
    });
    menu->addAction(action);
    
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowFlag(Qt::FramelessWindowHint, true);
    
    _M_tray->setContextMenu(menu);
    _M_tray->show();
}

void MainDialog::initLogBrowser()
{
    ui->logBrowser->document()->setMaximumBlockCount(huanhuan::slog.maxSize());
    
    ui->logBrowser->setOpenExternalLinks(true);
    
    ui->logBrowser->append(QString(R"-(<style type="text/css">a{color:#66FFFF;}.name{text-decoration:none;color:#FF8000}</style> <p style="color: rgb(255,255,0)"><b>%0</b> <i>%1</i></p>)-")
            .arg(QDateTime::currentDateTime().toString("[t MMM dd hh:mm:ss:zzz]").toHtmlEscaped())
            .arg(tr(R"-(Hello @%0, Welcome to <a href="https://github.com/huanhuanonly/ClipboardFilter">ClipboardFilter</a>(%1). By <a href="https://codeforces.com/profile/huanhuanonly"><span class="name">YangHuanhuan</span></a> (3347484963@qq.com) in December 2023 for <span class="name">YuFeixia</span>. All rights reserved. Last update in October 2024.)-")
                .arg(QDir::home().dirName().toHtmlEscaped())
                .arg(STRVALUE(__VERSION__)))
        );
    
    connect(&huanhuan::slog, &huanhuan::Logger::itemChanged, this, [this](const huanhuan::Logger::Item& item) -> void
    {
        QTextCursor tc = ui->logBrowser->textCursor();
        
        tc.movePosition(QTextCursor::End);
        tc.select(QTextCursor::BlockUnderCursor);
        tc.removeSelectedText();
        ui->logBrowser->setTextCursor(tc);
        
        ui->logBrowser->append(QString(R"-(<p%0><i><b>%1 [%2]</b></i> %3</p>)-")
                            .arg(huanhuan::slog.index() % 2
                                ? R"-( style="color: rgb(255,51,255)")-"
                                : R"-( style="color: rgb(51,255,255)")-")
                            .arg(item.index)
                            .arg(item.time.toString("hh:mm:ss:zzz"))
                            .arg(item.text.toHtmlEscaped()));
    });
    connect(&huanhuan::slog, &huanhuan::Logger::itemAdded, this, [this](const huanhuan::Logger::Item& item)
    {
        ui->logBrowser->append(QString(R"-(<p%0><i><b>%1 [%2]</b></i> %3</p>)-")
                            .arg(huanhuan::slog.index() % 2
                                ? R"-( style="color: rgb(255,51,255)")-"
                                : R"-( style="color: rgb(51,255,255)")-")
                            .arg(item.index)
                            .arg(item.time.toString("hh:mm:ss:zzz"))
                            .arg(item.text.toHtmlEscaped()));
        
        ui->logBrowser->moveCursor(QTextCursor::End);
    });
    
    
    connect(ui->labelStatusBar, &StatusBarLabel::moreOpened, this, [this]() -> void
    {
        ui->groupBox_logger->show();
    });
    connect(ui->labelStatusBar, &StatusBarLabel::moreClosed, this, [this]() -> void
    {
        ui->groupBox_logger->hide();
    });
    
    ui->groupBox_logger->hide();
}

void MainDialog::onClipboardDataChanged()
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
        disconnect(cb, &QClipboard::dataChanged, this, &MainDialog::onClipboardDataChanged);
        state = RunState::Stopping;
        emit runningChanged(false);
    }
    // Start
    else
    {
        ui->stateButton->setText(tr("&Stop"));
        connect(cb, &QClipboard::dataChanged, this, &MainDialog::onClipboardDataChanged, Qt::QueuedConnection);
        state = RunState::Running;
        emit runningChanged(true);
    }
    return;
}

void MainDialog::on_resetButton_clicked()
{
    ui->characterListBox->clear();
    ui->beforeButton->setChecked(false);
    ui->afterButton->setChecked(true);
    ui->enableVariablesCheckBox->setChecked(false);
    
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
            text = _M_replacer->variableParser()->parse(text);
        }
        catch (const std::exception& e)
        {
            huanhuan::slog << tr("Variable parsing failed, In CharactersSet: %0").arg(e.what())
                           << huanhuan::endl;
        }
        
        ui->characterListBox->setText(text);
    }
}


void MainDialog::on_clearButton_clicked()
{
    cb->clear();
}

