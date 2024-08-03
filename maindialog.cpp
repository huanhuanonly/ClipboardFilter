#include "maindialog.h"
#include "ui_maindialog.h"

#include <QDir>
#include <QSettings>
#include <QStandardItemModel>
#include <QDebug>

#include "StringReplaceListView.h"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , cb(QGuiApplication::clipboard())
{
    ui->setupUi(this);
    setWindowTitle(tr("The Clipboard Filter, Hello @%0").arg(QDir::home().dirName()));
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    
    titlebar = new TitleBar(this);
    
    QFont itFont(ui->labelCopyright->font());
    itFont.setItalic(true);
    itFont.setPointSize(6);
    ui->labelCopyright->setFont(itFont);
    connect(cb, &QClipboard::dataChanged, this, &MainDialog::on_clipboardChange, Qt::QueuedConnection);
    
    if (QFile::exists(QDir::tempPath() + R"(/ClipboardFilter.ini)"))
    {
        QSettings ini(QDir::tempPath() + R"(/ClipboardFilter.ini)", QSettings::Format::IniFormat);
        ui->characterListBox->setText(
                    ini.value("CharactersList", ui->characterListBox->text()).toString());
        ui->escapeCharactersCheckBox->setChecked(
                    ini.value("UseEscapeCharacters", ui->escapeCharactersCheckBox->isChecked()).toBool());
        
        QList<StringReplaceListView::DataType> dataList;
        int size = ini.value("ReplaceList/ItmeConut", 0).toInt();
        for (int i = 0; i < size; ++i)
        {
            dataList.push_back(
            {   ini.value(QString("ReplaceList/Item_%0/Pattern").arg(i), QString()).toString(),
                ini.value(QString("ReplaceList/Item_%0/Replace").arg(i), QString()).toString(),
                ini.value(QString("ReplaceList/Item_%0/Escape").arg(i), false).toBool(),
                ini.value(QString("ReplaceList/Item_%0/Escape2").arg(i), false).toBool(),
                static_cast<StringReplaceListView::RegexpState>(ini.value(QString("ReplaceList/Item_%0/MatchPattern").arg(i), 0).toInt())
            });
        }
        replaceListBox = new StringReplaceListView(dataList, this);
    }
    else
    {
        replaceListBox = new StringReplaceListView(QList<StringReplaceListView::DataType>(), this);
    }
    ui->groupBox_replaceList->layout()->addWidget(replaceListBox);
}

MainDialog::~MainDialog()
{
    QFile::remove(QDir::tempPath() + R"(/ClipboardFilter.ini)");
    
    QSettings ini(QDir::tempPath() + R"(/ClipboardFilter.ini)", QSettings::Format::IniFormat);
    ini.setValue("CharactersList", ui->characterListBox->text());
    ini.setValue("UseEscapeCharacters", ui->escapeCharactersCheckBox->isChecked());
    
    auto dataList = replaceListBox->model()->dataList();
    ini.setValue("ReplaceList/ItmeConut", dataList.size());
    for (int i = 0; i < dataList.size(); ++i)
    {
        ini.setValue(QString("ReplaceList/Item_%0/Pattern").arg(i), dataList[i].text);
        ini.setValue(QString("ReplaceList/Item_%0/Replace").arg(i), dataList[i].replaceText);
        ini.setValue(QString("ReplaceList/Item_%0/Escape").arg(i), dataList[i].escapeCharactesState);
        ini.setValue(QString("ReplaceList/Item_%0/Escape2").arg(i), dataList[i].escapeCharactesState2);
        ini.setValue(QString("ReplaceList/Item_%0/MatchPattern").arg(i), static_cast<int>(dataList[i].regexpState));
    }
    
    delete ui;
}


QString MainDialog::filter(const QString& text) const noexcept
{
    QString result;
    result.reserve(text.size());
    
    for (const QChar& i : text)
    {
        if (set_char.find(i) == set_char.end())
        {
            result.push_back(i);
        }
    }
    return result;
}

void MainDialog::on_clipboardChange()
{
    static bool flag = false;
    flag = !flag;
    
    if (cb->text().isEmpty() || flag == false)
    {
        return;
    }
    QString text = cb->text();
    cb->setText(filter(replaceListBox->filter(text)));
}

void MainDialog::on_stateButton_clicked()
{
    // stop
    if (state == State::Running)
    {
        ui->stateButton->setText(tr("&Run"));
        disconnect(cb, &QClipboard::dataChanged, this, &MainDialog::on_clipboardChange);
        state = State::Stopping;
    }
    // run
    else
    {
        ui->stateButton->setText(tr("&Stop"));
        connect(cb, &QClipboard::dataChanged, this, &MainDialog::on_clipboardChange);
        state = State::Running;
    }
    return;
}


void MainDialog::on_characterListBox_textChanged(const QString& text)
{
    set_char.clear();
    
    if (ui->escapeCharactersCheckBox->isChecked() == false)
    {
        for (const QChar& i : text)
        {
            set_char.insert(i);
        }
        return;
    }
    
    for (int i = 0; i < text.size(); ++i)
    {
        if (text[i] != L'\\')
            set_char.insert(text[i]);
        else
        {
            ++i;
            if (!(i < text.size())) break;
            switch (text[i].unicode())
            {
                case L'\\':
                {
                    set_char.insert(L'\\');
                    break;
                }
                
                case L'a':
                {
                    set_char.insert(L'\a');
                    break;
                }
                
                case L'b':
                {
                    set_char.insert(L'\b');
                    break;
                }
                
                case L'f':
                {
                    set_char.insert(L'\f');
                    break;
                }
                
                case L'n':
                {
                    set_char.insert(L'\n');
                    break;
                }
                
                case L'r':
                {
                    set_char.insert(L'\r');
                    break;
                }
                
                case L't':
                {
                    set_char.insert(L'\t');
                    break;
                }
                
                case L'v':
                {
                    set_char.insert(L'\v');
                    break;
                }
                
                default:
                {
                    set_char.insert(text[i]);
                    break;
                }
            }
        }
    }
    return;
}


void MainDialog::on_clearButton_clicked()
{
    ui->characterListBox->clear();
    int lineCount = replaceListBox->model()->rowCount();
    for (int i = 0; i < lineCount; ++i)
    {
        replaceListBox->removeRow(0);
    }
}


void MainDialog::on_insertButton_clicked()
{
    QModelIndex index = replaceListBox->currentIndex();
    
    replaceListBox->insertRow(index.isValid() ? index.row() + 1 : 0);
    replaceListBox->selectRow((index.isValid() ? index.row() : 0) + 1);
    replaceListBox->edit(replaceListBox->currentIndex());
}


void MainDialog::on_removeButton_clicked()
{
    replaceListBox->removeRow(replaceListBox->currentIndex().row());
}


void MainDialog::on_updataButton_clicked()
{
    on_clipboardChange();
}

