/**
 * @file TextReplaceRuleListView.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Implement @class TextReplaceRuleListModel,
 *        @class TextReplaceRuleListStyledItemDelegate and
 *        @class TextReplaceRuleListView
 *     
 * @list TextReplaceRuleListView.h
 *       TextEditWithVariables.h
 *       MainDialog.h
 */

#include "TextReplaceRuleListView.h"
#include "TextEditWithVariables.h"
#include "MainDialog.h"

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
                return _M_matchOptionsStringList[itemFromIndex(index)->data().toInt()];
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
TextReplaceRuleListModel::setData(const QModelIndex& index,
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
TextReplaceRuleListModel::headerData(int section, Qt::Orientation orientation,
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
        return section;
    }
    else
    {
        return _M_headerStringList[section];
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
            TextEditWithVariables* editer = new TextEditWithVariables(parent);
            connect(editer, &TextEditWithVariables::editingFinished, this, &TextReplaceRuleListStyledItemDelegate::editingFinished);
            editer->setStyleSheet(R"(border: 1px solid #19FF00; border-radius: 3px;)");
            editer->enableVariables();
            return editer;
        }
        
        case 1:
        case 3:
        {
            QCheckBox* checkBox = new QCheckBox(parent);
            connect(checkBox, &QCheckBox::stateChanged, this, &TextReplaceRuleListStyledItemDelegate::editingFinished);
            connect(checkBox, &QCheckBox::clicked, this, [checkBox](bool checked) -> void
            {
                if (checked)
                    checkBox->setText("$");
                else
                    checkBox->setText("_");
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
            comboBox->addItems({ tr("None"), tr("Wildcard"), tr("Regex") });
            connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextReplaceRuleListStyledItemDelegate::editingFinished);
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
                widget->setText("$");
            else
                widget->setText("_");
            return;
        }
            
        case 4:
        {
            qobject_cast<QComboBox*>(editor)->setCurrentIndex(
                        index.model()->data(index, Qt::ItemDataRole::UserRole + 1).toInt());
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
            model->setData(index, qobject_cast<QComboBox*>(editor)->currentIndex());
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
    TextReplaceRuleListStyledItemDelegate* delegate = new TextReplaceRuleListStyledItemDelegate(this);
    setItemDelegate(delegate);
    
    for (int i = 0; i < DataType::size(); ++i)
    {
        setColumnWidth(i,
            horizontalHeader()->fontMetrics()
                .horizontalAdvance(_M_model->headerStringList().at(i)) + (i == 0 || i == 2 ? 60 : 20));
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


QString&
TextReplaceRuleListView::filter(QString& text, const DataType& data)
{
    text = _M_replacer->setRule(data).replace(text, &MainDialog::variableParsingFailed);
    return text;
}


QString&
TextReplaceRuleListView::filter(QString& text)
{
    auto dataList = _M_model->dataList();
    for (int i = 0; i < dataList.size(); ++i)
    {
        filter(text, dataList.at(i));
    }
    return text;
}
