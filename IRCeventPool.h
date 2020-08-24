#ifndef IRCEVENTPOOL_H
#define IRCEVENTPOOL_H

#include <QObject>
#include <QVector>



namespace SwissalpS { namespace QtSssSircBot {


/// \brief mechanic to hold on to events and hand them out when they are requested
///
/// Connected module(s) may have their own run-loop and need time to process.
/// May be overkill, but maybe useful for some project.
class IRCeventPool : public QObject {

	Q_OBJECT

private:

protected:
	/// holds the events
	QVector<QStringList>aEvents;

public:
	/// constructor
	explicit IRCeventPool(QObject *pParent = nullptr);
	virtual ~IRCeventPool();

	/// get next event, it is removed from IRCeventPool::aEvents
	QStringList pollEvent();

signals:
	/// trickle up debug message system use onDebugMessage()
	/// to trigger with prepended sender id.
	void debugMessage(const QString &sMessage) const;
	/// emitted when an event was added to pool
	void eventAdded() const;
	/// emitted when pool is overflowing
	void eventDropped() const;

public slots:
	/// connect this to event emitter: method to add events to pool.
	virtual void onEvent(const QStringList &aEvent);
	/// signal a message for debugging
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("IRCeventPool:" + sMessage); }

}; // IRCeventPool



}	} // namespace SwissalpS::QtSssSircBot



#endif // IRCEVENTPOOL_H

