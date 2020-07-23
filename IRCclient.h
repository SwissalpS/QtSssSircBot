#ifndef SwissalpS_QtSssSircBot_IRCCLIENT_H
#define SwissalpS_QtSssSircBot_IRCCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QHostInfo>

class IRCServerMessage;



namespace SwissalpS { namespace QtSssSircBot {



class IRCclient : public QObject {

	Q_OBJECT

protected:
	QSslSocket *pSocket;
	QHostAddress oHost;
	quint16 uiPort;
	QString sHost;
	QString sNick;
	QString sPassword;
	QString sRealName;
	bool bConnected;
	bool bLoggedIn;

	// send raw IRC formatted line
	void sendLine(const QString &sLine);
	void handleChannelMessage(IRCServerMessage oSM);
	void handleDirectMessage(IRCServerMessage oSM);

protected slots:
	virtual void handleConnected();
	virtual void handleDisconnected();
	virtual void handleError(QAbstractSocket::SocketError oError);
	virtual void handleIncomingLine(const QString &sLine);
	virtual void handleReadyRead();
	virtual void onConnectEncrypted(const QHostInfo &oHI);

public:
	explicit IRCclient(const QString &domainOrIP, const QString &initialNick,
					   quint16 port = 6697u, const QString &realName = "",
					   const QString &password = "", QObject *pParent = 0);
	virtual ~IRCclient();

	static QStringList wordWrap(const QString &sMessage, const quint8 &ubWidth);

public slots:
	virtual void connectEncrypted();
	virtual void disconnect();

	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Ic:" + sMessage); }

	virtual void reconnect();

	void sendIRCCommand(const QString &sCommand,
						const QStringList &aArguments);
	// join a channel, channel names generally begin with #
	virtual void sendJoin(const QString &sChannel);
	void sendNicknameChangeRequest(const QString &sNickname);
	void sendPrivateMessage(const QString &sRecipient,
							const QString &sMessage);

	virtual void sendQuit();

signals:
	// emited when needs to abort: TODO: define return value - = don't restart + = restart in x seconds ?
	// may get replaced by errorSocket and errorIRC signals
	void abort(const qint16 iR) const;
	// emited when socket connects
	void connected(const QString &sIP) const;
	// emited when privmsg is received on channel
	// connect to this if you want your bot to monitor channels, gather statistics
	// listen for global commands, etc.
	void channelMessage(const QString &sChannel, const QString &sFromNick,
						const QString &sMessage) const;
	// emited when privmsg is received from user
	// connect to this for listening for direct commands
	void directMessage(const QString &sFromNick,
					   const QString &sMessage) const;
	// emited when socket connection disconnected
	// currently followed by a quit-signal
	void disconnected() const;
	// general debug messages, mute output or don't connect for release
	void debugMessage(const QString &sMessage) const;
	// emited when ... has joined a channel
	void joined(const QString &sNick, const QString &sChannel) const;
	// emited when this client has logged on to server
	void loggedIn(const QString &sNick) const;
	// emited when receiving list of nicks for channel
	void nicklist(const QString &sChannel, const QStringList &aNicks) const;
	// emited whenever a server sends ping (after pong is sent back)
	void ping(const QString &sMessage) const;
	// emited whenever a user has quit
	void quit(const QString &sNick, const QString &sMessage) const;
	// connect to this signal to debug all incoming lines as received
	void rawIncomingLine(const QString &sLine) const;
	// connect to this signal to debug all outgoing lines as sent
	void rawOutgoingLine(const QString &sLine) const;

}; // IRCclient



}	} // namespace SwissalpS::QtSssSircBot



#endif // IRCCLIENT_H
