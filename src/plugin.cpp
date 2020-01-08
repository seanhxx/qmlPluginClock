#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <qbasictimer.h>
#include <qcoreapplication.h>

// Implements a "TimeModel" class with hour and minute properties
// that change on-the-minute yet efficiently sleep the rest
// of the time.

class MinuteTimer : public QObject
{
    Q_OBJECT
public:
    MinuteTimer(QObject *parent) : QObject(parent)
    {
    }

    void start()
    {
        if (!timer.isActive()) {
            time = QTime::currentTime();
            timer.start(60000-time.second()*1000, this);
        }
    }

    void stop()
    {
        timer.stop();
    }

    int hour() const { return time.hour(); }
    int minute() const { return time.minute(); }

signals:
    void timeChanged();

protected:
    void timerEvent(QTimerEvent *) override
    {
        QTime now = QTime::currentTime();
        if (now.second() == 59 && now.minute() == time.minute() && now.hour() == time.hour()) {
            // just missed time tick over, force it, wait extra 0.5 seconds
            time = time.addSecs(60);
            timer.start(60500, this);
        } else {
            time = now;
            timer.start(60000-time.second()*1000, this);
        }
        emit timeChanged();
    }

private:
    QTime time;
    QBasicTimer timer;
};

//![0]
class TimeModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int hour READ hour NOTIFY timeChanged)
    Q_PROPERTY(int minute READ minute NOTIFY timeChanged)
//![0]

public:
    TimeModel(QObject *parent=nullptr) : QObject(parent)
    {
        if (++instances == 1) {
            if (!timer)
                timer = new MinuteTimer(QCoreApplication::instance());
            connect(timer, &MinuteTimer::timeChanged, this, &TimeModel::timeChanged);
            timer->start();
        }
    }

    ~TimeModel() override
    {
        if (--instances == 0) {
            timer->stop();
        }
    }

    int minute() const { return timer->minute(); }
    int hour() const { return timer->hour(); }

signals:
    void timeChanged();

private:
    QTime t;
    static MinuteTimer *timer;
    static int instances;
};

int TimeModel::instances=0;
MinuteTimer *TimeModel::timer=nullptr;

//![plugin]
class QExampleQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QLatin1String("TimeExample"));
        qmlRegisterType<TimeModel>(uri, 1, 0, "Time");
    }
};
//![plugin]

#include "plugin.moc"
