#ifndef SwissalpS_QtSssSircBot_DELAYEDCALLBACK_H
#define SwissalpS_QtSssSircBot_DELAYEDCALLBACK_H

#include "LuaController.h"

#include <QObject>



namespace SwissalpS { namespace QtSssSircBot {



class DelayedCallback : public QObject {

	Q_OBJECT

private:

protected:
	QString sIdent;
	int iInterval;
	LuaController *pLuaController;

public:
	explicit DelayedCallback(const QString &sID, const int &iDuration,
							 LuaController *pLC, QObject *pParent = nullptr);
	virtual ~DelayedCallback();

	void start();

signals:

public slots:
	void onDone();

}; // DelayedCallback



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_DELAYEDCALLBACK_H
