#include "DelayedCallback.h"

#include <QTimer>



namespace SwissalpS { namespace QtSssSircBot {



DelayedCallback::DelayedCallback(const QString &sID, const int &iDuration,
								 LuaController *pLC,
								 QObject *pParent) :
	QObject(pParent),
	sIdent(sID),
	iInterval(iDuration),
	pLuaController(pLC) {

} // construct


DelayedCallback::~DelayedCallback() {

} // dealloc


void DelayedCallback::start() {

	QTimer::singleShot(this->iInterval, this, SLOT(onDone()));

} // start


void DelayedCallback::onDone() {

	this->pLuaController->callDelayedCallback(this->sIdent);
	this->deleteLater();

} // onDone



}	} // namespace SwissalpS::QtSssSircBot

