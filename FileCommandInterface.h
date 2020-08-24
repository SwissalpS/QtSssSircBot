#ifndef SwissalpS_QtSssSircBot_FILECOMMANDINTERFACE_H
#define SwissalpS_QtSssSircBot_FILECOMMANDINTERFACE_H

#include <QObject>
#include <QTimer>



namespace SwissalpS { namespace QtSssSircBot {



/// \brief Observe a file for commands
///
/// Checks at intervals if a file exists. If it does, reads it's contents and
/// deletes the file.
/// Every line represents a command.
/// \rst
/// `Read more about this <../general/backdoor.html>`_
/// \endrst
class FileCommandInterface : public QObject {

	Q_OBJECT

private:

protected:
	/// QTimer instance that keeps checks happening at intervals
	QTimer *pTimer;
	/// the path to the file to observe
	QString sPath;

protected slots:
	/// called by FileCommandInterface::pTimer whenever interval has passed
	virtual void onTimeout();

public:
	/// constructor
	/// \param sPathFile path to file to observe
	/// \param iDelay interval between checks in milliseconds
	/// \param pParent parent QObject
	explicit FileCommandInterface(QString sPathFile, int iDelay, QObject *pParent = nullptr);
	virtual ~FileCommandInterface();

signals:
	/// trickle up debug message system use onDebugMessage()
	/// to trigger with prepended sender id.
	void debugMessage(const QString &sMessage) const;
	/// emitted when a command is read from file
	void commandEvent(const QStringList &aEvent) const;

public slots:
	/// signal a message for debugging
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("FileCommandInterface:" + sMessage); }

	/// start watching file
	virtual void start();
	/// stop watching file
	virtual void stop();

}; // FileCommandInterface



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_FILECOMMANDINTERFACE_H

