#ifndef STRINGREPLACELISTVIEW_H
#define STRINGREPLACELISTVIEW_H

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

class StringReplaceListModel : public QStandardItemModel
{
public:
    
    enum class RegexpState: int
    {
        None = 0,
        Wildcard,
        Regexp
    };
    struct DataType
    {
        QString     text;
        QString     replaceText;
        bool        escapeCharactesState;
        bool        escapeCharactesState2;
        RegexpState regexpState;
        
        explicit DataType() = default;
        
        DataType(const QString& v0, const QString& v1,
                 bool v2, bool v3, RegexpState v4)
            : text(v0), replaceText(v1)
            , escapeCharactesState(v2), escapeCharactesState2(v3)
            , regexpState(v4) { }
        
        static constexpr inline int size() noexcept { return 5; }
        
        friend class StringReplaceListModel;
        
    protected:
        inline QVariant operator[](int i) const
        {
            switch (i)
            {
            case 0: return text;
            case 1: return escapeCharactesState;
            case 2: return replaceText;
            case 3: return escapeCharactesState2;
            case 4: return static_cast<int>(regexpState);
            default: return QVariant();
            }
        }
        
        DataType(
                const QVariant& v0, const QVariant& v1,
                const QVariant& v2, const QVariant& v3,
                const QVariant& v4)
            : text(v0.toString())
            , replaceText(v2.toString())
            , escapeCharactesState(v1.toBool())
            , escapeCharactesState2(v3.toBool())
            , regexpState(static_cast<RegexpState>(v4.toInt()))
        { }
    };
    
    explicit StringReplaceListModel(const QList<DataType>& initDataList = QList<DataType>(), QObject* parent = nullptr)
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
                
                case 4: return _M_regexStateStringList[itemFromIndex(index)->data().toInt()];
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
    
    constexpr inline const QStringList& regexStateStringList() const noexcept
    {
        return _M_regexStateStringList;
    }
    
private:
    QStringList _M_headerStringList =
    {
        tr("Pattern"),
        tr("Escape"),
        tr("Replace"),
        tr("Escape"),
        tr("MatchPattern")
    };
    
    QStringList _M_regexStateStringList =
    {
        tr("None"), tr("Wildcard"), tr("Regex")
    };
};

class StringReplaceListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    
public:
    StringReplaceListItemDelegate(QObject* parent = nullptr)
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
                QLineEdit* lineEdit = new QLineEdit(parent);
                connect(lineEdit, &QLineEdit::editingFinished, this, &StringReplaceListItemDelegate::editingFinished);
                lineEdit->setStyleSheet(R"(border: 1px solid #19FF00; border-radius: 5px;)");
                return lineEdit;
            }
            
            case 1:
            case 3:
            {
                QCheckBox* checkBox = new QCheckBox(parent);
                connect(checkBox, &QCheckBox::stateChanged, this, &StringReplaceListItemDelegate::editingFinished);
                connect(checkBox, &QCheckBox::clicked, this, [checkBox](bool checked) -> void
                {
                    if (checked)
                        checkBox->setText(tr("+"));
                    else
                        checkBox->setText(tr("-"));
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
                connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StringReplaceListItemDelegate::editingFinished);
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
                qobject_cast<QLineEdit*>(editor)->setText(
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
                    widget->setText(tr("+"));
                else
                    widget->setText(tr("-"));
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
                model->setData(index, qobject_cast<QLineEdit*>(editor)->text());
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
    //    emit closeEditor(qobject_cast<QWidget*>(sender()));
    }
};

class StringReplaceListView : public QTableView
{
public:
    
    using RegexpState = StringReplaceListModel::RegexpState;
    using DataType    = StringReplaceListModel::DataType;
    
    StringReplaceListView(const QList<DataType>& initDataList = QList<DataType>(), QWidget* parent = nullptr)
        : QTableView(parent)
        , _M_model(new StringReplaceListModel(initDataList, this))
    {
        setAlternatingRowColors(true);
    //    setDragEnabled(true);
        setDefaultDropAction(Qt::DropAction::MoveAction);
        setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);
        setModel(_M_model);
        StringReplaceListItemDelegate* delegate = new StringReplaceListItemDelegate(this);
        setItemDelegate(delegate);
        
        for (int i = 0; i < DataType::size(); ++i)
        {
            setColumnWidth(i,
                horizontalHeader()->fontMetrics()
                    .horizontalAdvance(_M_model->headerStringList().at(i)) + (i == 0 || i == 2 ? 60 : 20));
        }
    }
    
    StringReplaceListModel* model() const noexcept
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
    
    static QString filter_escapeCharacters(const QString& text)
    {
        QString result;
        result.reserve(text.size());
        
        for (int i = 0; i < text.size(); ++i)
        {
            if (text[i] != L'\\')
                result.push_back(text[i]);
            else
            {
                ++i;
                if (!(i < text.size())) break;
                switch (text[i].unicode())
                {
                    case L'\\':
                    {
                        result.push_back(L'\\');
                        break;
                    }
                    
                    case L'a':
                    {
                        result.push_back(L'\a');
                        break;
                    }
                    
                    case L'b':
                    {
                        result.push_back(L'\b');
                        break;
                    }
                    
                    case L'f':
                    {
                        result.push_back(L'\f');
                        break;
                    }
                    
                    case L'n':
                    {
                        result.push_back(L'\n');
                        break;
                    }
                    
                    case L'r':
                    {
                        result.push_back(L'\r');
                        break;
                    }
                    
                    case L't':
                    {
                        result.push_back(L'\t');
                        break;
                    }
                    
                    case L'v':
                    {
                        result.push_back(L'\v');
                        break;
                    }
                    
                    default:
                    {
                        result.push_back(text[i]);
                        break;
                    }
                }
            }
        }
        return result;
    }
    
    static QString& filter(QString& text, const DataType& data)
    {
        QString str;
        if (data.escapeCharactesState)
            str = filter_escapeCharacters(data.text);
        else
            str = data.text;
        
        QRegExp regexp(str);
        switch (data.regexpState)
        {
            case RegexpState::None:
            {
                if (data.escapeCharactesState2)
                    return text.replace(str, filter_escapeCharacters(data.replaceText));
                else
                    return text.replace(str, data.replaceText);
            }
            
            case RegexpState::Wildcard:
            {
                regexp.setPatternSyntax(QRegExp::Wildcard);
                break;
            }
                
            case RegexpState::Regexp:
            {
                regexp.setPatternSyntax(QRegExp::RegExp2);
                break;
            }
        }
        if (data.escapeCharactesState2)
            return text.replace(regexp, filter_escapeCharacters(data.replaceText));
        else
            return text.replace(regexp, data.replaceText);
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
    StringReplaceListModel* _M_model;
};

#endif // STRINGREPLACELISTVIEW_H
