#include "IRCclient.h"
#include "IRCcommand.h"

#include "foreign/QtIRC/ircservermessage.h"
#include "foreign/QtIRC/ircreply.h"
#include "foreign/QtIRC/ircerror.h"

#include <QHostInfo>



namespace SwissalpS { namespace QtSssSircBot {



IRCclient::IRCclient(const QString &domainOrIP, const QString &initialNick,
					 quint16 port, const QString &realName,
					 const QString &password, QObject *pParent) :
	QObject(pParent),
	pSocket(0),
	oHost(QHostAddress()),
	uiPort(port),
	sHost(domainOrIP),
	sNick(initialNick),
	sPassword(password),
	sRealName(realName),
	bConnected(false),
	bLoggedIn(false) {

} // construct


IRCclient::~IRCclient() {

	this->disconnect();

} // destruct


void IRCclient::connectEncrypted() {

	// if already got a socket, destroy it
	this->disconnect();

	QHostInfo::lookupHost(this->sHost, this, SLOT(onConnectEncrypted(QHostInfo)));

} // connectEncrypted


void IRCclient::onConnectEncrypted(const QHostInfo &oHI) {

	if (QHostInfo::NoError != oHI.error()) {

		this->debugMessage("Failed to lookup host: " + this->sHost
						   + "\neM: " + oHI.errorString());
		this->quit(-1);

		return;

	} // if error happened

//	const auto addresses = oHI.addresses();
//	for (const QHostAddress &address : addresses) {
//		this->debugMessage("Found Address: " + address.toString());
//	}
	this->oHost = oHI.addresses().first();

	this->pSocket = new QSslSocket(this);
	connect(this->pSocket, SIGNAL(connected()),
			this, SLOT(handleConnected()));

	connect(this->pSocket, SIGNAL(disconnected()),
			this, SLOT(handleDisconnected()));

	connect(this->pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(handleError(QAbstractSocket::SocketError)));

	connect(this->pSocket, SIGNAL(readyRead()),
			this, SLOT(handleReadyRead()));

	this->pSocket->connectToHostEncrypted(this->oHost.toString(),
										  this->uiPort,
										  this->sHost);

} // onConnectEncrypted


void IRCclient::disconnect() {

	if (0 == this->pSocket) return;

	this->onDebugMessage("disconnect");

	QObject::disconnect(this->pSocket);
	this->pSocket->disconnect();
	this->pSocket->deleteLater();
	this->pSocket = 0;

} // disconnect


void IRCclient::handleChannelMessage(IRCServerMessage oSM) {

	const QString sFromNick = oSM.nick();
	const QString sChannel = oSM.parameter(0);
	const QString sMessage = oSM.parameter(1);

	this->onDebugMessage("CM: on: " + sChannel + " from: " + sFromNick
						 + " msg: " + sMessage);
	Q_EMIT this->channelMessage(sChannel, sFromNick, sMessage);

} // handleChannelMessage


void IRCclient::handleDirectMessage(IRCServerMessage oSM) {

	const QString sFromNick = oSM.nick();
	const QString sMessage = oSM.parameter(1);
	this->onDebugMessage("DM: from: " + sFromNick + " msg: " + sMessage);
	Q_EMIT this->directMessage(sFromNick, sMessage);
	this->sendPrivateMessage(sFromNick, sMessage.toUpper());

} // handleDirectMessage


void IRCclient::handleConnected() {

	this->onDebugMessage("connected");

	this->bConnected = true;

/* order of connect messages according to RFC2812
1. Pass message
2. Nick message                 2. Service message
3. User message
*/
	this->sendIRCCommand(IRCcommand::Password, QStringList(this->sPassword));

	this->sendNicknameChangeRequest(this->sNick);

	// RFC2812 3.1.3
	// USER <user> <mode> <unused> <realname>
	this->sendIRCCommand(IRCcommand::User, QStringList() << this->sNick << "0"
						 << "*" << this->sRealName);

	Q_EMIT this->connected(this->oHost.toString());

} // handleConnected


void IRCclient::handleDisconnected() {

	this->onDebugMessage("disconnected");
	Q_EMIT this->quit(0);

} // handleDisconnected


void IRCclient::handleError(QAbstractSocket::SocketError oError) {

	QString sError;

	switch (oError) {
		case QAbstractSocket::ConnectionRefusedError:
			sError = "ConnectionRefusedError";
		break;
		case QAbstractSocket::RemoteHostClosedError:
			sError = "RemoteHostClosedError";
		break;
		case QAbstractSocket::HostNotFoundError:
			sError = "HostNotFoundError";
		break;
		case QAbstractSocket::SocketAccessError:
			sError = "SocketAccessError";
		break;
		case QAbstractSocket::SocketResourceError:
			sError = "SocketResourceError";
		break;
		case QAbstractSocket::SocketTimeoutError:
			sError = "SocketTimeoutError";
		break;
		case QAbstractSocket::DatagramTooLargeError:
			sError = "DatagramTooLargeError";
		break;
		case QAbstractSocket::NetworkError:
			sError = "NetworkError";
		break;
		case QAbstractSocket::AddressInUseError:
			sError = "AddressInUseError";
		break;
		case QAbstractSocket::SocketAddressNotAvailableError:
			sError = "SocketAddressNotAvailableError";
		break;
		case QAbstractSocket::UnsupportedSocketOperationError:
			sError = "UnsupportedSocketOperationError";
		break;
		case QAbstractSocket::UnfinishedSocketOperationError:
			sError = "UnfinishedSocketOperationError";
		break;
		case QAbstractSocket::ProxyAuthenticationRequiredError:
			sError = "ProxyAuthenticationRequiredError";
		break;
		case QAbstractSocket::SslHandshakeFailedError:
			sError = "SslHandshakeFailedError";
		break;
		case QAbstractSocket::ProxyConnectionRefusedError:
			sError = "ProxyConnectionRefusedError";
		break;
		case QAbstractSocket::ProxyConnectionClosedError:
			sError = "ProxyConnectionClosedError";
		break;
		case QAbstractSocket::ProxyConnectionTimeoutError:
			sError = "ProxyConnectionTimeoutError";
		break;
		case QAbstractSocket::ProxyNotFoundError:
			sError = "ProxyNotFoundError";
		break;
		case QAbstractSocket::ProxyProtocolError:
			sError = "ProxyProtocolError";
		break;
		case QAbstractSocket::OperationError:
			sError = "OperationError";
		break;
		case QAbstractSocket::SslInternalError:
			sError = "SslInternalError";
		break;
		case QAbstractSocket::SslInvalidUserDataError:
			sError = "SslInvalidUserDataError";
		break;
		case QAbstractSocket::TemporaryError:
			sError = "TemporaryError";
		break;

		case QAbstractSocket::UnknownSocketError:
		default:
			sError = "UnknownSocketError";
		break;

	} // switch oError

	this->onDebugMessage("KO:" + sError);
	Q_EMIT this->quit(-1);

} // handleError


void IRCclient::handleIncomingLine(const QString &sLine) {

	if (sLine.isEmpty() || !this->bConnected) return;

	Q_EMIT this->rawIncomingLine(sLine);

	IRCServerMessage ircServerMessage(sLine);
	if(ircServerMessage.isNumeric()) {

		switch(ircServerMessage.numericValue()) {

			case IRCReply::Welcome:

				this->bLoggedIn = true;

				this->onDebugMessage("logged in " + this->sNick);

				Q_EMIT this->loggedIn(this->sNick);

			break;

			case IRCError::NicknameInUse:
			case IRCError::NickCollision:

				// If we are already logged in, the user attempted to
				// switch to a username that is already existing.
				// In that case warn him.
				if(this->bLoggedIn) {

					this->onDebugMessage("The nickname is already in use.");

				}
				// Otherwise we are attempting to log in to the server.
				// Change the nick so that we can at least log in.
				else {

					this->sNick += "_";
					this->sendNicknameChangeRequest(this->sNick);

				} // if already logged in or not

			break;

			case IRCError::PasswordMismatch:

				this->onDebugMessage("The password you provided is not correct.");
				Q_EMIT this->quit(-2);

			break;

			case IRCReply::MessageOfTheDayStart:
			case IRCReply::MessageOfTheDay:
			case IRCReply::MessageOfTheDayEnd:
			case IRCError::NoMessageOfTheDay:
				this->onDebugMessage("MOTD");
			break;

			case IRCReply::NoTopic:
			case IRCReply::Topic:
				this->onDebugMessage("TOPIC");
			break;

			case IRCReply::NameReply:

				QString sChannel = ircServerMessage.parameter(2);
				QString sNickList = ircServerMessage.parameter(3);

				Q_EMIT this->nicklist(sChannel, sNickList.split(
										  QRegExp("\\s+"),
										  QString::SkipEmptyParts));

			break;

		} // switch numeric value

	} else {

			QString sCommand = ircServerMessage.command();
			if(IRCcommand::Nick == sCommand) {

				this->onDebugMessage("!!!!!!!!nick changed");
				//handleNicknameChanged(ircServerMessage.nick(), ircServerMessage.parameter(0));

			} else if(IRCcommand::Quit == sCommand) {

				this->onDebugMessage("user quit");
				//handleUserQuit(ircServerMessage.nick(), ircServerMessage.parameter(0));

			} else if(IRCcommand::Join == sCommand) {

				this->onDebugMessage("joined " + ircServerMessage.nick() + " "
									 + ircServerMessage.parameter(0));
				Q_EMIT this->joined(ircServerMessage.nick(),
									ircServerMessage.parameter(0));

			} else if(IRCcommand::Part == sCommand) {

				this->onDebugMessage("got PART " + ircServerMessage.nick()
									 + " " + ircServerMessage.parameter(0)
									 + " " + ircServerMessage.parameter(1));

			} else if(IRCcommand::Mode == sCommand) {

				this->onDebugMessage("mode");

			} else if(IRCcommand::Topic == sCommand) {

				this->onDebugMessage("topic " + ircServerMessage.parameter(0));

			} else if(IRCcommand::Kick == sCommand) {

				this->onDebugMessage("WRITEME: Received kick command.");

			} else if(IRCcommand::Invite == sCommand) {

				this->onDebugMessage("WRITEME: Received invite command.");

			} else if(IRCcommand::PrivateMessage == sCommand) {

				if (ircServerMessage.parameter(0).startsWith("#")) {

					this->handleChannelMessage(ircServerMessage);

				} else if (0 == ircServerMessage.parameter(0).compare(this->sNick)) {

					this->handleDirectMessage(ircServerMessage);

				} else {

					this->onDebugMessage("OO:Unknown PrivMsg format");

				}

			} else if(IRCcommand::Notice == sCommand) {

				this->onDebugMessage("Notice " + ircServerMessage.parameter(1));

			} else if(IRCcommand::Ping == sCommand) {

				this->onDebugMessage("got ping, giving pong");
				this->sendIRCCommand(IRCcommand::Pong,
									 QStringList(ircServerMessage.parameter(0)));// this->sNick));

			} else if(IRCcommand::Error == sCommand) {

				this->onDebugMessage("KO:Error received from host: "
									 + ircServerMessage.parameter(0));

			} else {

				this->onDebugMessage(QString("FIXME: Received unknown reply: %1").arg(sCommand));

			} // if-switch command

		} // if is numeric or not

} // handleIncomingLine


void IRCclient::handleReadyRead() {

	//this->onDebugMessage("readyRead");

	QByteArray sLine;
	do {

		sLine = this->pSocket->readLine();
		if (sLine.size()) {

			const QString sL = QString::fromUtf8(sLine.data());
			this->onDebugMessage("got Line: " + sL);
			this->handleIncomingLine(sL);

		} else break;

	} while(true);

} // handleReadyRead


void IRCclient::reconnect() {

	this->disconnect();

	if (!this->uiPort) return;

	this->connectEncrypted();

} // reconnect


void IRCclient::sendJoin(const QString &sChannel) {

	this->sendIRCCommand(IRCcommand::Join, QStringList(sChannel));

} // sendJoin


void IRCclient::sendIRCCommand(const QString &sCommand,
							   const QStringList &aArguments) {

	QString sLine = sCommand;
	bool bApplyColon = false;
	quint8 uiCount;
	// RFC 2812 2.3 Messages may have max 15 parameters
	quint8 uiMax = qMin(15, qMax(0, aArguments.size()));

	for(uiCount = 0; uiCount < uiMax; uiCount++) {

		bApplyColon = false;
		// Usually all parameters are separated by spaces.
		// The last parameter of the message may contain spaces, it is usually used
		// to transmit messages. In order to parse it correctly, if needs to be prefixed
		// with a colon, so the server knows to ignore all forthcoming spaces and has to treat
		// all remaining characters as a single parameter. If we detect any whitespace in the
		// last argument, prefix it with a colon:
		if ((uiMax - 1u == uiCount)
				&& aArguments.at(uiCount).contains(QRegExp("\\s"))) {

			bApplyColon = true;

		} // if neet to apply colon

		sLine += QString(" %1%2").arg(bApplyColon ? ":" : "").arg(aArguments.at(uiCount));

	} // loop all arguments

	this->sendLine(sLine);

} // sendIRCCommand


void IRCclient::sendLine(const QString &sLine) {

	if(!this->bConnected) return;

	// RFC2812 2.3 Messages SHALL NOT be longer than 512 including CRLF
	QString sOut = sLine.mid(0, 510);

	this->onDebugMessage("sending: " + sOut);

	this->pSocket->write((sOut + "\r\n").toUtf8());

} // sendLine


void IRCclient::sendNicknameChangeRequest(const QString &sNickname) {

	this->sendIRCCommand(IRCcommand::Nick, QStringList(sNickname));

} // sendNicknameChangeRequest


void IRCclient::sendPrivateMessage(const QString &sRecipient,
								   const QString &sMessage) {

	this->sendIRCCommand(IRCcommand::PrivateMessage, QStringList()
						 << sRecipient << sMessage);

} // sendPrivateMessage


void IRCclient::sendQuit() {

	this->sendIRCCommand(IRCcommand::Quit, QStringList());

} // sendQuit



}	} // namespace SwissalpS::QtSssSircBot
