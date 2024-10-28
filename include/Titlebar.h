/**
 * @file Titlebar.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Customize the title bar to replace
 *        the system default title bar.
 *        
 * @ingroup huanhuan::ui
 */

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QSlider>
#include <QHBoxLayout>
#include <QPair>
#include <QPainter>
#include <QPaintEvent>

class TitleBar : public QWidget
{
    Q_OBJECT
    
public:
    
    TitleBar(QWidget* parent, QString title = QString(), QWidget* floatingWindow = nullptr)
        : QWidget(parent)
        , _M_fbColor(QColor(0xff, 0xff, 0xff), QColor(0x33, 0x33, 0x33))
        , _M_titleLabel(new QLabel(this))
        , _M_opacitySlider(new QSlider(Qt::Orientation::Horizontal, this))
        , _M_shrinkButton(new QPushButton(this))
        , _M_topmostButton(new QPushButton(this))
        , _M_closeButton(new QPushButton(this))
        , _M_floatingWindow(floatingWindow)
    {
        if (parent)
        {
            connect(parent, &QWidget::windowTitleChanged, this, &TitleBar::setTitle);
            parent->layout()->setMenuBar(this);
            parent->installEventFilter(this);
        }
        
        setAttribute(Qt::WA_TranslucentBackground);
        setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        setFixedHeight(_M_titlebarHeight);
        
        _M_titleLabel->setText(title.isEmpty() && parent != nullptr ? parent->windowTitle() : title);
        _M_titleLabel->setAlignment(Qt::AlignmentFlag::AlignVCenter);
        _M_titleLabel->setMinimumHeight(_M_titlebarHeight);
        _M_titleLabel->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        QPalette palette;
        palette.setColor(QPalette::WindowText, _M_fbColor.first);
        _M_titleLabel->setPalette(palette);
        _M_titleLabel->setStyleSheet(R"(background-color: transparent; font-weight: 700;)");
        
        _M_opacitySlider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        _M_opacitySlider->setMinimum(30);
        _M_opacitySlider->setMaximum(100);
        _M_opacitySlider->setValue(100);
        _M_opacitySlider->setStyleSheet(R"(background-color: transparent)");
        
        if (parent) [[likely]]
        {
            connect(_M_opacitySlider, &QSlider::sliderMoved, this, [parent](int value)
            {
                parent->setWindowOpacity(static_cast<qreal>(value) / 100);
            });
        }
        
        _M_shrinkButton->setFixedSize(_M_titlebarHeight, _M_titlebarHeight);
        _M_topmostButton->setFixedSize(_M_titlebarHeight, _M_titlebarHeight);
        _M_closeButton->setFixedSize(_M_titlebarHeight, _M_titlebarHeight);
        
        _M_opacitySlider->setToolTip(tr("Opacity Slider"));
        _M_shrinkButton->setToolTip(tr("Shrink to floating window"));
        _M_topmostButton->setToolTip(tr("Topmost"));
        _M_closeButton->setToolTip(tr("Close"));
        
        _M_shrinkButton->setIcon(QIcon(QPixmap(":/Icon/shrink.svg").scaled(_M_titlebarHeight, _M_titlebarHeight)));
        _M_topmostButton->setIcon(QIcon(QPixmap(":/Icon/topmost.svg").scaled(_M_titlebarHeight, _M_titlebarHeight)));
        _M_closeButton->setIcon(QIcon(QPixmap(":/Icon/exit.svg").scaled(_M_titlebarHeight, _M_titlebarHeight)));
        
        _M_topmostButton->setFlat(true);
        _M_closeButton->setFlat(true);
        
        connect(_M_shrinkButton, &QPushButton::clicked, this, &TitleBar::onShrinkButtonClicked);
        connect(_M_topmostButton, &QPushButton::clicked, this, &TitleBar::onTopmostButtonClicked);
        connect(_M_closeButton, &QPushButton::clicked, this, &TitleBar::onCloseButtonClicked);
        
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        
        hLayout->addWidget(_M_titleLabel);
        
        hLayout->addWidget(_M_opacitySlider);
        
        hLayout->addWidget(_M_shrinkButton);
        hLayout->addWidget(_M_topmostButton);
        hLayout->addWidget(_M_closeButton);
        
        hLayout->setSpacing(5);
        hLayout->setContentsMargins(5, 0, 0, 0);
        
        setLayout(hLayout);
        setStyleSheet(R"(border: none;)");
    }
    
    virtual ~TitleBar()
    {
        if (_M_floatingWindow) [[likely]]
        {
            _M_floatingWindow->deleteLater();
        }
    }
    
    TitleBar& operator=(const TitleBar&) = delete;
    TitleBar& operator=(TitleBar&&) = delete;
    
public slots:
    
    void setTitle(const QString& title)
    {
        _M_titleLabel->setText(title);
    }
    
public:
    
    [[nodiscard]] QString title() const
    {
        return _M_titleLabel->text();
    }
    
    void setColor(const QPair<QColor, QColor>& color)
    {
        _M_fbColor = color;
        update();
    }
    
    void setForegroundColor(const QColor& foreground)
    {
        _M_fbColor.first = foreground;
        update();
    }
    
    void setBackgroundColor(const QColor& background)
    {
        _M_fbColor.second = background;
        update();
    }
    
    [[nodiscard]] QPair<QColor, QColor> color() const noexcept
    {
        return _M_fbColor;
    }
    
    void setFloatingWindow(QWidget* widget) noexcept
    {
        _M_floatingWindow = widget;
    }
    
    [[nodiscard]] QWidget* floatingWindow() const noexcept
    {
        return _M_floatingWindow;
    }
    
public slots:
    
    void switchToMainWindow()
    {
        QWidget* p = parentWidget();
        
        if (p != nullptr) [[likely]]
        {
            p->show();
        }
        
        if (_M_floatingWindow != nullptr) [[likely]]
        {
            _M_floatingWindow->hide();
        }
    }
    
protected:
    
    void paintEvent(QPaintEvent* e) override
    {
        QPainter painter(this);
        painter.fillRect(e->rect(), _M_fbColor.second);
        
        QWidget::paintEvent(e);
    }
    
    void mousePressEvent(QMouseEvent* e) override
    {
        _M_moving = true;
        _M_lastMousePos = e->globalPos();
        QWidget::mousePressEvent(e);
    }
    
    void mouseMoveEvent(QMouseEvent* e) override
    {
        if (_M_moving)
        {
            QPoint movePoint = e->globalPos() - _M_lastMousePos;
            QPoint widgetPos = this->parentWidget()->pos();
            _M_lastMousePos = e->globalPos();
            this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
        }
        QWidget::mouseMoveEvent(e);
    }
    
    void mouseReleaseEvent(QMouseEvent* e) override
    {
        _M_moving = false;
        QWidget::mouseReleaseEvent(e);
    }
    
signals:
    
    void shrinkButtonClicked();
    void topmostButtonClicked(bool isTopmost);
    void closeButtonClicked();
    
private slots:
    
    void onShrinkButtonClicked()
    {
        QWidget* p = parentWidget();
        
        if (p != nullptr) [[likely]]
        {
            p->hide();
        }
        
        _M_floatingWindow->show();
    }
    
    void onTopmostButtonClicked()
    {
        bool isTopmost = this->parentWidget()->windowFlags() & Qt::WindowType::WindowStaysOnTopHint;
            
        if (isTopmost)
        {
            this->parentWidget()->setWindowFlags(
                this->parentWidget()->windowFlags() &~ Qt::WindowType::WindowStaysOnTopHint);
            
            _M_topmostButton->setIcon(QIcon(QPixmap(":/Icon/topmost.svg").scaled(_M_titlebarHeight, _M_titlebarHeight)));
            _M_topmostButton->setToolTip(tr("Topmost"));
        }
        else
        {
            this->parentWidget()->setWindowFlags(
                this->parentWidget()->windowFlags() | Qt::WindowType::WindowStaysOnTopHint);
            
            _M_topmostButton->setIcon(QIcon(QPixmap(":/Icon/topmost2.svg").scaled(_M_titlebarHeight, _M_titlebarHeight)));
            _M_topmostButton->setToolTip(tr("Cancel topmost"));
        }
        this->parentWidget()->show();
        
        emit topmostButtonClicked(isTopmost);
    }
    
    void onCloseButtonClicked()
    {
        this->parentWidget()->close();
        emit closeButtonClicked();
    }
    
private:
    
    QPoint _M_lastMousePos;
    bool _M_moving = false;
    
    int _M_titlebarHeight = 40;
    QPair<QColor, QColor> _M_fbColor;
    
    QLabel* _M_titleLabel;
    
    QSlider* _M_opacitySlider;
    
    QPushButton* _M_shrinkButton;
    QPushButton* _M_topmostButton;
    QPushButton* _M_closeButton;
    
    QWidget* _M_floatingWindow;
};

#endif // TITLEBAR_H
