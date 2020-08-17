#ifndef SwissalpS_QtSssSircBot_FILECOMMANDINTERFACE_H
#define SwissalpS_QtSssSircBot_FILECOMMANDINTERFACE_H

#include <QObject>
#include <QTimer>



namespace SwissalpS { namespace QtSssSircBot {



class FileCommandInterface : public QObject {

	Q_OBJECT

private:

protected:
	QTimer *pTimer;
	QString sPath;

protected slots:
	virtual void onTimeout();

public:
	explicit FileCommandInterface(QString sPathFile, int iDelay, QObject *pParent = nullptr);
	virtual ~FileCommandInterface();

signals:
	void debugMessage(const QString &sMessage) const;
	void commandEvent(const QStringList &aEvent) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("FileCommandInterface:" + sMessage); }

	virtual void start();
	virtual void stop();

}; // FileCommandInterface



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_FILECOMMANDINTERFACE_H
