/**
 * @file TextReplaceRuleListView.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Custom QTableView,
 *        Used to set multiple replacement rules
 * 
 * @include
 *     @class TextReplaceRuleListModel
 *     @class TextReplaceRuleListStyledItemDelegate
 *     @class TextReplaceRuleListView
 *     
 * @list TextReplacer.h
 */

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
    
    explicit
    TextReplaceRuleListModel(
            const QList<DataType>& initDataList = QList<DataType>(),
            QObject* parent = nullptr);
    
    
    QVariant
    data(const QModelIndex& index, int role) const override;

    
    QVariant
    headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;
    
    
    Qt::ItemFlags
    flags(const QModelIndex &index) const override;
    
    
    QList<DataType>
    dataList() const noexcept;
    
    
    constexpr inline const QStringList&
    headerStringList() const noexcept
    {
        return _M_headerStringList;
    }
    
    
    constexpr inline const QStringList&
    matchOptionsStringList() const noexcept
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
        : QStyledItemDelegate(parent)
    { }
    
    
    void
    paint(
        QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

    
    QWidget*
    createEditor(
            QWidget* parent,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;
    

    void
    setEditorData(
            QWidget* editor,
            const QModelIndex& index) const override;
    
    
    void
    setModelData(
            QWidget* editor,
            QAbstractItemModel* model,
            const QModelIndex& index) const override;
    
    void
    updateEditorGeometry(
            QWidget* editor,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const override;
    
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
    
    
    TextReplaceRuleListView(
        const std::shared_ptr<huanhuan::TextReplacer>& replacer,
        const QList<DataType>& initDataList = QList<DataType>(),
        QWidget* parent = nullptr);
    
    
    TextReplaceRuleListModel*
    model() const noexcept;
    
    
    void
    insertRow(int pos);
    
    
    void
    removeRow(int pos);
    
    
    QString&
    filter(QString& text, const DataType& data);
    
    
    QString&
    filter(QString& text);
    
private:
    
    TextReplaceRuleListModel* _M_model;
    std::shared_ptr<huanhuan::TextReplacer> _M_replacer;
};

#endif // TEXTREPLACERULELISTVIEW_H
