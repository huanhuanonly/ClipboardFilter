#ifndef TEXTREPLACERULELISTVIEW_H
#define TEXTREPLACERULELISTVIEW_H

#include <QWidget>
#include <QFrame>
#include <QList>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QRegExp>
#include <QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QApplication>

#include "TextReplacer.h"
#include "TextEditWithVariables.h"
#include "Logger.h"

class TextReplaceRuleListModel : public QStandardItemModel
{
public:
    
    using MatchOptions = huanhuan::TextReplacer::MatchOptions;
    
    struct DataType : public huanhuan::TextReplacer::Rule
    {
        static constexpr inline int size() noexcept { return 5; }
        
        friend class TextReplaceRuleListModel;
        friend class MainDialog;
        
    protected:
        inline QVariant operator[](int i) const
        {
            switch (i)
            {
            case 0: return pattern;
            case 1: return patternEnableVariables;
            case 2: return replacement;
            case 3: return replacementEnableVariables;
            case 4: return static_cast<int>(matchOptions);
            default: return QVariant();
            }
        }
        
        DataType(
                const QVariant& v0, const QVariant& v1,
                const QVariant& v2, const QVariant& v3,
                const QVariant& v4)
        {
            pattern = v0.toString();
            replacement = v2.toString();
            patternEnableVariables = v1.toBool();
            replacementEnableVariables = v3.toBool();
            matchOptions = static_cast<decltype(matchOptions)>(v4.toInt());
        }
    };
    
    explicit TextReplaceRuleListModel(const QList<DataType>& initDataList = QList<DataType>(), QObject* parent = nullptr)
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
    
    QVariant data(const QModelIndex& index, int role) const override
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
                    return itemFromIndex(index)->data().toBool() ? tr("ON") : tr("OFF");
                }
                
                case 4: return _M_matchOptionsStringList[itemFromIndex(index)->data().toInt()];
                default: return QVariant();
            }
        }
        
        
        if (role == Qt::ItemDataRole::UserRole + 1)
        {
            return itemFromIndex(index)->data();
        }
        
        return QVariant();
    }
    
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override
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

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override
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
    
    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return Qt::ItemIsEnabled;
    
        return QStandardItemModel::flags(index) | Qt::ItemIsEditable;
    }
    
    QList<DataType> dataList() const noexcept
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
    
    constexpr inline const QStringList& headerStringList() const noexcept
    {
        return _M_headerStringList;
    }
    
    constexpr inline const QStringList& matchOptionsStringList() const noexcept
    {
        return _M_matchOptionsStringList;
    }
    
private:
    QStringList _M_headerStringList =
    {
        tr("Pattern"),
        tr("EnVars"),
        tr("Replacement"),
        tr("EnVars"),
        tr("MatchOptions")
    };
    
    QStringList _M_matchOptionsStringList =
    {
        tr("None"), tr("Wildcard"), tr("Regex")
    };
};

class TextReplaceRuleListStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    
public:
    TextReplaceRuleListStyledItemDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent) { }
    
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override
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

    void setEditorData(QWidget* editor, const QModelIndex& index) const override
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
    
    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override
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
    
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const override
    {
        Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }
    
private slots:
    void editingFinished()
    {
        emit commitData(qobject_cast<QWidget*>(sender()));
        emit closeEditor(qobject_cast<QWidget*>(sender()));
    }
};

class TextReplaceRuleListView : public QTableView
{
public:
    
    using MatchOptions = TextReplaceRuleListModel::MatchOptions;
    using DataType     = TextReplaceRuleListModel::DataType;
    
    TextReplaceRuleListView(const std::shared_ptr<huanhuan::TextReplacer>& replacer, const QList<DataType>& initDataList = QList<DataType>(), QWidget* parent = nullptr)
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
    
    TextReplaceRuleListModel* model() const noexcept
    {
        return _M_model;
    }
    
    void insertRow(int pos)
    {
        QStandardItem* item = new QStandardItem();
        _M_model->insertRow(pos, item);
    }
    
    void removeRow(int pos)
    {
        _M_model->removeRow(pos);
    }
    
    QString& filter(QString& text, const DataType& data)
    {
        text = _M_replacer->setRule(data).replace(text, [](QStringView variable, const char* what) -> void
        {
            huanhuan::slog << tr("From variable ${ %0 }: %1").arg(variable).arg(what);
        });
        return text;
    }
    
    QString& filter(QString& text)
    {
        auto dataList = _M_model->dataList();
        for (int i = 0; i < dataList.size(); ++i)
        {
            filter(text, dataList.at(i));
        }
        return text;
    }
    
private:
    
    TextReplaceRuleListModel* _M_model;
    std::shared_ptr<huanhuan::TextReplacer> _M_replacer;
};

#endif // TEXTREPLACERULELISTVIEW_H
