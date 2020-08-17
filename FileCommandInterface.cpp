#include "FileCommandInterface.h"
#include "CommandEventCodes.h"

#include <QFile>



namespace SwissalpS { namespace QtSssSircBot {



FileCommandInterface::FileCommandInterface(QString sPathFile, int iDelay,
										   QObject *pParent) :
	QObject(pParent),
	sPath(sPathFile) {

	this->pTimer = new QTimer(this);

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onTimeout()));

	this->pTimer->setInterval(iDelay);

} // construct


FileCommandInterface::~FileCommandInterface() {

	if (this->pTimer) {

		this->pTimer->stop();
		this->pTimer->deleteLater();
		this->pTimer = nullptr;

	}

} // dealloc


void FileCommandInterface::onTimeout() {

	QFile oFile(this->sPath);

	if (!oFile.isReadable()) return;

	if (!oFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

		this->onDebugMessage("couldn't open command file: " + this->sPath);
		return;

	}

	QStringList aEvent;
	QStringList aParts;
	QByteArray aLine;
	QString sLine;
	QString sConnectionID;
	QString sCommand;
	QString sMessage;
	quint8 ubChar;
	int iCount;

	while (!oFile.atEnd()) {

		aEvent.clear();

		aLine = oFile.readLine();
		sLine = QString(aLine);
		if (sLine.isEmpty()) continue;

		aParts = sLine.split(" ", QString::SkipEmptyParts, Qt::CaseInsensitive);
		iCount = aParts.count();
		if (3 > iCount) continue;

		sConnectionID = aParts.takeFirst();
		sCommand = aParts.takeFirst();
		if (1 != sCommand.count()) continue;

		ubChar = sCommand.at(0).unicode();
		sMessage = aParts.join(" ");

		switch (ubChar) {
			case CommandEventCodes::Abort: // 'a'
				// does not apply to this version
			break;
			case CommandEventCodes::ChannelMessage: // 'C'
				if (4 <= iCount) {
					aEvent << sConnectionID << sCommand << aParts.takeFirst();
					aEvent << aParts.join(" ");
				}
			break;
			case CommandEventCodes::DirectMessage: // 'D'
				if (4 <= iCount) {
					aEvent << sConnectionID << sCommand << aParts.takeFirst();
					aEvent << aParts.join(" ");
				}
			break;
			case CommandEventCodes::Exit: // 'e'
				aEvent << sConnectionID << sCommand;
			break;
			case CommandEventCodes::IRCcommand: // 'I'
				// maybe later, it's tricky
			break;
			case CommandEventCodes::Joined: // 'J'
				// send JOIN
				aEvent << sConnectionID << sCommand << sMessage;
			break;
			case CommandEventCodes::LoggedIn: // 'L'
				// ignore
			break;
			case CommandEventCodes::NickList: // 'N'
				// request nick change
				aEvent << sConnectionID << sCommand << aParts.takeFirst();
				//aEvent << sConnectionID << sCommand << sMessage;
			break;
			case CommandEventCodes::Part: // '-'
				aEvent << sConnectionID << sCommand << sMessage;
			break;
			case CommandEventCodes::Ping: // 'P'
				// ignore
			break;
			case CommandEventCodes::Quit: // 'Q'
				aEvent << sConnectionID << sCommand << sMessage;
			break;
			case CommandEventCodes::Connected: // 'c'
				aEvent << sConnectionID << sCommand;
			break;
			case CommandEventCodes::Disconnected: // 'd'
				aEvent << sConnectionID << sCommand;
			break;
			case CommandEventCodes::RawIn: // '<'
				// maybe redirect to sendLine?
			break;
			case CommandEventCodes::RawOut: // '>'
				// ignore
			break;
			case CommandEventCodes::ReloadConnections: // 'r'
				aEvent << sConnectionID << sCommand;
			break;
			default:
				this->onDebugMessage("KO:unknown command: >" + sCommand
									 + "< from file interface.");
			break;
		} // switch ubChar

		if (!aEvent.isEmpty()) Q_EMIT this->commandEvent(aEvent);

	} // loop each line of file

	oFile.close();
	if (!oFile.remove()) {
		this->onDebugMessage("KO:could not remove command file: " + this->sPath);
	}

} // onTimeout


void FileCommandInterface::start() {

	if (!this->pTimer) return;

	this->pTimer->start();

} // start


void FileCommandInterface::stop() {

	if (!this->pTimer) return;

	this->pTimer->stop();

} // stop



}	} // namespace SwissalpS::QtSssSircBot

