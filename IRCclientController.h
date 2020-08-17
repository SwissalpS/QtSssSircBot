#ifndef IRCCLIENTCONTROLLER_H
#define IRCCLIENTCONTROLLER_H

#include <QJsonObject>
#include <QObject>

#include "IRCclient.h"



namespace SwissalpS { namespace QtSssSircBot {



class IRCclientController : public QObject {

	Q_OBJECT

private:

protected:
	QJsonObject oJo;
	IRCclient *pClient;
	int iChannelIndex; // used to join channels in a paced manner
	int iLineIndex; // used to do pre and post login lines in a paced manner

protected slots:
	void onJoinNextChannel();
	void onPostLoginNextLine();
	void onPreLoginNextLine();

public:
	explicit IRCclientController(const QJsonObject &oConfig,
								 QObject *pParent = nullptr);
	virtual ~IRCclientController();

	QString getConnectionID();
	void init();
	void start();

signals:
	void debugMessage(const QString &sMessage) const;
	void newEvent(const QStringList &aEvent) const;

public slots:
	void onAbort(const qint16 &iR);
	void onCommandEvent(const QStringList &aEvent);
	void onConnected(const QString &sIP);
	void onChannelMessage(const QString &sChannel, const QString &sFromNick,
						  const QString &sMessage);

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("IRCclientController:" + sMessage); }

	void onDirectMessage(const QString &sFromNick,
						 const QString &sMessage);

	void onDisconnected();
	void onJoined(const QString &sNick, const QString &sChannel);
	void onLoggedIn(const QString &sNick);
	void onNicklist(const QString &sChannel, const QStringList &aNicks);
	void onPing(const QString &sMessage);
	void onQuit(const QString &sNick, const QString &sMessage);
	void onRawIncomingLine(const QString &sLine);
	void onRawOutgoingLine(const QString &sLine);

	friend class IRCclient;

}; // IRCclientController



}	} // namespace SwissalpS::QtSssSircBot



#endif // IRCCLIENTCONTROLLER_H
