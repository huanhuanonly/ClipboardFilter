/**
 * @file MainFloatingWindow.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Main UI
 * 
 *        Floating window after the main window is hidden
 * 
 * @ingroup huanhuan::ui
 * 
 * @include
 *     @class MainFloatingWindow
 */

#ifndef MAINFLOATINGWINDOW_H
#define MAINFLOATINGWINDOW_H

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QScreen>

#include "maindialog.h"
#include "ClickableLabel.h"

class MainFloatingWindow : public QDialog
{
    Q_OBJECT
    
public:
    
    MainFloatingWindow(MainDialog* parent = nullptr);
    
protected:
    
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    
signals:
    
    void iconClicked();
    
private:
    
    QPoint _M_lastMousePos;
    bool _M_moving = false;
    
    ClickableLabel* _M_icon;
    QPushButton* _M_runningSwitch;
    QLabel* _M_statusBar;
};

#endif // MAINFLOATINGWINDOW_H
