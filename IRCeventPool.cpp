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

	// if we don't have any, return empty list
	if (this->aEvents.isEmpty()) return QStringList();

	// extract oldest
	const QStringList aOut = this->aEvents.first();
	// remove it from pile
	this->aEvents.removeFirst();

	// give it out
	return aOut;

} // pollEvent


void IRCeventPool::onEvent(const QStringList &aEvent) {

	// add new event to bottom of pile
	this->aEvents.append(aEvent);

	// check size
	int iCount = this->aEvents.count();
	// TODO: use define for this value
	const int IeventMaxLength = 333;
	this->onDebugMessage("EventCount: " + QString::number(iCount));

	// drop if needed, oldest first
	if (IeventMaxLength <= iCount) {
		this->onDebugMessage("OO:More events than requested limit, dropping oldest.");

		this->aEvents.removeFirst();

	}

} // onEvent



}	} // namespace SwissalpS::QtSssSircBot

