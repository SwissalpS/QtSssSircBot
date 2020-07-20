#ifndef SwissalpS_QtSssSircBot_APPSETTINGS_H
#define SwissalpS_QtSssSircBot_APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QJsonObject>



namespace SwissalpS { namespace QtSssSircBot {



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
	QJsonObject oJo; // contains overrides from CLI arguments
	bool bInitDone;
	void initOverrides();
	void initPaths();

public:
	static const QString sSettingIRCremoteChannels;
	static const QString sSettingIRCremoteHost;
	static const QString sSettingIRCremoteNick;
	static const QString sSettingIRCremotePassword;
	static const QString sSettingIRCremotePort;
	static const QString sSettingIRCremoteRealname;
	static const QString sSettingPIDpathFile;
#ifdef SssS_USE_GUI
	static const QString sSettingUseGUI;
#endif

	static const QString sSettingIRCremoteChannelsDefault;
	static const QString sSettingIRCremoteHostDefault;
	static const QString sSettingIRCremoteNickDefault;
	static const QString sSettingIRCremotePasswordDefault;
	static const quint16 sSettingIRCremotePortDefault;
	static const QString sSettingIRCremoteRealnameDefault;
	static const QString sSettingPIDpathFileDefault;
#ifdef SssS_USE_GUI
	static const bool sSettingUseGUIdefault;
#endif

	// destroy singelton
	static void drop();
	// public access to singelton instance
	static AppSettings *pAppSettings();
	virtual ~AppSettings();

	QVariant get(const QString sKey) const;

	QStringList getChannels() const;
	void setChannels(const QStringList aChannels);

	QString getPathData() const;

	QString getPathMusic() const;
	void setPathMusic(const QString &sNewPath);

	QSettings *getSettings() const;
	void setSettings(QSettings *pNewSettings);

	inline bool isOverridden(const QString sKey) { return this->oJo.contains(sKey); }
	void init();

signals:
	void debugMessage(const QString &sMessage) const;

public slots:
	inline void onDebugMessage(const QString &sMessage) const {
		Q_EMIT this->debugMessage("AS:" + sMessage); }

	inline void setValue(const QString &sKey, const QVariant &mValue) {
		if (this->isOverridden(sKey)) this->oJo.remove(sKey);
		this->pSettings->setValue(sKey, mValue); }

	inline void sync() { this->pSettings->sync(); }

}; // AppSettings



}	} // namespace SwissalpS::QtSssSircBot



#endif // SwissalpS_QtSssSircBot_APPSETTINGS_H
