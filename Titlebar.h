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
    explicit TitleBar(QWidget* parent, QString title = QString())
        : QWidget(parent)
        , fbColor(QColor(0xff, 0xff, 0xff), QColor(0x33, 0x33, 0x33))
        , hLayout(new QHBoxLayout(this))
        , titleLabel(new QLabel(this))
        , opacitySlider(new QSlider(Qt::Orientation::Horizontal, this))
        , topmostButton(new QPushButton(this))
        , closeButton(new QPushButton(this))
    {
        if (parent)
        {
            connect(parent, &QWidget::windowTitleChanged, this, &TitleBar::setTitle);
            parent->layout()->setMenuBar(this);
            parent->installEventFilter(this);
        }
        
        this->setAttribute(Qt::WA_TranslucentBackground);
        this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        this->setFixedHeight(titlebarHeight);
        
        titleLabel->setText(title.isEmpty() && parent != nullptr ? parent->windowTitle() : title);
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignVCenter);
        titleLabel->setMinimumHeight(titlebarHeight);
        titleLabel->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        QPalette palette;
        palette.setColor(QPalette::WindowText, fbColor.first);
        titleLabel->setPalette(palette);
        titleLabel->setStyleSheet(R"(background-color: transparent; font-weight: 700;)");
        
        opacitySlider->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        opacitySlider->setMinimum(30);
        opacitySlider->setMaximum(100);
        opacitySlider->setValue(100);
        opacitySlider->setStyleSheet(R"(background-color: transparent)");
        
        if (parent)
        connect(opacitySlider, &QSlider::sliderMoved, this, [parent](int value)
        {
            parent->setWindowOpacity(double(value) / 100);
        });
        
        topmostButton->setFixedSize(titlebarHeight, titlebarHeight);
        closeButton->setFixedSize(titlebarHeight, titlebarHeight);
        
        opacitySlider->setToolTip(tr("Opacity Slider"));
        topmostButton->setToolTip(tr("Topmost"));
        closeButton->setToolTip(tr("Close"));
        
        topmostButton->setIcon(QIcon(QPixmap(":/Icon/topmost.svg").scaled(titlebarHeight, titlebarHeight)));
        closeButton->setIcon(QIcon(QPixmap(":/Icon/exit.svg").scaled(titlebarHeight, titlebarHeight)));
        
        topmostButton->setFlat(true);
        closeButton->setFlat(true);
        
        connect(topmostButton, &QPushButton::clicked, this, &TitleBar::onTopmostButtonClicked);
        connect(closeButton, &QPushButton::clicked, this, &TitleBar::onCloseButtonClicked);
        
        hLayout->addWidget(titleLabel);
        
        hLayout->addWidget(opacitySlider);
        
        hLayout->addWidget(topmostButton);
        hLayout->addWidget(closeButton);
        
        hLayout->setSpacing(5);
        hLayout->setContentsMargins(5, 0, 0, 0);
        
        this->setLayout(hLayout);
        this->setStyleSheet(R"(border: none;)");
    }
    
public slots:
    void setTitle(const QString& title)
    {
        titleLabel->setText(title);
    }
    
public:
    QString title() const
    {
        return titleLabel->text();
    }
    
    void setColor(const QPair<QColor, QColor>& color)
    {
        fbColor = color;
        update();
    }
    void setForegroundColor(const QColor& foreground)
    {
        fbColor.first = foreground;
        update();
    }
    void setBackgroundColor(const QColor& background)
    {
        fbColor.second = background;
        update();
    }
    QPair<QColor, QColor> color() const
    {
        return fbColor;
    }
    
private:
    void paintEvent(QPaintEvent* e) override
    {
        QPainter painter(this);
        painter.fillRect(e->rect(), fbColor.second);
        
        QWidget::paintEvent(e);
    }
    void mousePressEvent(QMouseEvent* e) override
    {
        moving = true;
        movingStart = e->globalPos();
        QWidget::mousePressEvent(e);
    }
    void mouseMoveEvent(QMouseEvent* e) override
    {
        if (moving)
        {
            QPoint movePoint = e->globalPos() - movingStart;
            QPoint widgetPos = this->parentWidget()->pos();
            movingStart = e->globalPos();
            this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
        }
        QWidget::mouseMoveEvent(e);
    }
    void mouseReleaseEvent(QMouseEvent* e) override
    {
        moving = false;
        QWidget::mouseReleaseEvent(e);
    }
    
signals:
    void iconClicked();
    void topmostButtonClicked(bool isTopmost);
    void closeButtonClicked();
    
private slots:
    void onTopmostButtonClicked()
    {
        bool isTopmost = this->parentWidget()->windowFlags() & Qt::WindowType::WindowStaysOnTopHint;
            
        if (isTopmost)
        {
            this->parentWidget()->setWindowFlags(
                this->parentWidget()->windowFlags() &~ Qt::WindowType::WindowStaysOnTopHint);
            
            topmostButton->setIcon(QIcon(QPixmap(":/Icon/topmost.svg").scaled(titlebarHeight, titlebarHeight)));
            topmostButton->setToolTip(tr("Topmost"));
        }
        else
        {
            this->parentWidget()->setWindowFlags(
                this->parentWidget()->windowFlags() | Qt::WindowType::WindowStaysOnTopHint);
            
            topmostButton->setIcon(QIcon(QPixmap(":/Icon/topmost2.svg").scaled(titlebarHeight, titlebarHeight)));
            topmostButton->setToolTip(tr("Cancel topmost"));
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
    bool moving = false;
    QPoint movingStart;
    
    int titlebarHeight = 40;
    QPair<QColor, QColor> fbColor;
    
    QHBoxLayout* hLayout;
    
    QLabel* titleLabel;
    
    QSlider* opacitySlider;
    
    QPushButton* topmostButton;
    QPushButton* closeButton;
};

#endif // TITLEBAR_H
