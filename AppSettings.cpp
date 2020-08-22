#include "AppSettings.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>
#include <QStandardPaths>



namespace SwissalpS { namespace QtSssSircBot {



const QString AppSettings::sSettingBackdoorIntervalMS = "uiBackdoorIntervalMS";
const QString AppSettings::sSettingBackdoorPathFile = "sPathFileBackdoor";
const QString AppSettings::sSettingIRCconfig = "sIRCconfigPath";
const QString AppSettings::sSettingIRCremoteChannels = "sIRCremoteChannels";
const QString AppSettings::sSettingIRCremoteHost = "sIRCremoteHost";
const QString AppSettings::sSettingIRCremoteNicks = "sIRCremoteNicks";
const QString AppSettings::sSettingIRCremotePassword = "sIRCremotePassword";
const QString AppSettings::sSettingIRCremotePort = "uiIRCremotePort";
const QString AppSettings::sSettingIRCremoteRealname = "sIRCremoteRealname";
const QString AppSettings::sSettingPIDpathFile = "sPathFilePID";
#ifdef SssS_USE_GUI
const QString AppSettings::sSettingUseGUI = "bUseGUI";
#endif

const quint32 AppSettings::sSettingBackdoorIntervalMSdefault = (quint32)(3u * 60000u);
const QString AppSettings::sSettingBackdoorPathFileDefault = "";
const QString AppSettings::sSettingIRCconfigDefault = "";
const QString AppSettings::sSettingIRCremoteChannelsDefault = "";
const QString AppSettings::sSettingIRCremoteHostDefault = "0.0.0.0";
const QString AppSettings::sSettingIRCremoteNickDefault = "QtSssSbot";
const QString AppSettings::sSettingIRCremotePasswordDefault = "";
const quint16 AppSettings::sSettingIRCremotePortDefault = 6697u;
const QString AppSettings::sSettingIRCremoteRealnameDefault = "QtSwissalpSbot";
const QString AppSettings::sSettingPIDpathFileDefault = "";
#ifdef SssS_USE_GUI
const bool AppSettings::sSettingUseGUIdefault = true;
#endif

AppSettings *AppSettings::pSingelton = 0;


AppSettings::AppSettings(QObject *parent) :
	QObject(parent),
	sPathDataBase(""),
	sPathMusicBase(""),
	pSettings(0),
	bInitDone(false) {

	this->oJo = QJsonObject();

} // construct


AppSettings::~AppSettings() {

} // dealloc


AppSettings *AppSettings::pAppSettings() {

	///
	/// \brief oMutex
	///
	static QMutex oMutex;

	///////////////////////////////
	/* NOTE from: http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern
	 * Be aware that you should check for a std::bad_alloc exception
	 * to unlock the mutex in case new fails. We are also using
	 * double-checked locking as locking and unlocking a mutex
	 * is expensive and would significantly slow down a call to
	 *  Singleton::instance().
	 */
	if (!AppSettings::pSingelton) {

		oMutex.lock();

		if (!pSingelton) {

			pSingelton = new AppSettings();

		} // if first call

		oMutex.unlock();

	} // if first call

	return pSingelton;

} // singelton access


void AppSettings::drop() {

	static QMutex oMutex;

	oMutex.lock();

	delete pSingelton;
	pSingelton = 0;

	oMutex.unlock();

} // drop singelton


void AppSettings::copyResources(QString sPathSource, QString sPathTarget, bool bExe) {
	//qDebug() << __PRETTY_FUNCTION__ << sPathSource << sPathTarget;

	QDir oPath = QDir("/");
	// first make sure target path exists
	if (!oPath.mkpath(sPathTarget)) {

		this->onDebugMessage(tr("Can not make path: ") + sPathTarget +
						"\n" + tr("Without it, I can not predict what will happen."));

		return;

	} // if path does not exist and can not be made

	// ensure a trailing slash
	if (!sPathTarget.endsWith("/")) sPathTarget += "/";
	if (!sPathSource.endsWith("/")) sPathSource += "/";
	if (!sPathSource.startsWith(":/")) {

		if (sPathSource.startsWith("/")) {

			sPathSource = ":" + sPathSource;

		} else sPathSource = ":/" + sPathSource;

	} // if not a full source path

	QString sResource;
	QString sPathFileSource, sPathFileTarget;
	foreach (sResource, QDir(sPathSource).entryList()) {

		sPathFileSource = sPathSource + sResource;
		sPathFileTarget = sPathTarget + sResource;

		if (QFileInfo(sPathFileSource).isDir()) {

			this->copyResources(sPathFileSource, sPathFileTarget, bExe);

			continue;

		} // if dir encountered

		if (!QFileInfo(sPathFileTarget).exists()) {

			QFile::copy(sPathFileSource, sPathFileTarget);

			// set correct permissions
			if (bExe) {

				QFile(sPathFileTarget).setPermissions(
						QFile::ReadOther | QFile::ReadGroup
						| QFile::ReadOwner
						| QFile::WriteGroup | QFile::WriteOwner
						| QFile::ExeGroup | QFile::ExeOwner);

			} else {

				QFile(sPathFileTarget).setPermissions(
						QFile::ReadOther | QFile::ReadGroup
						| QFile::ReadOwner
						| QFile::WriteGroup | QFile::WriteOwner);

			} // if make executable or not

		} // if resource file does not exist in target directory

	} // loop all resources source directory

} // copyResources


QVariant AppSettings::get(const QString sKey) const {

	// return override value if exists
	if (this->oJo.contains(sKey)) return this->oJo.value(sKey).toVariant();

	if (sSettingBackdoorIntervalMS == sKey) {

		quint32 uiInterval = pSettings->value(sKey, sSettingBackdoorIntervalMSdefault).toUInt();

		return QVariant(uiInterval);

	} else if (sSettingBackdoorPathFile == sKey) {

		QString sPathFile = pSettings->value(
								sKey, sSettingBackdoorPathFileDefault).toString().trimmed();

		if (sPathFile.isEmpty()) {

			sPathFile = this->sPathDataBase + "backdoor";

		} // if none defined

		return QVariant(sPathFile);

	} else if (sSettingIRCconfig == sKey) {

		return pSettings->value(sKey, sSettingIRCconfigDefault);

	} else if (sSettingIRCremoteChannels == sKey) {

		return pSettings->value(sKey, sSettingIRCremoteChannelsDefault);

	} else if (sSettingIRCremoteHost == sKey) {

		return pSettings->value(sKey, sSettingIRCremoteHostDefault);

	} else if (sSettingIRCremoteNicks == sKey) {

		return pSettings->value(sKey, sSettingIRCremoteNickDefault);

	} else if (sSettingIRCremotePassword == sKey) {

		return pSettings->value(sKey, sSettingIRCremotePasswordDefault);

	} else if (sSettingIRCremotePort == sKey) {

		return pSettings->value(sKey, sSettingIRCremotePortDefault);

	} else if (sSettingIRCremoteRealname == sKey) {

		return pSettings->value(sKey, sSettingIRCremoteRealnameDefault);

	} else if (sSettingPIDpathFile == sKey) {

		QString sPathFile = pSettings->value(
								sKey, sSettingPIDpathFileDefault).toString().trimmed();

		if (sPathFile.isEmpty()) {

			sPathFile = this->sPathDataBase + SssS_APP_NAME + ".pid";

		} // if none defined

		return QVariant(sPathFile);

	}
#ifdef SssS_USE_GUI
	else if (sSettingUseGUI == sKey) {

		return pSettings->value(sKey, sSettingUseGUIdefault);

	}
#endif
	else {

		this->onDebugMessage(tr("OO:Unhandled key: ") + sKey);

		return this->pSettings->value(sKey);

	} // switch sKey

} // get


QJsonArray AppSettings::getConfigs() const {

	const QString sPath = this->get(sSettingIRCconfig).toString();
	QFile oFile(sPath);

	if (oFile.open(QIODevice::ReadOnly)) {

		const QByteArray aJson = oFile.readAll();
		oFile.close();

		QJsonDocument oJdoc = QJsonDocument::fromJson(aJson);

		if ((!oJdoc.isNull()) && oJdoc.isArray()) {

			return oJdoc.array();

		}

	} // if opened

	this->onDebugMessage("Falling back to one default connection.");

	QJsonArray oOut;
	QJsonObject oJo;

	oJo.insert("sConnectionID", "unset");
	oJo.insert(sSettingIRCremoteHost,
			   this->get(sSettingIRCremoteHost).toString());
	const QString sNicks = this->get(sSettingIRCremoteNicks).toString();
	const QStringList aNicks = sNicks.split(',', QString::SkipEmptyParts, Qt::CaseInsensitive);
	QJsonArray oJaNicks;
	for (int i = 0; i < aNicks.count(); ++i) oJaNicks.append(aNicks.at(i));
	oJo.insert(sSettingIRCremoteNicks, oJaNicks);
	oJo.insert(sSettingIRCremotePort,
			   (double)this->get(sSettingIRCremotePort).toUInt());
	oJo.insert(sSettingIRCremoteRealname,
			   this->get(sSettingIRCremoteRealname).toString());
	oJo.insert(sSettingIRCremotePassword,
			   this->get(sSettingIRCremotePassword).toString());
	QJsonArray oJa;
	const QStringList aChannels = this->getChannels();
	for (int i = 0; i < aChannels.count(); ++i) oJa.append(aChannels.at(i));
	oJo.insert(sSettingIRCremoteChannels, oJa);

	oJo.insert("sIRCrawPostLoginLines", QJsonArray());
	oJo.insert("sIRCrawPreLoginLines", QJsonArray());

	oOut.append(oJo);

	return oOut;

} // getConfigs


QStringList AppSettings::getChannels() const {

	QString sChannels = this->get(sSettingIRCremoteChannels).toString();
	return sChannels.split(",", QString::SkipEmptyParts, Qt::CaseInsensitive);

} // getChannels

void AppSettings::setChannels(const QStringList aChannels) {

	this->pSettings->setValue(sSettingIRCremoteChannels, aChannels.join(","));

} // setChannels


QString AppSettings::getPathBackdoor() const {

	return this->get(sSettingBackdoorPathFile).toString();

} // getPathBackdoor


QString AppSettings::getPathData() const {

	return this->sPathDataBase;

} // getPathData


QString AppSettings::getPathMusic() const {

	return this->sPathMusicBase;

} // getPathMusic

void AppSettings::setPathMusic(const QString &sNewPath) {

	if (0 == this->sPathMusicBase.compare(sNewPath)) return;

	QString sPath(sNewPath);

	if (!sPath.endsWith("/")) sPath.append("/");

	this->sPathMusicBase = sPath;

} // setPathMusic


QSettings *AppSettings::getSettings() const {

	return this->pSettings;

} // getSettings


void AppSettings::init() {

	if (this->bInitDone) return;

	// init paths
	this->initPaths();

	// init directory structure
	this->setupDirs(this->sPathDataBase);

	// init settings

	this->pSettings = new QSettings(this->sPathDataBase + "Settings.ini",
							  QSettings::IniFormat, this);

	QSettings *pS = this->pSettings;

	pS->setValue(sSettingBackdoorIntervalMS, this->get(sSettingBackdoorIntervalMS));
	pS->setValue(sSettingBackdoorPathFile, this->get(sSettingBackdoorPathFile));
	pS->setValue(sSettingIRCconfig, this->get(sSettingIRCconfig));
	pS->setValue(sSettingIRCremoteChannels, this->get(sSettingIRCremoteChannels));
	pS->setValue(sSettingIRCremoteHost, this->get(sSettingIRCremoteHost));
	pS->setValue(sSettingIRCremoteNicks, this->get(sSettingIRCremoteNicks));
	pS->setValue(sSettingIRCremotePassword, this->get(sSettingIRCremotePassword));
	pS->setValue(sSettingIRCremotePort, this->get(sSettingIRCremotePort));
	pS->setValue(sSettingIRCremoteRealname, this->get(sSettingIRCremoteRealname));
	pS->setValue(sSettingPIDpathFile, this->get(sSettingPIDpathFile));
#ifdef SssS_USE_GUI
	pS->setValue(sSettingUseGUI, this->get(sSettingUseGUI));
#endif

	this->pSettings->sync();

	this->initOverrides();

	this->bInitDone = true;

} // init


void AppSettings::initOverrides() {

	QCommandLineOption oBackdoor("backdoor",
								 "path to backdoor file to read "
								 "occassionally for commands.",
								 "backdoor");
	QCommandLineOption oChannels("channels",
								 "List of comma separated channels to join "
								 "once logged in.",
								 "channels");
	QCommandLineOption oConfig("config",
							   "Give path to configuration "
							   "JSON-file to use instead of values from "
							   "settings.ini or command line arguments. This "
							   "will enable multiple connections and other fun "
							   "ideas like a list of raw IRC to send for "
							   "additional authentication and mode changes.",
							   "configuration");
	QCommandLineOption oHost("host", "Remote host to use.", "host");
	QCommandLineOption oNicks("nicks", "Nicks to use.", "nicks");
	QCommandLineOption oPass("pass", "Password to use.", "password");
	QCommandLineOption oPort("port", "Remote port to use.", "port");
	QCommandLineOption oRealName("realname", "Realname to use.", "realname");
#ifdef SssS_USE_GUI
//	QCommandLineOption oGUI(QStringList() << "g" << "gui", "Show GUI.");
//	QCommandLineOption oNoGUI("no_gui", "Don't show GUI.");
#endif

	QCommandLineParser oCLP;
	oCLP.setApplicationDescription("Qt SwissalpS IRC bot.");
	oCLP.addHelpOption();
	oCLP.addVersionOption();

	oCLP.addOption(oBackdoor);
	oCLP.addOption(oChannels);
	oCLP.addOption(oConfig);
	oCLP.addOption(oHost);
	oCLP.addOption(oNicks);
	oCLP.addOption(oPass);
	oCLP.addOption(oPort);
	oCLP.addOption(oRealName);
#ifdef SssS_USE_GUI
//	oCLP.addOption(oGUI);
//	oCLP.addOption(oNoGUI);
#endif

	oCLP.process(*QCoreApplication::instance());

	const QString sBackdoor = oCLP.value(oBackdoor);
	const QString sChannels = oCLP.value(oChannels);
	const QString sConfig = oCLP.value(oConfig);
	const QString sHost = oCLP.value(oHost);
	const QString sNicks = oCLP.value(oNicks); // TODO: limit each to 9 chars
	const QString sPass = oCLP.value(oPass);
	const quint16 uiPort = oCLP.value(oPort).toUInt();
	const QString sRealName = oCLP.value(oRealName); // TODO: limit to x chars
#ifdef SssS_USE_GUI
//	bool bGUI = oCLP.isSet(oGUI);
//	bool bNoGUI = oCLP.isSet(oNoGUI);
#endif

	if (oCLP.isSet(oBackdoor)) {
		this->oJo.insert(sSettingBackdoorPathFile, sBackdoor);
	}

	if (oCLP.isSet(oChannels)) {
		this->oJo.insert(sSettingIRCremoteChannels, sChannels);
	}

	if (oCLP.isSet(oConfig)) {
		QFile oFile(sConfig);
		if (oFile.isReadable()) this->oJo.insert(sSettingIRCconfig, sConfig);
	}

	if (oCLP.isSet(oHost)) {
		this->oJo.insert(sSettingIRCremoteHost, sHost);
	}

	if (oCLP.isSet(oNicks)) {
		this->oJo.insert(sSettingIRCremoteNicks, sNicks);
	}

	if (oCLP.isSet(oPass)) {
		this->oJo.insert(sSettingIRCremotePassword, sPass);
	}

	if (oCLP.isSet(oPort)) {
		this->oJo.insert(sSettingIRCremotePort, (double)uiPort);
	} // if port

	if (oCLP.isSet(oRealName)) {
		this->oJo.insert(sSettingIRCremoteRealname, sRealName);
	}

#ifdef SssS_USE_GUI
//	if (bNoGUI) {
//		this->oJo.insert(sSettingUseGUI, false);
//	} else if (bGUI) {
//		this->oJo.insert(sSettingUseGUI, true);
//	} // if GUI or not
#endif

} // initOverrides


// [markerInitPaths]
void AppSettings::initPaths() {

	// data ie playlists, settings etc
	QStringList aPaths = QStandardPaths::standardLocations(
							 QStandardPaths::DocumentsLocation);

	this->sPathDataBase = aPaths.first() + "/"
						  + QCoreApplication::applicationName() + "/";

	this->onDebugMessage(tr("OK:Using for data: ") + this->sPathDataBase);

	// standard media path
	aPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);

	this->sPathMusicBase = aPaths.first() + "/";

	//this->onDebugMessage(tr("OK:Using for music: ") + this->sPathMusicBase);

} // initPaths
// [markerInitPaths]


void AppSettings::setSettings(QSettings *pNewSettings) {

	this->pSettings = pNewSettings;

} // setSettings


void AppSettings::setupDirs(QString sPathBase) {

	QDir oPath = QDir(sPathBase);

	if (!oPath.mkpath(sPathBase)) {

		this->onDebugMessage(tr("KO:Can not make path: ") + sPathBase +
						"\n" + tr("Without it, I can not predict what will happen."));

		return;

	} // if path does not exist and can not be made

	// also make a folder for executables and copy missing resources
	QString sPathBin = sPathBase + "bin/";
	this->copyResources(":/omxScripts/", sPathBin, true);

//	// copy gpio executables, if missing
//	this->copyResources(":/gpioScripts/", sPathBin, true);

	// copy config, if missing
	QString sPathConfig = sPathBase + "config/";
	this->copyResources(":/config/", sPathConfig, false);

	// copy root executables, if missing
	//TODO: chown root:root
//	QString sPathBinRoot = sPathBase + "sbin/";
//	this->copyResources(":/rootScripts/", sPathBinRoot, true);

//	// also make a folder for www and fill with the basics
//	QString sPathWWW = sPathBase + "www/";
//	this->copyResources(":/www/", sPathWWW);

} // setupDirs



}	} // namespace SwissalpS::QtSssSircBot

