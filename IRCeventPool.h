#ifndef IRCEVENTPOOL_H
#define IRCEVENTPOOL_H

#include <QObject>



namespace SwissalpS { namespace QtSssSircBot {



class IRCeventPool : public QObject {

	Q_OBJECT

private:

protected:
	QVector<const QStringList>aEvents;

public:
	explicit IRCeventPool(QObject *pParent = nullptr);
	virtual ~IRCeventPool();

	QStringList pollEvent();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	virtual void onEvent(const QStringList &aEvent);
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("IRCeventPool:" + sMessage); }

}; // IRCeventPool



}	} // namespace SwissalpS::QtSssSircBot



#endif // IRCEVENTPOOL_H
