/**
 * @file MainFloatingWindow.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @implements MainFloatingWindow class
 * 
 * @ingroup huanhuan::ui
 */

#include "MainFloatingWindow.h"
#include "ui_maindialog.h"

MainFloatingWindow::MainFloatingWindow(MainDialog* parent)
    : QDialog(parent)
    , _M_icon(new ClickableLabel(this))
    , _M_runningSwitch(new QPushButton(this))
    , _M_statusBar(new QLabel(this))
{
    hide();
    
    QScreen* screen = QApplication::primaryScreen();
    
    constexpr int width = SETTINGS__FLOATINGWINDOW_WIDTH, height = SETTINGS__FLOATINGWINDOW_HEIGHT;
    
    QRect rect {
        screen->size().width() / 5 * 4 - width / 2,
        screen->size().height() / 5 * 1 - height / 2,
        width,
        height
    };
    
    setGeometry(rect);
    
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowType::WindowStaysOnTopHint);
    setFixedSize(width, height);
    
    setWindowOpacity(0.7);
    
    setStyleSheet(R"(
*
{
    font-size: 22px;
    font-weight: normal;
}
.MainFloatingWindow
{
    background-color: rgb(51, 51, 51);
    border: 2px solid rgb(0, 255, 255);
    border-radius: 20px;
}
.MainFloatingWindow:!active
{
    border: 1px solid rgb(0, 155, 155);
    border-radius: 20px;
})");
    _M_icon->setStyleSheet("background-color: rgb(51, 51, 51);");
    _M_statusBar->setStyleSheet("background-color: rgb(51, 51, 51); font-style: italic;");
    
    
    _M_icon->setPixmap(QPixmap(":/Icon/MainIcon.ico").scaled(height - 10, height - 10, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    _M_runningSwitch->setText(parent->state == MainDialog::RunState::Running ? tr("Running") : tr("Continue"));
    _M_statusBar->setText(StatusBarLabel::_S_map[parent->ui->labelStatusBar->state()]);
    
    
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _M_icon->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    _M_runningSwitch->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    _M_statusBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    
    _M_icon->setAlignment(Qt::AlignCenter);
    _M_statusBar->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    
    connect(_M_icon, &ClickableLabel::clicked, this, &MainFloatingWindow::iconClicked);
    connect(parent, &MainDialog::runningChanged, this, [this](bool is_running) -> void
    {
        _M_runningSwitch->setText(is_running ? tr("Running") : tr("Continue"));
    });
    connect(_M_runningSwitch, &QPushButton::clicked, parent->ui->stateButton, &QPushButton::clicked);
    
    connect(parent->ui->labelStatusBar, &StatusBarLabel::stateChanged, this, [this](StatusBarLabel::Status status) -> void
    {
        _M_statusBar->setText(StatusBarLabel::_S_map[status]);
    });
    
    
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    
    hLayout->setSpacing(7);
    hLayout->setContentsMargins(7, 7, 7, 7);
    
    hLayout->addWidget(_M_icon);
    
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setFixedWidth(1);
    line->setFixedHeight(height - 15);
    line->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    line->setStyleSheet("background-color: rgb(0, 255, 0); min-width:1px;");
    hLayout->addWidget(line);
    
    hLayout->addWidget(_M_runningSwitch);
    hLayout->addWidget(_M_statusBar);
    
    setLayout(hLayout);
}

void MainFloatingWindow::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MainFloatingWindow::mousePressEvent(QMouseEvent* e)
{
    _M_moving = true;
    _M_lastMousePos = e->globalPos();
    QWidget::mousePressEvent(e);
}

void MainFloatingWindow::mouseMoveEvent(QMouseEvent* e)
{
    if (_M_moving)
    {
        QPoint movePoint = e->globalPos() - _M_lastMousePos;
        _M_lastMousePos = e->globalPos();
        
        move(pos().x() + movePoint.x(), pos().y() + movePoint.y());
    }
    QWidget::mouseMoveEvent(e);
}

void MainFloatingWindow::mouseReleaseEvent(QMouseEvent* e)
{
    _M_moving = false;
    QWidget::mouseReleaseEvent(e);
}
