#ifndef STATUSBARLABEL_H
#define STATUSBARLABEL_H

#include <QLabel>
#include <QTimerEvent>
#include <QApplication>
#include <QGuiApplication>
#include <QClipboard>

#include <map>

#include "Logger.h"

class StatusBarLabel final : public QLabel
{
    Q_OBJECT
    
public:
    
    enum class Status : int
    {
        Waiting = 0,
        Copied,
        Replacing,
        Replaced
    };
    
    StatusBarLabel(QWidget* parent = nullptr)
        : QLabel(parent)
    {
        connect(this, &QLabel::linkActivated, this, &StatusBarLabel::on_linkActivated);
    }
    
    void setState(Status __state) noexcept
    {
        _M_state = __state;
        
        _M_resetText();
        
        QString out = _M_map[__state];
        
        if (_M_state != Status::Waiting)
        {
            QString text = QGuiApplication::clipboard()->text();
            QFontMetrics fm(QApplication::font());
            
            out += tr(R"( with [%0] %1)").arg(text.size()).arg(fm.elidedText(text, Qt::ElideMiddle, fm.horizontalAdvance(text, 50), Qt::TextSingleLine));
        }
        
        huanhuan::slog << out;
        
        if (_M_state == Status::Replaced && _M_timerId == -1)
        {
            _M_timerId = startTimer(2000);
        }
    }
    
    Status state() const noexcept
    {
        return _M_state;
    }
    
    void timerEvent(QTimerEvent* event) override
    {
        if (event->timerId() == _M_timerId)
        {
            event->accept();
            
            setState(Status::Waiting);
            
            killTimer(_M_timerId);
            _M_timerId = -1;
        }
        else
        {
            QLabel::timerEvent(event);
        }
    }
    
private:
    
    void _M_resetText()
    {
        setText(QString("%0 %1 %2")
            .arg(R"-(<style type="text/css">a{text-decoration:none; color:%0;}</style>)-")
                .arg(_M_more ? "#FF00FF" : "#00FFFF")
            .arg(_M_map[_M_state])
            .arg(tr(R"-(<a href="%0">%1</a>)-"))
                .arg(_M_more ? "closeMore" : "openMore")
                .arg(_M_more ? "...Less" : "More..."));
    }
    
public slots:
    
    void on_linkActivated(const QString& link)
    {
        if (link == "openMore")
        {
            _M_more = true;
            _M_resetText();
            emit moreOpened();
        }
        else if (link == "closeMore")
        {
            _M_more = false;
            _M_resetText();
            emit moreClosed();
        }
    }
    
signals:
    
    void moreOpened();
    void moreClosed();
    
private:
    
    static inline std::map<Status, QString> _M_map {
        { Status::Waiting, QObject::tr("Waiting") },
        { Status::Copied, QObject::tr("Copied") },
        { Status::Replacing, QObject::tr("Replacing") },
        { Status::Replaced, QObject::tr("Replaced") }
    };
    
    Status _M_state = Status::Waiting;
    
    bool _M_more = false;
    
    int _M_timerId = -1;
};

#endif // STATUSBARLABEL_H
