#include "WorklistEnrDlg.h"
#include <QMessageBox.h>
#include <QLocale.h>
#include <QDateTime.h>
#include <QSqlField.h>
#include <QCompleter.h>
#include "..\..\DicomCommon\Util\Tools.h"

WorklistEnrDlg::WorklistEnrDlg(const structWorklistMemberTable data, QWidget *parent)
	: QDialog(parent)
	, m_WorklistMemberData(data)
{
	ui.setupUi(this);
	settings = SettingsManager::instance();
	//initialize GroupBox border size, color, radius 
	ui.GB_ImagingServiceRequest->setStyleSheet("QGroupBox {border: 2px solid green;border-radius: 4px}");
	ui.GB_Patient->setStyleSheet("QGroupBox {border: 2px solid green;border-radius: 4px}");
	ui.GB_PatientMedical->setStyleSheet("QGroupBox {border: 2px solid green;border-radius: 4px}");
	ui.GB_RequestedProcedure->setStyleSheet("QGroupBox {border: 2px solid green;border-radius: 4px}");
	ui.GB_ScheduledProcedureStep->setStyleSheet("QGroupBox {border: 2px solid green;border-radius: 4px}");

	//initialize the comboBox with table's data and active the autocompletion 
	initializeScheduledStationAETitle();
	ui.CBX_ScheduledStationAETitle->installEventFilter(this);
	initializeScheduledStationName();
	ui.CBX_ScheduledStationName->installEventFilter(this);
	initializeScheduledPerformingPhysicianName();
	ui.CBX_ScheduledPerformingPhysicianName->installEventFilter(this);
	initializeRequestingPhysician();
	ui.CBX_RequestingPhysician->installEventFilter(this);
	initializeReferringPhysicianName();
	ui.CBX_ReferringPhysicianName->installEventFilter(this);
	initializePreMedication();
	ui.CBX_PreMedication->installEventFilter(this);
	initializeMedicalAlerts();
	ui.CBX_MedicalAlerts->installEventFilter(this);
	initializeContrastAllergies();
	ui.CBX_ContrastAllergies->installEventFilter(this);

	QLocale locale;
	ui.L_BirthDateFormat->setText(locale.dateFormat(QLocale::ShortFormat));
	ui.L_StartDateFormat->setText(locale.dateFormat(QLocale::ShortFormat));
	ui.L_StartTimeFormat->setText(locale.timeFormat(QLocale::LongFormat));

	QStringList qslPN = m_WorklistMemberData.PatientName.split("^");
	if( qslPN.length() >= 2 )
	{
		ui.LE_FamilyName->setText(qslPN[0]);
		ui.LE_GivenName->setText(qslPN[1]);
		if( qslPN.length() >= 3 )
			ui.LE_MiddleName->setText(qslPN[2]);
	}

	if( data.ScheduledProcedureStepStartDate == "" )
	{
		QDateTime qd = QDateTime::currentDateTime();
		ui.LE_ScheduledProcedureStepStartDate->setText(qd.toString(locale.dateFormat(QLocale::ShortFormat)));
		ui.LE_ScheduledProcedureStepStartTime->setText(qd.toString(locale.timeFormat(QLocale::LongFormat)));
		ui.CBX_SpecificCharacterSet->setCurrentIndex(0);
		QDateTime qdt = QDateTime::currentDateTime();
		QString qsTmp = qdt.toString("yyyyMMddHHmmsszzz");
		qint64 i64 = qsTmp.mid(0, DS_LENGTH).toLongLong();
		ui.LE_PatientID->setText(QString("%0").arg(i64));
		ui.LE_ScheduledProcedureStepID->setText(QString("%0").arg(i64+1));
		ui.LE_RequestedProcedureID->setText(QString("%0").arg(i64+2));
		ui.LE_AccessionNumber->setText(QString("%0").arg(i64+3));
		ui.LE_StudyInstanceUID->setText(QString("%0").arg(i64+4));
		ui.CBX_RequestedProcedurePriority->setCurrentIndex(0);
		ui.LE_ScheduledProcedureStepDescription->setText("SPSD");
		ui.LE_RequestedProcedureDescription->setText("RPD");
	}
	else
	{
		ui.LE_PatientBirthDate->setText(m_WorklistMemberData.PatientBirthDate);
		ui.CBX_PatientSex->setCurrentIndex(ui.CBX_PatientSex->findText(m_WorklistMemberData.PatientSex));
		ui.LE_PatientID->setText(m_WorklistMemberData.PatientID);
		ui.CBX_SpecificCharacterSet->setCurrentIndex(ui.CBX_SpecificCharacterSet->findText(m_WorklistMemberData.SpecificCharacterSet));

		ui.LE_ScheduledProcedureStepStartDate->setText(m_WorklistMemberData.ScheduledProcedureStepStartDate);
		ui.LE_ScheduledProcedureStepStartTime->setText(m_WorklistMemberData.ScheduledProcedureStepStartTime);
		ui.CBX_Modality->setCurrentIndex(ui.CBX_Modality->findText(m_WorklistMemberData.Modality));
		ui.LE_ScheduledProcedureStepID->setText(m_WorklistMemberData.ScheduledProcedureStepID);
		ui.LE_RequestedProcedureID->setText(m_WorklistMemberData.RequestedProcedureID);
		ui.LE_AccessionNumber->setText(m_WorklistMemberData.AccessionNumber);
		ui.LE_StudyInstanceUID->setText(m_WorklistMemberData.StudyInstanceUID);
		ui.CBX_RequestedProcedurePriority->setCurrentIndex(ui.CBX_RequestedProcedurePriority->findText(m_WorklistMemberData.RequestedProcedurePriority));
		ui.LE_ScheduledProcedureStepDescription->setText(m_WorklistMemberData.ScheduledProcedureStepDescription);
		ui.LE_RequestedProcedureDescription->setText(m_WorklistMemberData.RequestedProcedureDescription);
	}
	ui.CBX_ScheduledStationAETitle->setCurrentIndex(ui.CBX_ScheduledStationAETitle->findText(m_WorklistMemberData.ScheduledStationAETitle));
	ui.CBX_ScheduledPerformingPhysicianName->setCurrentIndex(ui.CBX_ScheduledPerformingPhysicianName->findText(m_WorklistMemberData.ScheduledPerformingPhysicianName));

	ui.LE_PatientWeight->setText(m_WorklistMemberData.PatientWeight);
	ui.CBX_ScheduledStationName->setCurrentIndex(ui.CBX_ScheduledStationName->findText(m_WorklistMemberData.ScheduledStationName));
	ui.LE_ScheduledProcedureStepDescription->setText(m_WorklistMemberData.ScheduledProcedureStepDescription);
	ui.LE_ScheduledProcedureStepLocation->setText(m_WorklistMemberData.ScheduledProcedureStepLocation);
	ui.CBX_PreMedication->setCurrentIndex(ui.CBX_PreMedication->findText(m_WorklistMemberData.PreMedication));
	ui.LE_RequestedContrastAgent->setText(m_WorklistMemberData.RequestedContrastAgent);
	ui.LE_PatientTransportArrangements->setText(m_WorklistMemberData.PatientTransportArrangements);

	ui.CBX_RequestingPhysician->setCurrentIndex(ui.CBX_RequestingPhysician->findText(m_WorklistMemberData.RequestingPhysician));
	ui.CBX_ReferringPhysicianName->setCurrentIndex(ui.CBX_ReferringPhysicianName->findText(m_WorklistMemberData.ReferringPhysicianName));
	ui.CBX_MedicalAlerts->setCurrentIndex(ui.CBX_MedicalAlerts->findText(m_WorklistMemberData.MedicalAlerts));
	ui.CBX_ContrastAllergies->setCurrentIndex(ui.CBX_ContrastAllergies->findText(m_WorklistMemberData.ContrastAllergies));

	m_sqlRecord.clearValues();

}

WorklistEnrDlg::~WorklistEnrDlg()
{

}

//---------------------------------------------------------------------------------
bool WorklistEnrDlg::eventFilter(QObject *object, QEvent *e)
{
	bool bRet = true;
	switch(e->type())
	{
	case QEvent::FocusOut	:	if( object->objectName() == ui.CBX_ScheduledStationAETitle->objectName() )
									on_CBX_ScheduledStationAETitle_focusOut();
								else if( object->objectName() == ui.CBX_ScheduledPerformingPhysicianName->objectName() )
									on_CBX_ScheduledPerformingPhysicianName_focusOut();
								else if( object->objectName() == ui.CBX_RequestingPhysician->objectName() )
									on_CBX_RequestingPhysician_focusOut();
								else if( object->objectName() == ui.CBX_ReferringPhysicianName->objectName() )
									on_CBX_ReferringPhysicianName_focusOut();
								else if( object->objectName() == ui.CBX_PreMedication->objectName() )
									on_CBX_PreMedication_focusOut();
								else if( object->objectName() == ui.CBX_ScheduledStationName->objectName() )
									on_CBX_ScheduledStationName_focusOut();
								else if( object->objectName() == ui.CBX_MedicalAlerts->objectName() )
									on_CBX_MedicalAlerts_focusOut();
								else if( object->objectName() == ui.CBX_ContrastAllergies->objectName() )
									on_CBX_ContrastAllergies_focusOut();
								break;
	default					:	
								break;
	}
	bRet  = QWidget::eventFilter(object, e);
	return bRet;
}


//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeScheduledStationAETitle(void)
{
	QCompleter *completer = new QCompleter(settings->getScheduledStationAETitleList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_ScheduledStationAETitle->setCompleter(completer);
	ui.CBX_ScheduledStationAETitle->clear();
	ui.CBX_ScheduledStationAETitle->addItems(settings->getScheduledStationAETitleList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_ScheduledStationAETitle_focusOut()
{
	if( ui.CBX_ScheduledStationAETitle->currentText() != "" )
	{
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblScheduledStationAETitle);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblScheduledStationAETitle);
		qsMapFieldsValues["ScheduledStationAETitleID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblScheduledStationAETitle, false));
		qsMapFieldsValues["ScheduledStationAETitleVALUE"] = ui.CBX_ScheduledStationAETitle->currentText();
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblScheduledStationAETitle, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this ScheduledStationAETitle doses not exist\n%0\nDo you want to add it ?").arg(ui.CBX_ScheduledStationAETitle->currentText()), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblScheduledStationAETitle, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["ScheduledStationAETitleID"] = settings->getAppSqliteDatabase().readNextSequence(tblScheduledStationAETitle, true);
					settings->refreshScheduledStationAETitleList();
					initializeScheduledStationAETitle();
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeScheduledPerformingPhysicianName(void)
{
	QCompleter *completer = new QCompleter(settings->getScheduledPerformingPhysicianNameList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_ScheduledPerformingPhysicianName->setCompleter(completer);
	ui.CBX_ScheduledPerformingPhysicianName->clear();
	ui.CBX_ScheduledPerformingPhysicianName->addItems(settings->getScheduledPerformingPhysicianNameList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_ScheduledPerformingPhysicianName_focusOut()
{
	if( ui.CBX_ScheduledPerformingPhysicianName->currentText() != "" )
	{
		QString qsToAdd = Tools::upperCaseFirst(ui.CBX_ScheduledPerformingPhysicianName->currentText());
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblScheduledPerformingPhysicianName);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblScheduledPerformingPhysicianName);
		qsMapFieldsValues["ScheduledPerformingPhysicianNameID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblScheduledPerformingPhysicianName, false));
		qsMapFieldsValues["ScheduledPerformingPhysicianNameVALUE"] = qsToAdd;
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblScheduledPerformingPhysicianName, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this ScheduledPerformingPhysicianName doses not exist\n%0\nDo you want to add it ?").arg(qsToAdd), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblScheduledPerformingPhysicianName, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["ScheduledPerformingPhysicianNameID"] = settings->getAppSqliteDatabase().readNextSequence(tblScheduledPerformingPhysicianName, true);
					settings->refreshScheduledPerformingPhysicianNameList();
					initializeScheduledPerformingPhysicianName();
					ui.CBX_ScheduledPerformingPhysicianName->setItemText(ui.CBX_ScheduledPerformingPhysicianName->currentIndex(), qsToAdd);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeRequestingPhysician(void)
{
	QCompleter *completer = new QCompleter(settings->getRequestingPhysicianList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_RequestingPhysician->setCompleter(completer);
	ui.CBX_RequestingPhysician->clear();
	ui.CBX_RequestingPhysician->addItems(settings->getRequestingPhysicianList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_RequestingPhysician_focusOut()
{
	if( ui.CBX_RequestingPhysician->currentText() != "" )
	{
		QString qsToAdd = Tools::upperCaseFirst(ui.CBX_RequestingPhysician->currentText());
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblRequestingPhysician);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblRequestingPhysician);
		qsMapFieldsValues["RequestingPhysicianID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblRequestingPhysician, false));
		qsMapFieldsValues["RequestingPhysicianVALUE"] = qsToAdd;
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblRequestingPhysician, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this RequestingPhysician doses not exist\n%0\nDo you want to add it ?").arg(qsToAdd), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblRequestingPhysician, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["RequestingPhysicianID"] = settings->getAppSqliteDatabase().readNextSequence(tblRequestingPhysician, true);
					settings->refreshRequestingPhysicianList();
					initializeRequestingPhysician();
					ui.CBX_RequestingPhysician->setItemText(ui.CBX_RequestingPhysician->currentIndex(), qsToAdd);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeReferringPhysicianName(void)
{
	QCompleter *completer = new QCompleter(settings->getReferringPhysicianNameList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_ReferringPhysicianName->setCompleter(completer);
	ui.CBX_ReferringPhysicianName->clear();
	ui.CBX_ReferringPhysicianName->addItems(settings->getReferringPhysicianNameList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_ReferringPhysicianName_focusOut()
{
	if( ui.CBX_ReferringPhysicianName->currentText() != "" )
	{
		QString qsToAdd = Tools::upperCaseFirst(ui.CBX_ReferringPhysicianName->currentText());
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblReferringPhysicianName);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblReferringPhysicianName);
		qsMapFieldsValues["ReferringPhysicianNameID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblReferringPhysicianName, false));
		qsMapFieldsValues["ReferringPhysicianNameVALUE"] = qsToAdd;
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblReferringPhysicianName, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this ReferringPhysicianName doses not exist\n%0\nDo you want to add it ?").arg(qsToAdd), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblReferringPhysicianName, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["ReferringPhysicianNameID"] = settings->getAppSqliteDatabase().readNextSequence(tblReferringPhysicianName, true);
					settings->refreshReferringPhysicianNameList();
					initializeReferringPhysicianName();
					ui.CBX_ReferringPhysicianName->setItemText(ui.CBX_ReferringPhysicianName->currentIndex(), qsToAdd);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializePreMedication(void)
{
	QCompleter *completer = new QCompleter(settings->getPreMedicationList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_PreMedication->setCompleter(completer);
	ui.CBX_PreMedication->clear();
	ui.CBX_PreMedication->addItems(settings->getPreMedicationList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_PreMedication_focusOut()
{
	if( ui.CBX_PreMedication->currentText() != "" )
	{
		QString qsToAdd = Tools::upperCaseFirst(ui.CBX_PreMedication->currentText());
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblPreMedication);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblPreMedication);
		qsMapFieldsValues["PreMedicationID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblPreMedication, false));
		qsMapFieldsValues["PreMedicationVALUE"] = qsToAdd;
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblPreMedication, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this PreMedication doses not exist\n%0\nDo you want to add it ?").arg(qsToAdd), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblPreMedication, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["PreMedicationID"] = settings->getAppSqliteDatabase().readNextSequence(tblPreMedication, true);
					settings->refreshPreMedicationList();
					initializePreMedication();
					ui.CBX_PreMedication->setItemText(ui.CBX_PreMedication->currentIndex(), qsToAdd);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeMedicalAlerts(void)
{
	QCompleter *completer = new QCompleter(settings->getMedicalAlertsList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_MedicalAlerts->setCompleter(completer);
	ui.CBX_MedicalAlerts->clear();
	ui.CBX_MedicalAlerts->addItems(settings->getMedicalAlertsList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_MedicalAlerts_focusOut()
{
	if( ui.CBX_MedicalAlerts->currentText() != "" )
	{
		QString qsToAdd = Tools::upperCaseFirst(ui.CBX_MedicalAlerts->currentText());
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblMedicalAlerts);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblMedicalAlerts);
		qsMapFieldsValues["MedicalAlertsID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblMedicalAlerts, false));
		qsMapFieldsValues["MedicalAlertsVALUE"] = qsToAdd;
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblMedicalAlerts, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this MedicalAlerts doses not exist\n%0\nDo you want to add it ?").arg(qsToAdd), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblMedicalAlerts, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["MedicalAlertsID"] = settings->getAppSqliteDatabase().readNextSequence(tblMedicalAlerts, true);
					settings->refreshMedicalAlertsList();
					initializeMedicalAlerts();
					ui.CBX_MedicalAlerts->setItemText(ui.CBX_MedicalAlerts->currentIndex(), qsToAdd);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeContrastAllergies(void)
{
	QCompleter *completer = new QCompleter(settings->getContrastAllergiesList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_ContrastAllergies->setCompleter(completer);
	ui.CBX_ContrastAllergies->clear();
	ui.CBX_ContrastAllergies->addItems(settings->getContrastAllergiesList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_ContrastAllergies_focusOut()
{
	if( ui.CBX_ContrastAllergies->currentText() != "" )
	{
		QString qsToAdd = Tools::upperCaseFirst(ui.CBX_ContrastAllergies->currentText());
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblContrastAllergies);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblContrastAllergies);
		qsMapFieldsValues["ContrastAllergiesID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblContrastAllergies, false));
		qsMapFieldsValues["ContrastAllergiesVALUE"] = qsToAdd;
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblContrastAllergies, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this ContrastAllergies doses not exist\n%0\nDo you want to add it ?").arg(qsToAdd), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblContrastAllergies, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["ContrastAllergiesID"] = settings->getAppSqliteDatabase().readNextSequence(tblContrastAllergies, true);
					settings->refreshContrastAllergiesList();
					initializeContrastAllergies();
					ui.CBX_ContrastAllergies->setItemText(ui.CBX_ContrastAllergies->currentIndex(), qsToAdd);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::initializeScheduledStationName(void)
{
	QCompleter *completer = new QCompleter(settings->getScheduledStationNameList(), this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.CBX_ScheduledStationName->setCompleter(completer);
	ui.CBX_ScheduledStationName->clear();
	ui.CBX_ScheduledStationName->addItems(settings->getScheduledStationNameList());
}
//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_CBX_ScheduledStationName_focusOut()
{
	if( ui.CBX_ScheduledStationName->currentText() != "" )
	{
		QList<CSqliteDatabase::structTable> qlstFieldsStruct = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).DBgetStructTableWithDetails(tblScheduledStationName);
		QMap<QString, QString> qsMapFieldsValues = ((CSqliteDatabase &) settings->getAppSqliteDatabase()).copyTableStructToQMap(tblScheduledStationName);
		qsMapFieldsValues["ScheduledStationNameID"] = QString("%0").arg(settings->getAppSqliteDatabase().readNextSequence(tblScheduledStationName, false));
		qsMapFieldsValues["ScheduledStationNameVALUE"] = ui.CBX_ScheduledStationName->currentText();
		if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).testExistenceElement(tblScheduledStationName, qlstFieldsStruct, qsMapFieldsValues) == false )
		{
			if( QMessageBox::question(this, "Element to Add", QString("this ScheduledStationName doses not exist\n%0\nDo you want to add it ?").arg(ui.CBX_ScheduledStationName->currentText()), QMessageBox::Yes , QMessageBox::No) == QMessageBox::Yes )
			{
				if( ((CSqliteDatabase &) settings->getAppSqliteDatabase()).insertOrReplaceIntoTable(tblScheduledStationName, qlstFieldsStruct, qsMapFieldsValues) == true )
				{
					qsMapFieldsValues["ScheduledStationNameID"] = settings->getAppSqliteDatabase().readNextSequence(tblScheduledStationName, true);
					settings->refreshScheduledStationNameList();
					initializeScheduledStationName();
				}
			}
		}
	}
}



//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_PB_Ok_clicked()
{
	if( verifyInputs() == true )
		QDialog::accept();
}

//---------------------------------------------------------------------------------
void WorklistEnrDlg::on_PB_Cancel_clicked()
{
	QDialog::reject();
}

//---------------------------------------------------------------------------------
bool WorklistEnrDlg::verifyInputs(void)
{
	bool bRet = true;
	QString qsError = "";
	QSqlField sqlField(QString(), QVariant::String);
	for(int i =0; (i < 13) && (qsError == ""); i++)
	{
		switch(i)
		{
		case 0:		{
						if( (ui.LE_FamilyName->text() != "" ) && (ui.LE_GivenName->text() != "") )
						{
							m_WorklistMemberData.PatientName = ui.LE_FamilyName->text() + "^" + ui.LE_GivenName->text();
							if( ui.LE_MiddleName->text() != "" )
								m_WorklistMemberData.PatientName = m_WorklistMemberData.PatientName + "^" + ui.LE_MiddleName->text();
							if( m_WorklistMemberData.PatientName.length() > PN_LENGTH+1 )
								qsError = QString(tr("Total patient's name length must be inferior to ") + "%0").arg(PN_LENGTH+1);
							m_sqlRecord.setValue("tagPatientName", m_WorklistMemberData.PatientName);
						}
						else
							qsError = tr("Family name and given name must not be empty string");
					}
					break;
		case 1:		{
						QLocale locale;
						QDateTime qd = QDateTime::fromString(ui.LE_PatientBirthDate->text(), locale.dateFormat(QLocale::ShortFormat));
						if( qd.isValid() == false )
							qsError = tr("Patient's BirthDate must be valid");
						else
						{
							m_WorklistMemberData.PatientBirthDate = ui.LE_PatientBirthDate->text();
							m_sqlRecord.setValue("tagPatientBirthDate", m_WorklistMemberData.PatientBirthDate);
						}
					}
					break;
		case 2:		{
						if( ui.CBX_PatientSex->currentIndex() == -1 )
							qsError = tr("You must select Sex");
						else
						{
							m_WorklistMemberData.PatientSex = ui.CBX_PatientSex->currentText();
							m_sqlRecord.setValue("tagPatientSex", m_WorklistMemberData.PatientSex);
						}
					}
					break;
		case 3:		{
						if( ui.LE_PatientID->text() != "" )
						{
							m_WorklistMemberData.PatientID = ui.LE_PatientID->text();
							m_sqlRecord.setValue("tagPatientID", m_WorklistMemberData.PatientID);
							if( m_WorklistMemberData.PatientID.length() > LO_LENGTH+1 )
								qsError = QString(tr("Patient's ID length must be inferior to ") + "%0").arg(LO_LENGTH+1);
						}
						else
							qsError = tr("Patient's ID must not be empty string");
					}
					break;
		case 4:		{
						if( ui.CBX_ScheduledStationAETitle->currentText() != "" )
						{
							m_WorklistMemberData.ScheduledStationAETitle = ui.CBX_ScheduledStationAETitle->currentText();
							m_sqlRecord.setValue("tagScheduledStationAETitle", m_WorklistMemberData.ScheduledStationAETitle);
							if( m_WorklistMemberData.ScheduledStationAETitle.length() > AE_LENGTH+1 )
								qsError = QString(tr("ScheduledStationAETitle length must be inferior to ") + "%0").arg(AE_LENGTH+1);
						}
						else
							qsError = tr("ScheduledStationAETitle must not be empty string");
					}
					break;
		case 5:		{
						QLocale locale;
						QDateTime qd = QDateTime::fromString(ui.LE_ScheduledProcedureStepStartDate->text(), locale.dateFormat(QLocale::ShortFormat));
						if( qd.isValid() == false )
							qsError = tr("ScheduledProcedureStepStartDate must be valid");
						else
						{
							m_WorklistMemberData.ScheduledProcedureStepStartDate = ui.LE_ScheduledProcedureStepStartDate->text();
							m_sqlRecord.setValue("tagScheduledProcedureStepStartDate", m_WorklistMemberData.ScheduledProcedureStepStartDate);
						}
					}
					break;
		case 6:		{
						QLocale locale;
						QDateTime qd = QDateTime::fromString(ui.LE_ScheduledProcedureStepStartTime->text(), locale.timeFormat(QLocale::LongFormat));
						if( qd.isValid() == false )
							qsError = tr("ScheduledProcedureStepStartTime must be valid");
						else
						{
							m_WorklistMemberData.ScheduledProcedureStepStartTime = ui.LE_ScheduledProcedureStepStartTime->text();
							m_sqlRecord.setValue("tagScheduledProcedureStepStartTime", m_WorklistMemberData.ScheduledProcedureStepStartTime);
						}
					}
					break;
		case 7:		{
						if( ui.CBX_Modality->currentIndex() == -1 )
							qsError = tr("You must select Modality");
						else
						{
							m_WorklistMemberData.Modality = ui.CBX_Modality->currentText();
							m_sqlRecord.setValue("tagModality", m_WorklistMemberData.Modality);
						}
					}
					break;
		case 8:		{
						if( ui.CBX_ScheduledPerformingPhysicianName->currentText() != "" )
						{
							m_WorklistMemberData.ScheduledPerformingPhysicianName = ui.CBX_ScheduledPerformingPhysicianName->currentText();
							m_sqlRecord.setValue("tagScheduledPerformingPhysicianName", m_WorklistMemberData.ScheduledPerformingPhysicianName);
							if( m_WorklistMemberData.ScheduledPerformingPhysicianName.length() > PN_LENGTH+1)
								qsError = QString(tr("ScheduledPerformingPhysicianName length must be inferior to ") + "%0").arg(PN_LENGTH+1);
						}
						else
							qsError = tr("ScheduledPerformingPhysicianName must not be empty string");
					}
					break;
		case 9:		{
						if( ui.LE_AccessionNumber->text() != "" )
						{
							m_WorklistMemberData.AccessionNumber = ui.LE_AccessionNumber->text();
							m_sqlRecord.setValue("tagAccessionNumber", m_WorklistMemberData.AccessionNumber);
							if( m_WorklistMemberData.AccessionNumber.length() > SH_LENGTH+1 )
								qsError = QString(tr("AccessionNumber length must be inferior to ") + "%0").arg(SH_LENGTH+1);
						}
						else
							qsError = tr("AccessionNumber must not be empty string");
					}
					break;
		case 10:		{
						if( ui.LE_ScheduledProcedureStepDescription->text() != "" )
						{
							m_WorklistMemberData.ScheduledProcedureStepDescription = ui.LE_ScheduledProcedureStepDescription->text();
							m_sqlRecord.setValue("tagScheduledProcedureStepDescription", m_WorklistMemberData.ScheduledProcedureStepDescription);
							if( m_WorklistMemberData.ScheduledProcedureStepDescription.length() > LO_LENGTH+1 )
								qsError = QString(tr("ScheduledProcedureStepDescription length must be inferior to ") + "%0").arg(LO_LENGTH+1);
						}
						else
							qsError = tr("ScheduledProcedureStepDescription must not be empty string");
					}
					break;
		case 11:		{
						if( ui.LE_RequestedProcedureDescription->text() != "" )
						{
							m_WorklistMemberData.RequestedProcedureDescription = ui.LE_RequestedProcedureDescription->text();
							m_sqlRecord.setValue("tagRequestedProcedureDescription", m_WorklistMemberData.RequestedProcedureDescription);
							if( m_WorklistMemberData.RequestedProcedureDescription.length() > LO_LENGTH+1 )
								qsError = QString(tr("RequestedProcedureDescription length must be inferior to ") + "%0").arg(LO_LENGTH+1);
						}
						else
							qsError = tr("RequestedProcedureDescription must not be empty string");
					}
					break;
		case 12:		{
						if( ui.LE_StudyInstanceUID->text() != "" )
						{
							m_WorklistMemberData.StudyInstanceUID = ui.LE_StudyInstanceUID->text();
							m_sqlRecord.setValue("tagStudyInstanceUID", m_WorklistMemberData.StudyInstanceUID);
							if( m_WorklistMemberData.StudyInstanceUID.length() > UI_LENGTH+1 )
								qsError = QString(tr("StudyInstanceUID length must be inferior to ") + "%0").arg(UI_LENGTH+1);
						}
						else
							qsError = tr("StudyInstanceUID must not be empty string");
					}
					break;
		}
	}
	if( qsError != "" )
	{
		QMessageBox::warning(this,"EDAPdicom", qsError, QMessageBox::Ok);
		bRet = false;
	}
	else
	{
		m_WorklistMemberData.PatientWeight						= ui.LE_PatientWeight->text();
		m_sqlRecord.setValue("tagPatientWeight", m_WorklistMemberData.PatientWeight);
		m_WorklistMemberData.SpecificCharacterSet				= ui.CBX_SpecificCharacterSet->currentText();
		m_sqlRecord.setValue("tagSpecificCharacterSet", m_WorklistMemberData.SpecificCharacterSet);
		m_WorklistMemberData.ScheduledStationName				= ui.CBX_ScheduledStationName->currentText();
		m_sqlRecord.setValue("tagScheduledStationName", m_WorklistMemberData.ScheduledStationName);
		m_WorklistMemberData.ScheduledProcedureStepLocation		= ui.LE_ScheduledProcedureStepLocation->text();
		m_sqlRecord.setValue("tagScheduledProcedureStepLocation", m_WorklistMemberData.ScheduledProcedureStepLocation);
		m_WorklistMemberData.PreMedication						= ui.CBX_PreMedication->currentText();
		m_sqlRecord.setValue("tagPreMedication", m_WorklistMemberData.PreMedication);
		m_WorklistMemberData.ScheduledProcedureStepID			= ui.LE_ScheduledProcedureStepID->text();
		m_sqlRecord.setValue("tagScheduledProcedureStepID", m_WorklistMemberData.ScheduledProcedureStepID);
		m_WorklistMemberData.RequestedContrastAgent				= ui.LE_RequestedContrastAgent->text();
		m_sqlRecord.setValue("tagRequestedContrastAgent", m_WorklistMemberData.RequestedContrastAgent);
		m_WorklistMemberData.RequestedProcedureID				= ui.LE_RequestedProcedureID->text();
		m_sqlRecord.setValue("tagRequestedProcedureID", m_WorklistMemberData.RequestedProcedureID);

//		m_WorklistMemberData.CurrentPatientLocation = ui.LE_ScheduledProcedureStepLocation->text();
//		m_sqlRecord.setValue("tagScheduledProcedureStepLocation", m_WorklistMemberData.CurrentPatientLocation);

		m_WorklistMemberData.RequestedProcedurePriority			= ui.CBX_RequestedProcedurePriority->currentText();
		m_sqlRecord.setValue("tagRequestedProcedurePriority", m_WorklistMemberData.RequestedProcedurePriority);
		m_WorklistMemberData.PatientTransportArrangements		= ui.LE_PatientTransportArrangements->text();
		m_sqlRecord.setValue("tagPatientTransportArrangements", m_WorklistMemberData.PatientTransportArrangements);
		m_WorklistMemberData.RequestingPhysician				= ui.CBX_RequestingPhysician->currentText();
		m_sqlRecord.setValue("tagRequestingPhysician", m_WorklistMemberData.RequestingPhysician);
		m_WorklistMemberData.ReferringPhysicianName				= ui.CBX_ReferringPhysicianName->currentText();
		m_sqlRecord.setValue("tagReferringPhysicianName", m_WorklistMemberData.ReferringPhysicianName);
		m_WorklistMemberData.MedicalAlerts						= ui.CBX_MedicalAlerts->currentText();
		m_sqlRecord.setValue("tagMedicalAlerts", m_WorklistMemberData.MedicalAlerts);
		m_WorklistMemberData.ContrastAllergies					= ui.CBX_ContrastAllergies->currentText();
		m_sqlRecord.setValue("tagContrastAllergies", m_WorklistMemberData.ContrastAllergies);
	}
	return bRet;
}
