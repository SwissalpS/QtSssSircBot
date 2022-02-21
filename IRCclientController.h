#ifndef IRCCLIENTCONTROLLER_H
#define IRCCLIENTCONTROLLER_H

#include <QJsonObject>
#include <QObject>

#include "IRCclient.h"



namespace SwissalpS { namespace QtSssSircBot {



/// \brief Manages an IRCclient instance
///
/// May take over more functions from IRCclient in the future.
/// You can extend this class to add logic and other functions that may seem
/// fit for your application.
class IRCclientController : public QObject {

	Q_OBJECT

private:

protected:
	/// the connection configuration is held in this QJsonObject
	QJsonObject oJo;
	/// this is the managed IRCclient instance
	IRCclient *pClient;
	/// used to join channels in a paced manner
	int iChannelIndex;
	/// used to do pre and post login lines in a paced manner
	int iLineIndex;

protected slots:
	/// mechanism to join channels with pauses in between
	void onJoinNextChannel();
	/// mechanism to send post-login lines with pauses in between
	void onPostLoginNextLine();
	/// mechanism to send pre-login lines with pauses in between
	void onPreLoginNextLine();

public:
	/// constructor
	/// \param oConfig connection configuration
	/// \param pParent the parent QObject
	explicit IRCclientController(const QJsonObject &oConfig,
								 QObject *pParent = nullptr);
	virtual ~IRCclientController();

	QString getConnectionID();
	/// instantiate IRCclient and connect signals
	void init();
	/// have IRCclient start the connection process
	void start();

signals:
	/// trickle up debug message system use onDebugMessage()
	/// to trigger with prepended sender id.
	void debugMessage(const QString &sMessage) const;
	/// not used in this branch.
	/// is emitted for e.g. IRCeventPool to catch and add to pool
	void newEvent(const QStringList &aEvent) const;

public slots:
	void onAbort(const qint16 &iR);
	void onCommandEvent(const QStringList &aEvent);
	void onConnected(const QString &sIP);
	void onChannelMessage(const QString &sChannel, const QString &sFromNick,
						  const QString &sMessage);

	/// signal a message for debugging
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

