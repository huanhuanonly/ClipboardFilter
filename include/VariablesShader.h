/**
 * @file VariablesShader.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Variable shader
 * 
 * @ingroup huanhuan::ui
 */

#ifndef VARIABLESSHADER_H
#define VARIABLESSHADER_H

#include <QObject>
#include <QTextDocument>
#include <QSyntaxHighlighter>

#include <QVector>
#include <QColor>

#include <algorithm>

#include "VariableParser.h"

class VariablesShader : public QSyntaxHighlighter
{
    Q_OBJECT
    
public:
    
    VariablesShader(const huanhuan::VariableParser* variablesParser, QTextDocument* parent = nullptr)
        : QSyntaxHighlighter(parent)
        , _M_parser(variablesParser)
    { }
    
    void highlightBlock(const QString& text) override
    {
        static const QVector<QColor> colors {
            QColor(0, 255, 255),
            QColor(255, 0, 255),
            QColor(255, 255, 0),
            QColor(255, 128, 0),
            QColor(0, 255, 0)
        };
        
        QTextCharFormat f_sym_accept;
        f_sym_accept.setFontItalic(true);
        
        QTextCharFormat f_var_accept;
        f_var_accept.setFontWeight(1000);
        f_var_accept.setForeground(QColor(0, 255, 0));
        
        QTextCharFormat f_sym_reject;
        f_sym_reject.setFontStrikeOut(true);
        f_sym_reject.setForeground(QColor(255, 31, 31));
        
        QTextCharFormat f_var_reject;
        f_var_reject.setFontUnderline(true);
        f_var_reject.setForeground(QColor(255, 31, 31));
        
        int stack = 0;
        
        QString ins;
        bool meet = false;
        
        for (int i = 0; i < text.size(); ++i)
        {
            if (text[i] == '{')
            {
                if (i && text[i - 1] == '$')
                {
                    meet = false;
                    
                    f_sym_accept.setForeground(colors[stack % colors.size()]);
                    setFormat(i - 1, 2, f_sym_accept);
                    
                    ++stack;
                }
                else
                {
                    setFormat(i, 1, f_sym_reject);
                }
                
                ins.clear();
            }
            else if (text[i] == '}')
            {
                if (stack)
                {
                    meet = false;
                    --stack;
                    
                    f_sym_accept.setForeground(colors[stack % colors.size()]);
                    setFormat(i, 1, f_sym_accept);
                    
                    if (meet == false)
                    {
                        if (_M_parser->contains(ins.trimmed()))
                        {
                            setFormat(i - ins.size(), ins.size(), f_var_accept);
                        }
                        else
                        {
                            setFormat(i - ins.size(), ins.size(), f_var_reject);
                        }
                    }
                }
                else
                {
                    setFormat(i, 1, f_sym_reject);
                }
                
                ins.clear();
            }
            else if (text[i] == ':')
            {
                if (stack)
                {
                    meet = true;
                    
                    if (_M_parser->contains(ins.trimmed()))
                    {
                        setFormat(i - ins.size(), ins.size(), f_var_accept);
                    }
                    else
                    {
                        setFormat(i - ins.size(), ins.size(), f_var_reject);
                    }
                    
                    setFormat(i, 1, f_sym_accept);
                    
                    ins.clear();
                }
            }
            else if (stack && meet == false)
            {
                ins.append(text[i].toLower());
            }
        }
    }
    
private:
    
    const huanhuan::VariableParser* _M_parser;
};

#endif // VARIABLESSHADER_H
