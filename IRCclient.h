#ifndef SwissalpS_QtSssSircBot_IRCCLIENT_H
#define SwissalpS_QtSssSircBot_IRCCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QHostInfo>

class IRCServerMessage;



namespace SwissalpS { namespace QtSssSircBot {



/// \brief Manages a secure connection to an IRC host
///
/// Originally I had not designed for there to be an IRCclientController too.
/// It could be that some functionality will be moved there, so that this class
/// concerns itself less reactions.
class IRCclient : public QObject {

	Q_OBJECT

protected:
	/// managed socket instance
	QSslSocket *pSocket;
	/// target host address
	QHostAddress oHost;
	/// target port
	quint16 uiPort;
	/// target host address as string
	QString sHost;
	/// current nick
	QString sNick;
	/// list of optional nicks
	QStringList aNicks;
	/// index of current nick in aNicks unless already appending '_'s
	int iNick;
	/// the password to use
	QString sPassword;
	/// real name on IRC
	QString sRealName;
	/// are we connected?
	bool bConnected;
	/// are we logged in?
	bool bLoggedIn;

	/// deal with channel message
	void handleChannelMessage(IRCServerMessage oSM);
	/// deal with direct message
	void handleDirectMessage(IRCServerMessage oSM);

protected slots:
	/// triggered by pSocket when connection is established
	virtual void handleConnected();
	/// triggered by pSocket when connection is broken
	virtual void handleDisconnected();
	/// triggered by pSocket when an error occured
	virtual void handleError(QAbstractSocket::SocketError oError);
	/// called from within handleReadyRead() when a line has been read
	virtual void handleIncomingLine(const QString &sLine);
	/// triggered by pSocket when there is data to be read
	virtual void handleReadyRead();
	/// triggered by QHostInfo::lookupHost which is started in connectEncrypted()
	virtual void onConnectEncrypted(const QHostInfo &oHI);

public:
	/// constructor
	/// \param domainOrIP host address to connect to
	/// \param aNicks nicks to use
	/// \param port the port to connect on
	/// \param realName real name to use on IRC
	/// \param password the password for network
	/// \param pParent parent QObject pointer
	explicit IRCclient(const QString &domainOrIP, const QStringList &aNicks,
					   quint16 port = 6697u, const QString &realName = "",
					   const QString &password = "", QObject *pParent = 0);
	virtual ~IRCclient();

	/// helper function to ensure sent lines are in limits regarding length
	/// used by sendPrivateMessage()
	static QStringList wordWrap(const QString &sMessage, const quint8 &ubWidth);

public slots:
	/// triggered to start connection process
	virtual void connectEncrypted();
	/// disconnects pSocket
	virtual void disconnectSocket();
	/// mechanism to login
	virtual void doLogin();

	/// signal a message for debugging
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("Ic:" + sMessage); }

	/// disconnect socket if connected, then re-connect to host.
	virtual void reconnect();

	/// send an IRC command formated according to standards.
	/// \param sCommand the command
	/// \param aArguments the arguments
	virtual void sendIRCCommand(const QString &sCommand,
						const QStringList &aArguments);
	/// join a channel, channel names generally begin with #
	virtual void sendJoin(const QString &sChannel);
	/// send raw IRC formatted line
	virtual void sendLine(const QString &sLine);
	/// send NICK command to request another nick
	virtual void sendNicknameChangeRequest(const QString &sNickname);
	/// send a PART command to leave channels
	/// \param sChannels comma separated list of channels to part from.
	///   No spaces, just commas
	/// \param sMessage optional part-message
	virtual void sendPart(const QString &sChannels, const QString &sMessage = "");
	/// send a direct or channel message
	/// \param sRecipient nick or channel message is for
	/// \param sMessage the message
	virtual void sendPrivateMessage(const QString &sRecipient,
							const QString &sMessage);

	/// send a QUIT command and optional message. Most hosts will disconnect.
	virtual void sendQuit(const QString &sMessage = "");

signals:
	/// emitted when needs to abort: TODO: define return value - = don't restart + = restart in x seconds ?
	/// may get replaced by errorSocket and errorIRC signals
	void abort(const qint16 iR) const;
	/// emitted when socket connects
	void connected(const QString &sIP) const;
	/// emitted when privmsg is received on channel
	/// connect to this if you want your bot to monitor channels, gather statistics
	/// listen for global commands, etc.
	void channelMessage(const QString &sChannel, const QString &sFromNick,
						const QString &sMessage) const;
	/// emitted when privmsg is received from user
	/// connect to this for listening for direct commands
	void directMessage(const QString &sFromNick,
					   const QString &sMessage) const;
	/// emitted when socket connection disconnected
	/// currently followed by a quit-signal
	void disconnected() const;
	/// general debug messages, mute output or don't connect for release
	void debugMessage(const QString &sMessage) const;
	/// emitted when ... has joined a channel
	void joined(const QString &sNick, const QString &sChannel) const;
	/// emitted when this client has logged on to server
	void loggedIn(const QString &sNick) const;
	/// emitted when receiving list of nicks for channel
	void nicklist(const QString &sChannel, const QStringList &aNicks) const;
	/// emitted whenever a server sends ping (after pong is sent back)
	void ping(const QString &sMessage) const;
	/// emitted whenever a user has quit
	void quit(const QString &sNick, const QString &sMessage) const;
	/// connect to this signal to debug all incoming lines as received
	void rawIncomingLine(const QString &sLine) const;
	/// connect to this signal to debug all outgoing lines as sent
	void rawOutgoingLine(const QString &sLine) const;

}; // IRCclient



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_IRCCLIENT_H

