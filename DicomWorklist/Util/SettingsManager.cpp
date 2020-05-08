#include "SettingsManager.h"

#include <QtCore\QDir>
#include <QtCore\QStringList>
#include <windows.h>


SettingsManager::SettingsManager() {
	
	//qs = new QSettings( QSettings::NativeFormat , QSettings::UserScope/*QSettings::SystemScope*/, EDAP_REGISTRY_KEY_ORGANISATION, EDAP_REGISTRY_KEY_WORKSTATION);	
	qs = new QSettings(EDAP_REGISTRY_KEY_ORGANISATION, EDAP_REGISTRY_KEY_WORKLISTSCP);	

	log = Logger::instance();

	loadSettings();
	setDefaultOptions();
	saveSettings();

	m_bDBcreated = false;

	loadWorklistFilterSettings();
	setWorklistFilterDefaultOptions();
	saveWorklistFilterSettings();

}

bool SettingsManager::initDB(void)
{
	//verify first the databse creation
	m_bDBcreated = m_Database.CreateDataBase(DICOMsqliteFile);
	if( m_bDBcreated == true )
	{
		//refreshWorklistAETlist();
		//refreshStoreImageAETlist();
		refreshScheduledStationAETitleList();
		refreshScheduledPerformingPhysicianNameList();
		refreshRequestingPhysicianList();
		refreshReferringPhysicianNameList();
		refreshPreMedicationList();
		refreshMedicalAlertsList();
		refreshContrastAllergiesList();
		refreshScheduledStationNameList();
	}
	return m_bDBcreated;
}


SettingsManager* SettingsManager::instance()
{
	return &sm;
}

SettingsManager::~SettingsManager() {
	if( qs != NULL )
	{
		delete qs;
		qs = NULL;
	}
}

void SettingsManager::setDefaultOptions() {
	// we are using the getOption() method to either get the existing value
	// (i.e., the value doesn't change) or we might replace it with a default
	// value (if the option is faulty or missing)
	setOption( "initialLaunch", getOption( "initialLaunch" ) );
	setOption( "localPort", getOption( "localPort" ) );
	setOption( "activeWLMSCPdirectory", "" ); // will be filled later
	setOption( "localServerFork", getOption( "localServerFork" ) );
	setOption( "calledAETitle", getOption( "calledAETitle" ) );
	setOption( "serverDebugMode", getOption( "serverDebugMode" ) );
	setOption( "serverPublicMode", getOption( "serverPublicMode" ) );
	setOption( "serverOutputLog", getOption( "serverOutputLog" ) );
	setOption( "serverStartAutomatically", getOption( "serverStartAutomatically" ) );	
	setOption( "serverVeryVerboseMode", getOption( "serverVeryVerboseMode" ) );
	setOption( "windowSizeWidth", getOption( "windowSizeWidth" ) );
	setOption( "windowSizeHeight", getOption( "windowSizeHeight" ) );

	// make sure the active local AET exists, modify it if not
	/*if ( !localAETs.contains( settingsMap[ "activeLocalAET" ] ) ||
		 localAETs[ settingsMap[ "activeLocalAET" ] ] != settingsMap[ "activeLocalAETdirectory" ] )
	{

		if ( localAETs.count() > 0 )
		{
			settingsMap[ "activeLocalAET" ] = localAETs.keys().first();
			settingsMap[ "activeLocalAETdirectory" ] = localAETs.value( settingsMap[ "activeLocalAET" ] );
		}
		else
		{
			settingsMap[ "activeLocalAET" ] = "";
			settingsMap[ "activeLocalAETdirectory" ] = "";
		}
		
	}*/	

}

void SettingsManager::saveSettings()
{
	mutex.lock();
	qs->beginGroup( "settings" );
	QMap<QString,QString>::const_iterator i = settingsMap.constBegin();
	while( i != settingsMap.constEnd() )
	{
		qs->setValue( i.key(), i.value() );
		i++;
	}
	qs->endGroup();
	mutex.unlock(); // release manually to avoid possibly deadlock
	emit settingsChanged();
}

void SettingsManager::loadSettings()
{
	QMutexLocker locker( &mutex );
	settingsMap.clear();
	qs->beginGroup( "settings" );
	QStringList keys = qs->childKeys();
	for ( int i = 0; i < keys.size(); i++ )
	{
		settingsMap[ keys.at(i) ] = qs->value( keys.at(i) ).toString();
	}
	qs->endGroup();
}

QString SettingsManager::getOption( const QString& key ) const
{
	QMutexLocker locker( &mutex );
	QString returnValue = settingsMap[ key ];
	if ( returnValue.isEmpty() )
	{
    	// if empty (or doesn't exist), use a default value
		if ( key == "initialLaunch" ) {
			returnValue = "true";
		} else if ( key == "localPort" ) {
			returnValue = "104";
		} else if ( key == "activeWLMSCPdirectory" ) {
			returnValue = "c:/DicomWorklistSCP"; 
		} else if ( key == "calledAETitle" ) {
			returnValue = "EDAP_BWL"; 
		} else if ( key == "localServerFork" ) {
			returnValue = "true";
		} else if ( key == "serverDebugMode" ) {
			returnValue = "false";
		} else if ( key == "serverPublicMode" ) {
			returnValue = "false";
		} else if ( key == "serverOutputLog" ) {
			returnValue = QDir::tempPath() + "/EDAPdicom_server_output.txt";
		} else if ( key == "serverStartAutomatically" ) {
			returnValue = "true";	
		} else if ( key == "serverVeryVerboseMode" ) {
			returnValue = "false";
		} else if ( key == "windowSizeWidth" ) {
			returnValue = "1024";
		} else if ( key == "windowSizeHeight" ) {
			returnValue = "768";
		}
	}
	return returnValue;
}

void SettingsManager::setOption( const QString& key, const QString& value )
{
	QMutexLocker locker( &mutex );
	settingsMap[ key ] = value;
}

/*void SettingsManager::refreshLocalAETlist()
{
	QMutexLocker locker( &mutex );

	// clear local AET map
	localAETs.clear();

	// get local AETs from the config file
	QFile file( EDAP_DCMQRSCP_CONFIG_FILE );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		QByteArray content = file.readAll();		
		QString startTag = "AETable BEGIN";
		QString endTag = "AETable END";
		int begIdx = content.indexOf( startTag ) + startTag.length() + 1;
		int endIdx = content.indexOf( endTag );
		QByteArray aeTable = content.mid( begIdx, endIdx - begIdx );
		QList<QByteArray> lines = aeTable.split( '\n' );
		for ( int i = 0; i < lines.size(); i++ ) {				
			QRegExp rx( "^(\\w+(?!#))\\s+(\\S+)\\s" );			
			rx.setMinimal( true );
			if ( rx.indexIn( lines[i] ) != -1 ) {
				// get the resultclean (possible) quotation marks
				QString aet = rx.cap( 1 );	
				QString dir = rx.cap( 2 ).remove( '"' );
				// in addition, add a slash to the end
				if ( !dir.endsWith( '\\' ) && !dir.endsWith( '/' ) ) {        
					dir += QDir::separator();
				}
				localAETs[ aet ] = dir;
			}
		}				
	} else {
		log->dbg( "Error: unable to open server config file." );		
	}
	
	if ( localAETs.count() == 0 )  {
		log->dbg( "Error: no local AETs specified" );
	}

}

const QMap<QString,QString>* SettingsManager::getLocalAETlist() {
	QMutexLocker locker( &mutex );
	return &localAETs;
}*/





//---------------------------------------------------------------------------------------------
const QString SettingsManager::getComputerName(void)
{
	QString qsRet = tr("Error while reading Computer name");
	TCHAR chrName[30];
	DWORD dwSize = 29;
	if( GetComputerName(chrName, &dwSize) )
	{
		QString qs((QChar*)chrName, dwSize);
		qsRet = qs;
	}
	return qsRet;
}



//---------------------------------------------------------------------------------------------
void SettingsManager::refreshWorklistAETlist()
{
/*	mutex.lock();

	// clear local AET map
	m_qlstWorklistAETs.clear();
	if( m_bDBcreated == true )
	{
	}
	mutex.unlock();
	emit settingsWorklistServerChanged();*/
}

//---------------------------------------------------------------------------------------------
const QList<structServerTable> *SettingsManager::getWorklistAETlist()
{
	QMutexLocker locker( &mutex );
	return &m_qlstWorklistAETs;
}


//---------------------------------------------------------------------------------------------
void SettingsManager::refreshScheduledStationAETitleList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslScheduledStationAETitle.clear();
		m_qslScheduledStationAETitle = m_Database.readOneTableField(tblScheduledStationAETitle, "ScheduledStationAETitleVALUE");
	}
}

//---------------------------------------------------------------------------------------------
void SettingsManager::refreshScheduledStationNameList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslScheduledStationName.clear();
		m_qslScheduledStationName = m_Database.readOneTableField(tblScheduledStationName, "ScheduledStationNameVALUE");
	}
}

//---------------------------------------------------------------------------------------------
void SettingsManager::refreshScheduledPerformingPhysicianNameList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslScheduledPerformingPhysicianName.clear();
		m_qslScheduledPerformingPhysicianName = m_Database.readOneTableField(tblScheduledPerformingPhysicianName, "ScheduledPerformingPhysicianNameVALUE");
	}
}
//---------------------------------------------------------------------------------------------
void SettingsManager::refreshRequestingPhysicianList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslRequestingPhysician.clear();
		m_qslRequestingPhysician = m_Database.readOneTableField(tblRequestingPhysician, "RequestingPhysicianVALUE");
	}
}
//---------------------------------------------------------------------------------------------
void SettingsManager::refreshReferringPhysicianNameList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslReferringPhysicianName.clear();
		m_qslReferringPhysicianName = m_Database.readOneTableField(tblReferringPhysicianName, "ReferringPhysicianNameVALUE");
	}
}

//---------------------------------------------------------------------------------------------
void SettingsManager::refreshPreMedicationList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslPreMedication.clear();
		m_qslPreMedication = m_Database.readOneTableField(tblPreMedication, "PreMedicationVALUE");
	}
}
//---------------------------------------------------------------------------------------------
void SettingsManager::refreshMedicalAlertsList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslMedicalAlerts.clear();
		m_qslMedicalAlerts = m_Database.readOneTableField(tblMedicalAlerts, "MedicalAlertsVALUE");
	}
}
//---------------------------------------------------------------------------------------------
void SettingsManager::refreshContrastAllergiesList(void)
{
	if( m_bDBcreated == true )
	{
		QMutexLocker locker( &mutex );
		m_qslContrastAllergies.clear();
		m_qslContrastAllergies = m_Database.readOneTableField(tblContrastAllergies, "ContrastAllergiesVALUE");
	}
}
//---------------------------------------------------------------------------------------------
void SettingsManager::refreshStoreImageAETlist()
{
/*	mutex.lock();

	// clear local AET map
	m_qlstStoreImageAETs.clear();
	if( m_bDBcreated == true )
	{
	}
	mutex.unlock();
	emit settingsStoreImageServerChanged();*/
}


//---------------------------------------------------------------------------------------------
const QList<structServerTable> *SettingsManager::getStoreImageAETlist()
{
	QMutexLocker locker( &mutex );
	return &m_qlstStoreImageAETs;
}




//-------------------- WorklistFilter settings -------------------------------
void SettingsManager::setWorklistFilterDefaultOptions()
{
	// we are using the getWorklistFilterOption() method to either get the existing value
	// (i.e., the value doesn't change) or we might replace it with a default
	// value (if the option is faulty or missing)
	setWorklistFilterOption( txtPatientNameFilter, getWorklistFilterOption(txtPatientNameFilter) );
	setWorklistFilterOption( txtPatientIDFilter, getWorklistFilterOption(txtPatientIDFilter) );
	setWorklistFilterOption( txtPatientSexFilter, getWorklistFilterOption(txtPatientSexFilter) );
	setWorklistFilterOption( txtPatientBirthDateFilter, getWorklistFilterOption(txtPatientBirthDateFilter) );
	setWorklistFilterOption( txtAccessionNumberFilter, getWorklistFilterOption(txtAccessionNumberFilter) );
	setWorklistFilterOption( txtScheduledStationAETitleFilter, getWorklistFilterOption(txtScheduledStationAETitleFilter) );
	setWorklistFilterOption( txtModalityFilter, getWorklistFilterOption(txtModalityFilter) );
	setWorklistFilterOption( txtScheduledPerformingPhysiciansNameFilter, getWorklistFilterOption(txtScheduledPerformingPhysiciansNameFilter) );
	setWorklistFilterOption( txtScheduledProcedureStepStartDateFilter, getWorklistFilterOption(txtScheduledProcedureStepStartDateFilter) );
}

void SettingsManager::saveWorklistFilterSettings()
{
	mutex.lock();
	qs->beginGroup( "WorklistFilter" );
	QMap<QString,QString>::const_iterator i = settingsWorklistFilterMap.constBegin();
	while( i != settingsWorklistFilterMap.constEnd() )
	{
		qs->setValue( i.key(), i.value() );
		i++;
	}
	qs->endGroup();
	mutex.unlock(); // release manually to avoid possibly deadlock
	emit settingsWorklistFilterChanged();
}

void SettingsManager::loadWorklistFilterSettings()
{
	QMutexLocker locker( &mutex );
	settingsWorklistFilterMap.clear();
	qs->beginGroup( "WorklistFilter" );
	QStringList keys = qs->childKeys();
	for ( int i = 0; i < keys.size(); i++ )
	{
		settingsWorklistFilterMap[ keys.at(i) ] = qs->value( keys.at(i) ).toString();
	}
	qs->endGroup();
}

QString SettingsManager::getWorklistFilterOption( const QString& key ) const
{
	QMutexLocker locker( &mutex );
	QString returnValue = settingsWorklistFilterMap[ key ];
	if ( returnValue.isEmpty() )
	{
		if( key == txtPatientSexFilter ) {
			returnValue = txtAny;
		}else if ( key == txtPatientBirthDateFilter ) {
			returnValue = txtAny;
		}else if ( key == txtScheduledStationAETitleFilter ) {
			returnValue = txtAny;
		}else if ( key == txtScheduledProcedureStepStartDateFilter ) {
			returnValue = txtAny;
		}
	}
	return returnValue;
}

void SettingsManager::setWorklistFilterOption( const QString& key, const QString& value )
{
	QMutexLocker locker( &mutex );
	settingsWorklistFilterMap[ key ] = value;
}




// static member initialization
SettingsManager SettingsManager::sm;

