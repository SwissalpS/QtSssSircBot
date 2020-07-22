#include "IRCeventPool.h"



namespace SwissalpS { namespace QtSssSircBot {



IRCeventPool::IRCeventPool(QObject *pParent) :
	QObject(pParent) {

	//this->aEvents = QVector<const QStringList>();
	this->aEvents.clear();

} // construct


IRCeventPool::~IRCeventPool() {

	this->aEvents.clear();

} // dealloc


QStringList IRCeventPool::pollEvent() {

	if (this->aEvents.isEmpty()) return QStringList();

	const QStringList aOut = this->aEvents.first();
	this->aEvents.removeFirst();

	return aOut;

} // pollEvent


void IRCeventPool::onEvent(const QStringList &aEvent) {

	this->aEvents.append(aEvent);
	int iCount = this->aEvents.count();
	// TODO: use define for this value
	const int IeventMaxLength = 333;

	this->onDebugMessage("EventCount: " + QString::number(iCount));
	if (IeventMaxLength <= iCount) {
		this->onDebugMessage("OO:More events than requested limit, dropping oldest.");
		this->aEvents.removeFirst();
	}

} // onEvent



}	} // namespace SwissalpS::QtSssSircBot

