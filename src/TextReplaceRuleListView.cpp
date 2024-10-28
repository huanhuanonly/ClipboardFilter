/**
 * @file TextReplaceRuleListView.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Implement @class TextReplaceRuleListModel,
 *        @class TextReplaceRuleListStyledItemDelegate and
 *        @class TextReplaceRuleListView
 *     
 * @ingroup huanhuan::ui
 *     
 * @list TextReplaceRuleListView.h
 *       TextEditWithVariables.h
 *       MainDialog.h
 */

#include "TextReplaceRuleListView.h"
#include "TextEditWithVariables.h"
#include "Logger.h"

#include <array>

TextReplaceRuleListModel::
TextReplaceRuleListModel(const QList<DataType>& initDataList, QObject* parent)
    : QStandardItemModel(initDataList.size(), DataType::size(), parent)
{
    for (int i = 0; i < initDataList.size(); ++i)
    {
        for (int j = 0; j < DataType::size(); ++j)
        {
            QStandardItem* item = new QStandardItem();
            item->setData(initDataList.at(i)[j]);
            setItem(i, j, item);
        }
    }
}


QVariant
TextReplaceRuleListModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() == false)
    {
        return QVariant();
    }
    
    if (role == Qt::ItemDataRole::TextAlignmentRole)
    {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    
    if (role == Qt::ItemDataRole::DisplayRole)
    {
        switch (index.column())
        {
            case 0:
            case 2:
            {
                return itemFromIndex(index)->data().toString();
            }
                
            case 1:
            case 3:
            {
                return itemFromIndex(index)->data().toBool()
                        ? tr("ON")
                        : tr("OFF");
            }
            
            case 4:
            {
                return matchOptionsStringMap()[
                        static_cast<MatchOptions>(
                            itemFromIndex(index)->data().toInt())];
            }
            
            default:
            {
                return QVariant();
            }
        }
    }
    
    
    if (role == Qt::ItemDataRole::UserRole + 1)
    {
        return itemFromIndex(index)->data();
    }
    
    return QVariant();
}


bool
TextReplaceRuleListModel::setData(
        const QModelIndex& index,
        const QVariant& value,
        int role)
{
    if (index.isValid() == false)
    {
        return false;
    }
    
    if (role == Qt::EditRole)
    {
        itemFromIndex(index)->setData(value);
        return true;
    }
    return false;
}


QVariant
TextReplaceRuleListModel::headerData(
        int section,
        Qt::Orientation orientation,
        int role) const
{
    if (role == Qt::ItemDataRole::SizeHintRole)
    {
        return QSize(30, 30);
    }
    
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    
    if (orientation == Qt::Orientation::Vertical)
    {
        return 1 + section;
    }
    else
    {
        return headerStringList()[section];
    }
    return QVariant();
}


Qt::ItemFlags
TextReplaceRuleListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QStandardItemModel::flags(index) | Qt::ItemIsEditable;
}


QList<TextReplaceRuleListModel::DataType>
TextReplaceRuleListModel::dataList() const noexcept
{
    QList<DataType> result;
    for (int i = 0; i < rowCount(); ++i)
    {
        result.push_back(
            {
                item(i, 0)->data(),
                item(i, 1)->data(),
                item(i, 2)->data(),
                item(i, 3)->data(),
                item(i, 4)->data(),
            });
    }
    return result;
}


void
TextReplaceRuleListStyledItemDelegate::paint(QPainter* painter,
        const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}


QWidget*
TextReplaceRuleListStyledItemDelegate::createEditor(QWidget* parent,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const
{
    Q_UNUSED(option);
    
    switch (index.column())
    {
        case 0:
        case 2:
        {
            TextEditWithVariables* editer = new TextEditWithVariables(_M_parser, parent);
            
            connect(editer, &TextEditWithVariables::editingFinished,
                    this, &TextReplaceRuleListStyledItemDelegate::editingFinished);
            
            editer->setStyleSheet(R"(border: 1px solid #19FF00; border-radius: 3px;)");
            editer->enableVariables();
            return editer;
        }
        
        case 1:
        case 3:
        {
            QCheckBox* checkBox = new QCheckBox(parent);
            
            connect(checkBox, &QCheckBox::stateChanged,
                    this, &TextReplaceRuleListStyledItemDelegate::editingFinished);
            
            connect(checkBox, &QCheckBox::clicked,
                    this, [checkBox](bool checked) -> void
            {
                    if (checked)
                        checkBox->setText("On");
                    else
                        checkBox->setText("Off");
            });
            
            // subcontrol-position:center  center;
            checkBox->setStyleSheet(
R"(QCheckBox::indicator
{
    border-style: solid;
    border-radius: 10px;
    border-color: rgb(85, 255, 255);
    width: 30px;
    height: 30px;
}
QCheckBox
{
    spacing: 10px;
})");
            
            return checkBox;
        }
        
        case 4:
        {
            QComboBox* comboBox = new QComboBox(parent);
            
            comboBox->addItem(TextReplaceRuleListModel::matchOptionsStringMap()[
                                TextReplaceRuleListModel::MatchOptions::CaseSensitive],
                              QVariant::fromValue<int>(TextReplaceRuleListModel::MatchOptions::CaseSensitive));
            
            comboBox->addItem(TextReplaceRuleListModel::matchOptionsStringMap()[
                                TextReplaceRuleListModel::MatchOptions::CaseInsensitive],
                              QVariant::fromValue<int>(TextReplaceRuleListModel::MatchOptions::CaseInsensitive));
            
            comboBox->addItem(TextReplaceRuleListModel::matchOptionsStringMap()[
                                  TextReplaceRuleListModel::MatchOptions(
                                    TextReplaceRuleListModel::MatchOptions::Wildcard |
                                    TextReplaceRuleListModel::MatchOptions::CaseSensitive)],
                              QVariant::fromValue<int>(
                                  TextReplaceRuleListModel::MatchOptions::Wildcard |
                                  TextReplaceRuleListModel::MatchOptions::CaseSensitive));
            
            comboBox->addItem(TextReplaceRuleListModel::matchOptionsStringMap()[
                                TextReplaceRuleListModel::MatchOptions(
                                    TextReplaceRuleListModel::MatchOptions::Wildcard |
                                    TextReplaceRuleListModel::MatchOptions::CaseInsensitive)],
                              QVariant::fromValue<int>(
                                  TextReplaceRuleListModel::MatchOptions::Wildcard |
                                  TextReplaceRuleListModel::MatchOptions::CaseInsensitive));
            
            comboBox->addItem(TextReplaceRuleListModel::matchOptionsStringMap()[
                                TextReplaceRuleListModel::MatchOptions(
                                    TextReplaceRuleListModel::MatchOptions::Regex |
                                    TextReplaceRuleListModel::MatchOptions::CaseSensitive)],
                              QVariant::fromValue<int>(
                                  TextReplaceRuleListModel::MatchOptions::Regex |
                                  TextReplaceRuleListModel::MatchOptions::CaseSensitive));
            
            comboBox->addItem(TextReplaceRuleListModel::matchOptionsStringMap()[
                                TextReplaceRuleListModel::MatchOptions(
                                    TextReplaceRuleListModel::MatchOptions::Regex |
                                    TextReplaceRuleListModel::MatchOptions::CaseInsensitive)],
                              QVariant::fromValue<int>(
                                  TextReplaceRuleListModel::MatchOptions::Regex |
                                  TextReplaceRuleListModel::MatchOptions::CaseInsensitive));
            
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    this, &TextReplaceRuleListStyledItemDelegate::editingFinished);
            
            return comboBox;
        }
        
        default:
        {
            return nullptr;
        }
    }
}


void
TextReplaceRuleListStyledItemDelegate::setEditorData(QWidget* editor,
        const QModelIndex& index) const
{
    if (index.isValid() == false) return;
    
    switch (index.column())
    {
        case 0:
        case 2:
        {
            qobject_cast<TextEditWithVariables*>(editor)->setText(
                        index.model()->data(index, Qt::ItemDataRole::UserRole + 1).toString());
            return;
        }
            
        case 1:
        case 3:
        {
            QCheckBox* widget = qobject_cast<QCheckBox*>(editor);
            widget->setChecked(
                        index.model()->data(index, Qt::ItemDataRole::UserRole + 1).toBool());
            if (widget->isChecked())
                widget->setText("On");
            else
                widget->setText("Off");
            return;
        }
            
        case 4:
        {
            const QString& str =
                    TextReplaceRuleListModel::matchOptionsStringMap()[
                        static_cast<TextReplaceRuleListModel::MatchOptions>(
                            index.model()->data(index, Qt::ItemDataRole::UserRole + 1).toInt())];
            
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            
            for (int i = 0, cnt = comboBox->count(); i < cnt; ++i)
            {
                if (comboBox->itemText(i) == str)
                {
                    comboBox->setCurrentIndex(i);
                    break;
                }
            }
            
            return;
        }
        
        default:
        {
            QStyledItemDelegate::setEditorData(editor, index);
            return;
        }
    }
    return;
}


void
TextReplaceRuleListStyledItemDelegate::setModelData(QWidget* editor,
                  QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    if (index.isValid() == false) return;
    
    switch (index.column())
    {
        case 0:
        case 2:
        {
            model->setData(index, qobject_cast<TextEditWithVariables*>(editor)->text());
            return;
        }
            
        case 1:
        case 3:
        {
            model->setData(index, qobject_cast<QCheckBox*>(editor)->isChecked());
            return;
        }
            
        case 4:
        {
            model->setData(index, qobject_cast<QComboBox*>(editor)->currentData());
            return;
        }
        
        default:
        {
            QStyledItemDelegate::setModelData(editor, model, index);
            return;
        }
    }
    return;
}


void
TextReplaceRuleListStyledItemDelegate::updateEditorGeometry(
        QWidget* editor,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}


TextReplaceRuleListView::TextReplaceRuleListView(
        const std::shared_ptr<huanhuan::TextReplacer>& replacer,
        const QList<DataType>& initDataList,
        QWidget* parent)
    : QTableView(parent)
    , _M_model(new TextReplaceRuleListModel(initDataList, this))
    , _M_replacer(replacer)
{
    setAlternatingRowColors(true);
//    setDragEnabled(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
    setModel(_M_model);
    TextReplaceRuleListStyledItemDelegate* delegate = new TextReplaceRuleListStyledItemDelegate(_M_replacer->variableParser(), this);
    setItemDelegate(delegate);
    
    for (int i = 0; i < DataType::size(); ++i)
    {
        setColumnWidth(i,
            horizontalHeader()->fontMetrics()
                .horizontalAdvance(_M_model->headerStringList().at(i)) +
                       (i == 0 || i == 2 ? 60 : i == 1 || i == 3 ? 20 : 100));
    }
}


TextReplaceRuleListModel*
TextReplaceRuleListView::model() const noexcept
{
    return _M_model;
}


void
TextReplaceRuleListView::insertRow(int pos)
{
    QStandardItem* item = new QStandardItem();
    _M_model->insertRow(pos, item);
}


void
TextReplaceRuleListView::removeRow(int pos)
{
    _M_model->removeRow(pos);
}


void
TextReplaceRuleListView::filter(const QString& text, const DataType& data)
{
    _M_replacer->buffer() = _M_replacer->setRule(data).replace(text);
}


QString
TextReplaceRuleListView::filter(const QString& text)
{
    auto dataList = _M_model->dataList();
    for (int i = 0; i < dataList.size(); ++i)
    {
        try
        {
            huanhuan::slog << tr("Using the %0-%1 rule to replace...")
                              .arg(i + 1)
                              .arg(std::array<const char*, 5>{"th", "st", "nd", "rd", "th"}[std::min((i + 1) % 10, 4)])
                           << huanhuan::endl;
            
            filter(text, dataList.at(i));
        }
        catch (const std::exception& e)
        {
            huanhuan::slog << tr("Error in Replace's Rule %0-%1 item: %2")
                              .arg(i + 1)
                              .arg(std::array<const char*, 5>{"th", "st", "nd", "rd", "th"}[std::min((i + 1) % 10, 4)])
                              .arg(e.what())
                           << huanhuan::endl;
        }
    }
    return _M_replacer->buffer();
}
