/**
 * @file TextEditWithVariables.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief QTextEdit with additional support for variables
 * 
 * @ingroup huanhuan::ui
 * 
 * @include
 *     @class TextEditWithVariables
 *     
 * @list VariableParser.h
 *       VariablesShader.h
 */

#ifndef TEXTEDITWITHVARIABLES_H
#define TEXTEDITWITHVARIABLES_H

#include <QObject>
#include <QTextEdit>
#include <QLineEdit>

#include <QCompleter>
#include <QAbstractItemView>

#include "VariableParser.h"
#include "VariablesShader.h"

class TextEditWithVariables : public QTextEdit
{
    Q_OBJECT
    
public:
    
    TextEditWithVariables(const huanhuan::VariableParser* parser, QWidget* parent = nullptr)
        : QTextEdit(parent)
    {
        QStringList listForCompleter;
        
        const_cast<huanhuan::VariableParser*>(parser)->forEach([&](const QString& name, std::function<QString(QStringView, int)>&) -> void
        {
            listForCompleter.append(QString("${%0}").arg(name));
        });
        
        _M_completer = new QCompleter(listForCompleter, this);
        
        _M_completer->setWidget(this);
        _M_completer->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
        _M_completer->setCaseSensitivity(Qt::CaseInsensitive);
        _M_completer->setFilterMode(Qt::MatchContains);
        
        _M_variablesShader = new VariablesShader(parser, document());
    }
    
    void enableVariables()
    {
        connect(_M_completer, QOverload<const QString&>::of(&QCompleter::activated), this, [this](const QString& text)
        {
//            if (_M_completer->widget() != this)
//            {
//                return;
//            }
            
            QTextCursor tc = textCursor();
            
            QString word = wordUnderCursor();
            tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, word.size());
            
            tc.insertText(text);
            setTextCursor(tc);
        });
    }
    
    void disableVariables()
    {
        disconnect(_M_completer, nullptr, this, nullptr);
    }
    
    QString text() const
    {
        return document()->toRawText();
    }
    
    QString wordUnderCursor()
    {
        QTextCursor tc = textCursor();
        
        while (tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor))
        {
            QChar first = tc.selectedText()[0];
            if (first.isLetter() == false && first != '$' && first != '{' && first != '}')
            {
                return tc.selectedText().left(1);
            }
            else if (first == '$')
            {
                break;
            }
        }
        
        return tc.selectedText();
    }
    
    void keyPressEvent(QKeyEvent* event) override
    {
        if ((event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier))
            && event->text().isEmpty())
        {
            return;
        }
        
        if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier && event->key() == Qt::Key_Return)
        {
            if (textCursor().atEnd())
            {
                clearFocus();
                emit editingFinished();
                return;
            }
        }
        
        if (_M_completer->popup()->isVisible())
        {
            switch (event->key())
            {
                case Qt::Key_Escape:
                case Qt::Key_Enter:
                case Qt::Key_Return:
                case Qt::Key_Tab:
                case Qt::Key_Delete:
                
                    event->ignore();
                    return;
                    
                default:
                    break;
            }
        }
        
        QTextEdit::keyPressEvent(event);
        
        QString pre = wordUnderCursor();
        
        if (pre.isEmpty() == false)
        {
            _M_completer->setCompletionPrefix(pre);
            _M_completer->complete();
        }
    }
    
signals:
    
    void editingFinished();
    
private:
    
    QCompleter* _M_completer;
    VariablesShader* _M_variablesShader;
};

// For .ui file
class LineEditWithVariables : public QLineEdit
{
    Q_OBJECT
    
public:
    
    LineEditWithVariables(QWidget* parent = nullptr)
        : QLineEdit(parent)
    {
        // Note: The global VariableParser is used here!
        
        huanhuan::VariableParser::global()->forEach(
            [&](const QString& name, std::function<QString(QStringView, int)>&) -> void
            {
                _M_list.append(QString("${%0}").arg(name));
            });
    }
    
    void enableVariables()
    {
        if (_M_completer == nullptr)
        {
            _M_completer = new QCompleter(_M_list, this);
            
            _M_completer->setWidget(this);
            _M_completer->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
            _M_completer->setCaseSensitivity(Qt::CaseInsensitive);
            _M_completer->setFilterMode(Qt::MatchContains);
            
            setCompleter(_M_completer);
        }
    }
    
    void disableVariables()
    {
        if (_M_completer)
        {
            setCompleter(nullptr);
            _M_completer = nullptr;
        }
    }
    
private:
    
    QStringList _M_list;
    QCompleter* _M_completer = nullptr;
};

#endif // TEXTEDITWITHVARIABLES_H
