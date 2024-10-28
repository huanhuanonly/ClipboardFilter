/**
 * @file ClickableLabel.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief ClickableLabel class
 *        Inherit QLabel and send cursor click and double-click signals
 *        
 * @ingroup huanhuan::ui
 */

#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT
    
public:
    
    ClickableLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QLabel(parent, f)
    { }
    
    ClickableLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QLabel(text, parent, f)
    { }
    
protected:
    
    void mousePressEvent(QMouseEvent* e) override
    {
        if (e->button() == Qt::LeftButton)
        {
            _M_clicked = true;
        }
        
        QLabel::mousePressEvent(e);
    }
    
    void mouseMoveEvent(QMouseEvent* e) override
    {
        _M_clicked = false;
        
        QLabel::mouseMoveEvent(e);
    }
    
    void mouseReleaseEvent(QMouseEvent* e) override
    {
        if (_M_clicked)
        {
            emit clicked();
            _M_clicked = false;
        }
        
        QLabel::mouseReleaseEvent(e);
    }
    
    void mouseDoubleClickEvent(QMouseEvent* e) override
    {
        if (e->button() == Qt::LeftButton)
        {
            emit doubleClicked();
        }
        
        QLabel::mouseDoubleClickEvent(e);
    }
    
signals:
    
    void clicked();
    void doubleClicked();
    
private:
    
    bool _M_clicked = false;
};

#endif // CLICKABLELABEL_H
