#ifndef SwissalpS_QtSssSircBot_APPSETTINGS_H
#define SwissalpS_QtSssSircBot_APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>



namespace SwissalpS { namespace QtSssSircBot {



/// \brief Singleton class to manage application settings.
///
/// Use pAppSettings() to get hold of pointer to shared instance.
/// Hold on to that pointer for performance reasons.
/// \rst
/// `Read more about Configuration <../general/configuration.html>`_
/// \endrst
class AppSettings : public QObject {

	Q_OBJECT
	Q_DISABLE_COPY(AppSettings)

private:
	QString sPathDataBase;
	QString sPathMusicBase;
	QSettings *pSettings;

	static AppSettings *pSingelton;

	// keep this private as we want only one instance
	// read; http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	explicit AppSettings(QObject *pParent = 0);

	void copyResources(QString sPathSource, QString sPathTarget, bool bExe = false);
	void setupDirs(QString sPathBase);

protected:
	/// contains overrides from CLI arguments
	QJsonObject oJo;
	/// has init run
	bool bInitDone;
	/// parse the command line arguments
	void initOverrides();
	/// figure out where to live and work
	void initPaths();

public:
	/// @name Settings Keys
	/// key names of fields in Settings.ini and throughout application
	/// \rst
	/// `read more in <../general/configuration.html>`_
	/// \endrst
	///@{

	/// how many milliseconds to wait between checking backdoor file
	static const QString sSettingBackdoorIntervalMS;
	/// which file to whatch for backdoor commands
	static const QString sSettingBackdoorPathFile;
	/// where to get connection paramaters from. Path to JSON file
	static const QString sSettingIRCconfig;
	/// comma separated list of channels to join after login
	static const QString sSettingIRCremoteChannels;
	/// IP or FQDN to connect to. e.g. irc.freenode.net
	static const QString sSettingIRCremoteHost;
	/// comma separated list of nicks to use. In order of priority
	/// if first nick can't be used, try second ... finally
	/// adds underscores to nick
	/// RFC says nicks should be max 8 characters long, but most networks
	/// support longer nicks.
	static const QString sSettingIRCremoteNicks;
	/// network password, may be empty ""
	static const QString sSettingIRCremotePassword;
	/// port to connect to
	static const QString sSettingIRCremotePort;
	/// real name may contain spaces
	static const QString sSettingIRCremoteRealname;
	/// where application's PID is written to
	static const QString sSettingPIDpathFile;
#ifdef SssS_USE_GUI
	static const QString sSettingUseGUI;
#endif
	///@}

	/// @name Default Settings
	/// Default values for fields in Settings.ini
	///@{

	/// default value for AppSettings::sSettingBackdoorIntervalMS
	static const quint32 sSettingBackdoorIntervalMSdefault;
	/// default value for AppSettings::sSettingBackdoorPathFile
	static const QString sSettingBackdoorPathFileDefault;
	/// default value for AppSettings::sSettingIRCconfig
	static const QString sSettingIRCconfigDefault;
	/// default value for AppSettings::sSettingIRCremoteChannels
	static const QString sSettingIRCremoteChannelsDefault;
	/// default value for AppSettings::sSettingIRCremoteHost
	static const QString sSettingIRCremoteHostDefault;
	/// default value for AppSettings::sSettingIRCremoteNicks
	static const QString sSettingIRCremoteNickDefault;
	/// default value for AppSettings::sSettingIRCremotePassword
	static const QString sSettingIRCremotePasswordDefault;
	/// default value for AppSettings::sSettingIRCremotePort
	static const quint16 sSettingIRCremotePortDefault;
	/// default value for AppSettings::sSettingIRCremoteRealname
	static const QString sSettingIRCremoteRealnameDefault;
	/// default value for AppSettings::sSettingPIDpathFile
	static const QString sSettingPIDpathFileDefault;
#ifdef SssS_USE_GUI
	/// default value for AppSettings::sSettingUseGUI
	static const bool sSettingUseGUIdefault;
#endif
	///@}

	/// destroy singelton.
	/// no need to call this in most cases
	static void drop();
	/// public access to singelton instance.
	/// keep hold to your pointer for speed.
	static AppSettings *pAppSettings();
	virtual ~AppSettings();

	/// get a value for given key
	QVariant get(const QString sKey) const;

	/// the read config file or an empty JSON-array
	QJsonArray getConfigs() const;

	/// get the channels from Settings.ini as list of strings
	QStringList getChannels() const;
	/// set the channels string from given list of strings
	void setChannels(const QStringList aChannels);

	/// path to backdoor file
	QString getPathBackdoor() const;

	/// path to where application works in
	QString getPathData() const;

	/// path to system's Music directory
	/// not used in base (yet). It's left in to provide a quick way to add
	/// another path to your application for some quick tests :D
	/// Also maybe you want your application to play alerts from somewhere.
	QString getPathMusic() const;
	/// set another path to use for music/media
	void setPathMusic(const QString &sNewPath);

	/// get pointer to QSettings instance being used
	QSettings *getSettings() const;
	/// use another QSettings instance
	void setSettings(QSettings *pNewSettings);

	/// check if a value has been overriden by command line argument
	inline bool isOverridden(const QString sKey) { return this->oJo.contains(sKey); }

	/// \brief initializes AppSettings instance.
	///
	///  - calls AppSettings::initPaths()
	///  - calls AppSettings::setupDirs() which sets up a directory and file
	///    tree in the working directory, AppSettings::sPathDataBase. If there
	///    are any missing files, these are created from copies stored in
	///    QtSssSircBot.qrc
	///  - instantiates and prepares AppSettings::pSettings making sure all
	///    values are set and written to Settings.ini.
	///  - calls AppSettings::initOverrides()
	void init();

signals:
	/// trickle up debug message system use onDebugMessage()
	/// to trigger with prepended sender id.
	void debugMessage(const QString &sMessage) const;

public slots:
	/// signal a message for debugging
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("AS:" + sMessage); }

	/// set the value for provided key. Will override value passed as CLI argument
	inline void setValue(const QString &sKey, const QVariant &mValue) {
		if (this->isOverridden(sKey)) this->oJo.remove(sKey);
		this->pSettings->setValue(sKey, mValue); }

	/// passthrough method to QSettings::sync()
	inline void sync() { this->pSettings->sync(); }

}; // AppSettings



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_APPSETTINGS_H

