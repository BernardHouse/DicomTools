#pragma once

#include "..\..\DicomCommon\Util\Logger.h"

#include <QMutex>
#include <QSettings>
#include "Defines.h"
#include <qlist.h>
#include "Sqlite\AppSqliteDatabase.h"

/** A manager class for global application settings.
 */
class SettingsManager : public QObject {

	Q_OBJECT

public:

	static SettingsManager* instance();
	virtual ~SettingsManager();

	void saveWorklistFilterSettings();
	void loadWorklistFilterSettings();
	void saveSettings();
	void loadSettings();

	QString getWorklistFilterOption( const QString& key ) const;
	void setWorklistFilterOption( const QString& key, const QString& value );
	QString getOption( const QString& key ) const;
	void setOption( const QString& key, const QString& value );

	void setDefaultOptions();
	void setWorklistFilterDefaultOptions();
	const QString getComputerName(void);

	bool initDB(void);
	AppSqliteDatabase &getAppSqliteDatabase(void){return m_Database;};
	void refreshWorklistAETlist();
	void refreshStoreImageAETlist();
	const QList<structServerTable> *getWorklistAETlist();
	const QList<structServerTable> *getStoreImageAETlist();

	bool getDBcreated(void){return m_bDBcreated;};

	void refreshScheduledStationAETitleList(void);
	const QStringList getScheduledStationAETitleList(){return m_qslScheduledStationAETitle;};
	void refreshScheduledPerformingPhysicianNameList(void);
	const QStringList getScheduledPerformingPhysicianNameList(){return m_qslScheduledPerformingPhysicianName;};
	void refreshRequestingPhysicianList(void);
	const QStringList getRequestingPhysicianList(){return m_qslRequestingPhysician;};
	void refreshReferringPhysicianNameList(void);
	const QStringList getReferringPhysicianNameList(){return m_qslReferringPhysicianName;};
	void refreshPreMedicationList(void);
	const QStringList getPreMedicationList(){return m_qslPreMedication;};
	void refreshMedicalAlertsList(void);
	const QStringList getMedicalAlertsList(){return m_qslMedicalAlerts;};
	void refreshContrastAllergiesList(void);
	const QStringList getContrastAllergiesList(){return m_qslContrastAllergies;};
	void refreshScheduledStationNameList(void);
	const QStringList getScheduledStationNameList(){return m_qslScheduledStationName;};


signals:

	void settingsChanged();
	void settingsWorklistFilterChanged();
	void settingsWorklistServerChanged();
	void settingsStoreImageServerChanged();

private:

	SettingsManager();
	SettingsManager& operator=( SettingsManager& ); // disallowed
	SettingsManager( const SettingsManager& ); // disallowed

	mutable QMutex mutex;
	QSettings* qs;
	Logger* log;
	QMap<QString,QString> settingsMap;
	QMap<QString,QString> settingsWorklistFilterMap;

	QList<structServerTable> m_qlstWorklistAETs;
	QList<structServerTable> m_qlstStoreImageAETs;

	QStringList m_qslScheduledStationAETitle;
	QStringList m_qslScheduledPerformingPhysicianName;
	QStringList m_qslRequestingPhysician;
	QStringList m_qslReferringPhysicianName;
	QStringList m_qslPreMedication;
	QStringList m_qslScheduledStationName;
	QStringList m_qslMedicalAlerts;
	QStringList m_qslContrastAllergies;

	AppSqliteDatabase m_Database;
	bool m_bDBcreated;

	static SettingsManager sm;

};
