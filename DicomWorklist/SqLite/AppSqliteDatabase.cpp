#include "AppSqliteDatabase.h"
#include <QFile>
#include <QMessageBox>

AppSqliteDatabase::AppSqliteDatabase(void) 
{
}

AppSqliteDatabase::~AppSqliteDatabase(void)
{
}




bool AppSqliteDatabase::CreateTables(void)
{
	bool bRet = false;
	if(ReadAllTables())
	{
		bRet = true;
	}
	else
	{
		QSqlQuery query;

		bRet = query.exec("create table ALL_SEQUENCES ("
						"[TABLE_NAME] varchar(64) not null,"
						"[CURRENT_VALUE] integer not null,"
						"CONSTRAINT TABLE_NAME_UK UNIQUE (TABLE_NAME)"
						")");
		bRet = query.exec("create table WorklistSCP ("
						"[tagPatientName] varchar(64) not null,"
						"[tagPatientBirthDate] varchar(8),"
						"[tagPatientID] varchar(64) not null unique,"
						"[tagAccessionNumber] varchar(16) not null,"
						"[tagPatientSex] varchar(16),"
						"[tagModality] varchar(16) not null,"
						"[tagScheduledProcedureStepStartDate] varchar(8) not null,"
						"[tagScheduledProcedureStepStartTime] varchar(16) not null,"
						"[tagScheduledPerformingPhysicianName] varchar(64) not null," 
						"[tagSpecificCharacterSet] varchar(16),"
						"[tagScheduledProcedureStepSequence] varchar(255),"
						"[tagScheduledStationAETitle] varchar(16) not null,"
						"[tagScheduledProcedureStepDescription] varchar(64),"
						"[tagScheduledStationName] varchar(16),"
						"[tagScheduledProcedureStepLocation] varchar(16),"
						"[tagPreMedication] varchar(64),"
						"[tagScheduledProcedureStepID] varchar(16),"
						"[tagRequestedContrastAgent] varchar(64),"
						"[tagRequestedProcedureID] varchar(16),"
						"[tagRequestedProcedureDescription] varchar(64),"
						"[tagStudyInstanceUID] varchar(64),"
						"[tagRequestedProcedurePriority] varchar(16),"
						"[tagPatientTransportArrangements] varchar(64),"
						"[tagRequestingPhysician] varchar(64),"
						"[tagReferringPhysicianName] varchar(64),"
						"[tagAdmissionID] varchar(64),"
						"[tagCurrentPatientLocation] varchar(64),"
						"[tagPatientWeight] varchar(16),"
						"[tagMedicalAlerts] varchar(64),"
						"[tagContrastAllergies] varchar(64)"
						")");

		bRet = query.exec("create table ScheduledStationAETitle ("
						"[ScheduledStationAETitleID] integer PRIMARY KEY not null,"
						"[ScheduledStationAETitleVALUE] varchar(16) not null,"
						"CONSTRAINT ScheduledStationAETitle_UK UNIQUE (ScheduledStationAETitleVALUE)"
						")");
		bRet = query.exec("create table ScheduledPerformingPhysicianName ("
						"[ScheduledPerformingPhysicianNameID] integer PRIMARY KEY not null,"
						"[ScheduledPerformingPhysicianNameVALUE] varchar(64) not null,"
						"CONSTRAINT ScheduledPerformingPhysicianName_UK UNIQUE (ScheduledPerformingPhysicianNameVALUE)"
						")");
		bRet = query.exec("create table RequestingPhysician ("
						"[RequestingPhysicianID] integer PRIMARY KEY not null,"
						"[RequestingPhysicianVALUE] varchar(64) not null,"
						"CONSTRAINT RequestingPhysician_UK UNIQUE (RequestingPhysicianVALUE)"
						")");
		bRet = query.exec("create table ReferringPhysicianName ("
						"[ReferringPhysicianNameID] integer PRIMARY KEY not null,"
						"[ReferringPhysicianNameVALUE] varchar(64) not null,"
						"CONSTRAINT ReferringPhysicianName_UK UNIQUE (ReferringPhysicianNameVALUE)"
						")");
		bRet = query.exec("create table PreMedication ("
						"[PreMedicationID] integer PRIMARY KEY not null,"
						"[PreMedicationVALUE] varchar(64) not null,"
						"CONSTRAINT PreMedication_UK UNIQUE (PreMedicationVALUE)"
						")");
		bRet = query.exec("create table ScheduledStationName ("
						"[ScheduledStationNameID] integer PRIMARY KEY not null,"
						"[ScheduledStationNameVALUE] varchar(16) not null,"
						"CONSTRAINT ScheduledStationName_UK UNIQUE (ScheduledStationNameVALUE)"
						")");
		bRet = query.exec("create table MedicalAlerts ("
						"[MedicalAlertsID] integer PRIMARY KEY not null,"
						"[MedicalAlertsVALUE] varchar(64) not null,"
						"CONSTRAINT MedicalAlerts_UK UNIQUE (MedicalAlertsVALUE)"
						")");
		bRet = query.exec("create table ContrastAllergies ("
						"[ContrastAllergiesID] integer PRIMARY KEY not null,"
						"[ContrastAllergiesVALUE] varchar(64) not null,"
						"CONSTRAINT ContrastAllergies_UK UNIQUE (ContrastAllergiesVALUE)"
						")");
		if(ReadAllTables())
		{
			bRet = query.exec("insert into ALL_SEQUENCES values('ScheduledStationAETitle', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('ScheduledPerformingPhysicianName', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('RequestingPhysician', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('ReferringPhysicianName', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('PreMedication', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('ScheduledStationName', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('MedicalAlerts', 0)");
			bRet = query.exec("insert into ALL_SEQUENCES values('ContrastAllergies', 0)");

			bRet = query.exec(QString("insert into ScheduledStationAETitle values(%0, 'SONOLITHISYS')").arg(readNextSequence("ScheduledStationAETitle", true)));
			bRet = query.exec(QString("insert into ScheduledStationAETitle values(%0, 'SONOLITHIMOVE')").arg(readNextSequence("ScheduledStationAETitle", true)));
			bRet = query.exec(QString("insert into ScheduledStationAETitle values(%0, 'FOCALONE')").arg(readNextSequence("ScheduledStationAETitle", true)));

			bRet = query.exec(QString("insert into ScheduledPerformingPhysicianName values(%0, 'Default ScheduledPerformingPhysicianName')").arg(readNextSequence("ScheduledPerformingPhysicianName", true)));
			bRet = query.exec(QString("insert into RequestingPhysician values(%0, 'Default RequestingPhysician')").arg(readNextSequence("RequestingPhysician", true)));
			bRet = query.exec(QString("insert into ReferringPhysicianName values(%0, 'Default ReferringPhysicianName')").arg(readNextSequence("ReferringPhysicianName", true)));
			bRet = query.exec(QString("insert into PreMedication values(%0, 'Default PreMedication')").arg(readNextSequence("PreMedication", true)));
			bRet = true;
		}
		else
			bRet = false;
	}
	return bRet;

}

//-----------------------------------------------------------------------------------------------------------
int AppSqliteDatabase::readNextSequence(const QString &qsTable, const bool &bWriteIt)
{
	int iNextSequence = -1;
	bool bRet = false;
	QSqlQuery query;
	QString qsToExec = QString("select CURRENT_VALUE from ALL_SEQUENCES where TABLE_NAME = '%0'").arg(qsTable);
	if( (bRet = query.exec(qsToExec)) == true )
	{
		if(  query.next() == true )
		{
			iNextSequence = query.value(0).toInt()+1;
			if( bWriteIt == true )
			{
				qsToExec = QString("update ALL_SEQUENCES set CURRENT_VALUE = %0 where TABLE_NAME = '%1'").arg(iNextSequence).arg(qsTable);
				bRet = query.exec(qsToExec);
			}
		}
	}

	return iNextSequence;
}


//-----------------------------------------------------------------------------------------------------------
bool AppSqliteDatabase::addModifyWorklistMember(const structWorklistMemberTable data)
{
	bool bRet = false;
	QSqlQuery query;
	QString qsToexec ="insert or replace into WorklistSCP values('" + data.PatientName + "',"
										+ "'" + data.PatientBirthDate + "',"
										+ "'" + data.PatientID + "',"
										+ "'" + data.AccessionNumber + "',"
										+ "'" + data.PatientSex + "',"
										+ "'" + data.Modality + "',"
										+ "'" + data.ScheduledProcedureStepStartDate + "',"
										+ "'" + data.ScheduledProcedureStepStartTime + "',"
										+ "'" + data.ScheduledPerformingPhysicianName + "',"
										+ "'" + data.SpecificCharacterSet + "',"
										+ "'" + data.ScheduledProcedureStepSequence + "',"
										+ "'" + data.ScheduledStationAETitle + "',"
										+ "'" + data.ScheduledProcedureStepDescription + "',"
										+ "'" + data.ScheduledStationName + "',"
										+ "'" + data.ScheduledProcedureStepLocation + "',"
										+ "'" + data.PreMedication + "',"
										+ "'" + data.ScheduledProcedureStepID + "',"
										+ "'" + data.RequestedContrastAgent + "',"
										+ "'" + data.RequestedProcedureID + "',"
										+ "'" + data.RequestedProcedureDescription + "',"
										+ "'" + data.StudyInstanceUID + "',"
										+ "'" + data.RequestedProcedurePriority + "',"
										+ "'" + data.PatientTransportArrangements + "',"
										+ "'" + data.RequestingPhysician + "',"
										+ "'" + data.ReferringPhysicianName + "',"
										+ "'" + data.AdmissionID + "',"
										+ "'" + data.CurrentPatientLocation + "',"
										+ "'" + data.PatientWeight + "',"
										+ "'" + data.MedicalAlerts + "',"
										+ "'" + data.ContrastAllergies + "'" + ")";
	bRet = query.exec(qsToexec);
	return bRet;
}



//-----------------------------------------------------------------------------------------------------------
bool AppSqliteDatabase::deleteWorklistMember(const structWorklistMemberTable data)
{
	bool bRet = false;
	QSqlQuery query;
	QString qsToexec = QString("delete from WorklistSCP where tagPatientID = %0").arg(data.PatientID);
	bRet = query.exec(qsToexec);
	return bRet;
}


//-----------------------------------------------------------------------------------------------------------
QStringList AppSqliteDatabase::readOneTableField(const QString &qsTable, const QString &qsFieldName)
{
	QStringList qsl;
	bool bRet = false;
	QSqlQuery query;
	QString qsToExec = QString("select %0 from %1").arg(qsFieldName).arg(qsTable);
	if( (bRet = query.exec(qsToExec)) == true )
	{
		while(  query.next() == true )
		{
			qsl.append(query.value(0).toString());
		}
	}

	return qsl;
}
