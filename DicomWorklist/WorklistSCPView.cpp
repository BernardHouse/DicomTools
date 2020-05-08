#include "WorklistSCPView.h"
#include "..\..\DicomCommon\CommonDefines.h"
#include "WorklistEnrDlg.h"
#include "..\..\DicomCommon\Util\Tools.h"
#include "Defines.h"

#include "DcmDataset.h"
#include "DcmFile.h"
#include "DcmTagSQ.h"
#include "DcmTagUI.h"
#include "DcmTagCS.h"
#include "DcmTagDS.h"
#include "DcmTagLO.h"
#include "DcmTagAE.h"
#include "DcmTagDA.h"
#include "DcmTagTM.h"
#include "DcmTagPN.h"
#include "DcmTagSH.h"
#include "DcmTagLT.h"


WorklistSCPView::WorklistSCPView(QWidget *parent)
	: QWidget(parent)
	, m_pWorklistSCPModel(NULL)
{
	ui.setupUi(this);
	settings = SettingsManager::instance();
	cleanWorklistMemberData();

	if( settings->getAppSqliteDatabase().m_DB.isOpen() == true )
	{
		//With TableView
		m_pWorklistSCPModel = new QWLSqlTableModel(this, settings->getAppSqliteDatabase().m_DB/*m_DBapp.m_DB*/);
		m_pWorklistSCPModel->setTable("WorklistSCP");
		m_pWorklistSCPModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
		m_pWorklistSCPModel->setHeaderData( PatientNameColumn0							, Qt::Horizontal, tr("Patient's name"));
		m_pWorklistSCPModel->setHeaderData( PatientBirthDateColumn1						, Qt::Horizontal, tr("Birth Date"));
		m_pWorklistSCPModel->setHeaderData( PatientIDColumn2							, Qt::Horizontal, tr("Patient ID"));
		m_pWorklistSCPModel->setHeaderData( AccessionNumberColumn3						, Qt::Horizontal, tr("Accession number"));
		m_pWorklistSCPModel->setHeaderData( PatientSexColumn4							, Qt::Horizontal, tr("Sex"));
		m_pWorklistSCPModel->setHeaderData( ModalityColumn5								, Qt::Horizontal, tr("Modality"));
		m_pWorklistSCPModel->setHeaderData( ScheduledProcedureStepStartDateColumn6		, Qt::Horizontal, tr("SPS Start date"));
		m_pWorklistSCPModel->setHeaderData( ScheduledProcedureStepStartTimeColumn7		, Qt::Horizontal, tr("SPS Start time"));
		m_pWorklistSCPModel->setHeaderData( ScheduledPerformingPhysiciansNameColumn8	, Qt::Horizontal, tr("Performing Physician"));
		m_pWorklistSCPModel->setHeaderData( SpecificCharacterSetColumn9					, Qt::Horizontal, tr("Specific Character Set"));
		m_pWorklistSCPModel->setHeaderData( ScheduledProcedureStepSequenceColumn10		, Qt::Horizontal, tr("ScheduledProcedureStepSequence"));	
		m_pWorklistSCPModel->setHeaderData( ScheduledStationAETitleColumn11				, Qt::Horizontal, tr("ScheduledStationAETitle"));	
		m_pWorklistSCPModel->setHeaderData( ScheduledProcedureStepDescriptionColumn12	, Qt::Horizontal, tr("ScheduledProcedureStepDescription"));	
		m_pWorklistSCPModel->setHeaderData( ScheduledStationNameColumn13				, Qt::Horizontal, tr("ScheduledStationName"));
		m_pWorklistSCPModel->setHeaderData( ScheduledProcedureStepLocationColumn14		, Qt::Horizontal, tr("ScheduledProcedureStepLocation"));	
		m_pWorklistSCPModel->setHeaderData( PreMedicationColumn15						, Qt::Horizontal, tr("PreMedication"));	
		m_pWorklistSCPModel->setHeaderData( ScheduledProcedureStepIDColumn16			, Qt::Horizontal, tr("ScheduledProcedureStepID"));
		m_pWorklistSCPModel->setHeaderData( RequestedContrastAgentColumn17				, Qt::Horizontal, tr("RequestedContrastAgent"));	
		m_pWorklistSCPModel->setHeaderData( RequestedProcedureIDColumn18				, Qt::Horizontal, tr("RequestedProcedureID"));
		m_pWorklistSCPModel->setHeaderData( RequestedProcedureDescriptionColumn19		, Qt::Horizontal, tr("RequestedProcedureDescription"));	
		m_pWorklistSCPModel->setHeaderData( StudyInstanceUIDColumn20					, Qt::Horizontal, tr("StudyInstanceUID"));
		m_pWorklistSCPModel->setHeaderData( RequestedProcedurePriorityColumn21			, Qt::Horizontal, tr("RequestedProcedurePriority"));	
		m_pWorklistSCPModel->setHeaderData( PatientTransportArrangementsColumn22		, Qt::Horizontal, tr("PatientTransportArrangements"));
		m_pWorklistSCPModel->setHeaderData( RequestingPhysicianColumn23					, Qt::Horizontal, tr("RequestingPhysician"));	
		m_pWorklistSCPModel->setHeaderData( ReferingPhysicianNameColumn24				, Qt::Horizontal, tr("ReferringPhysicianName"));	
		m_pWorklistSCPModel->setHeaderData( AdmissionIDColumn25							, Qt::Horizontal, tr("AdmissionID"));	
		m_pWorklistSCPModel->setHeaderData( CurrentPatientLocationColumn26				, Qt::Horizontal, tr("CurrentPatientLocation"));	
		m_pWorklistSCPModel->setHeaderData( PatientWeightColumn27						, Qt::Horizontal, tr("PatientWeight"));	
		m_pWorklistSCPModel->setHeaderData( MedicalAlertsColumn28						, Qt::Horizontal, tr("MedicalAlerts"));	
		m_pWorklistSCPModel->setHeaderData( ContrastAllergiesColumn29					, Qt::Horizontal, tr("ContrastAllergies"));	
		m_pWorklistSCPModel->select();

		// set the default sorting order
		ui.TBL_WorklistSCP->setSortingEnabled(true);
		ui.TBL_WorklistSCP->sortByColumn( 0, Qt::AscendingOrder );

		ui.TBL_WorklistSCP->setModel(m_pWorklistSCPModel);

		ui.TBL_WorklistSCP->setColumnHidden(ScheduledProcedureStepSequenceColumn10, true);
		ui.TBL_WorklistSCP->setColumnHidden(AdmissionIDColumn25, true);

		ui.TBL_WorklistSCP->setAlternatingRowColors(true);
		ui.TBL_WorklistSCP->show();
		// resize columns
		ui.TBL_WorklistSCP->resizeColumnsToContents();

		//select all the line and only one
		ui.TBL_WorklistSCP->setSelectionBehavior(QAbstractItemView::SelectRows);
		ui.TBL_WorklistSCP->setSelectionMode(QAbstractItemView::SingleSelection);
		ui.TBL_WorklistSCP->setEditTriggers(QAbstractItemView::NoEditTriggers);

		connect(ui.TBL_WorklistSCP->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, 
		SLOT(TBL_WorklistSCP_selectionChanged(const QItemSelection &, const QItemSelection &)));

		ui.PB_Delete->setEnabled(false);
		ui.PB_Modify->setEnabled(false);
		ui.PB_CreateDcmWl->setEnabled(false);
	}
	else
	{
		QWidget::setEnabled(false);
	}
}

WorklistSCPView::~WorklistSCPView()
{
	if( m_pWorklistSCPModel != NULL)
	{
		delete m_pWorklistSCPModel;
		m_pWorklistSCPModel = NULL;
	}
}


//---------------------------------------------------------------------------------
void WorklistSCPView::cleanWorklistMemberData(void)
{
	m_WorklistMemberData.SpecificCharacterSet = "";
	m_WorklistMemberData.ScheduledProcedureStepSequence = "";
	m_WorklistMemberData.ScheduledStationAETitle = "";
	m_WorklistMemberData.ScheduledProcedureStepStartDate = "";
	m_WorklistMemberData.ScheduledProcedureStepStartTime = "";
	m_WorklistMemberData.Modality = "";
	m_WorklistMemberData.ScheduledPerformingPhysicianName = "";
	m_WorklistMemberData.ScheduledProcedureStepDescription = "";
	m_WorklistMemberData.ScheduledStationName = "";
	m_WorklistMemberData.ScheduledProcedureStepLocation = "";
	m_WorklistMemberData.PreMedication = "";
	m_WorklistMemberData.ScheduledProcedureStepID = "";
	m_WorklistMemberData.RequestedContrastAgent = "";
	m_WorklistMemberData.RequestedProcedureID = "";
	m_WorklistMemberData.RequestedProcedureDescription = "";
	m_WorklistMemberData.StudyInstanceUID = "";
	m_WorklistMemberData.RequestedProcedurePriority = "";
	m_WorklistMemberData.PatientTransportArrangements = "";
	m_WorklistMemberData.AccessionNumber = "";
	m_WorklistMemberData.RequestingPhysician = "";
	m_WorklistMemberData.ReferringPhysicianName = "";
	m_WorklistMemberData.AdmissionID = "";
	m_WorklistMemberData.CurrentPatientLocation = "";
	m_WorklistMemberData.PatientName = "";
	m_WorklistMemberData.PatientID = "";
	m_WorklistMemberData.PatientBirthDate = "";
	m_WorklistMemberData.PatientSex = "";
	m_WorklistMemberData.PatientWeight = "";
	m_WorklistMemberData.MedicalAlerts = "";
	m_WorklistMemberData.ContrastAllergies = "";
}

//---------------------------------------------------------------------------------
void WorklistSCPView::on_PB_Add_clicked()
{
	cleanWorklistMemberData();
	WorklistEnrDlg dlg(m_WorklistMemberData, this);
	QSqlRecord tbl_record = settings->getAppSqliteDatabase().m_DB.record("WorklistSCP");
	dlg.setWorklistMemberSqlRecord(tbl_record);
	if( dlg.exec() == QDialog::Accepted )
	{
		//m_DBapp.addModifyWorklistMember(dlg.getWorklistMemberTableData());
		
		bool bRet = false;
		int iCount = m_pWorklistSCPModel->rowCount();
		if( (bRet = m_pWorklistSCPModel->insertRows(iCount, 1)) == true )
		{
			if( (bRet = m_pWorklistSCPModel->setRecord(iCount, dlg.getWorklistMemberSqlRecord())) == true )
			{
				bRet = m_pWorklistSCPModel->submitAll();
			}
		}
		if( bRet == false )
		{
			QMessageBox::critical(this, "EDAPdicom", tr("Error while inserting record\n") + m_pWorklistSCPModel->lastError().text(), QMessageBox::Ok);
			m_pWorklistSCPModel->revertAll();
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistSCPView::on_PB_Modify_clicked()
{
	if( Tools::isOneTableViewLineSelected(ui.TBL_WorklistSCP, this) == true )
	{
		WorklistEnrDlg dlg(m_WorklistMemberData, this);
		QSqlRecord tbl_record =settings->getAppSqliteDatabase().m_DB.record("WorklistSCP");
		dlg.setWorklistMemberSqlRecord(tbl_record);
		if( dlg.exec() == QDialog::Accepted )
		{
			//m_DBapp.addModifyWorklistMember(dlg.getWorklistMemberTableData());
			bool bRet = false;
			if( (bRet = m_pWorklistSCPModel->setRecord(m_currentSelectedIndex.row(), dlg.getWorklistMemberSqlRecord())) == true )
				bRet = m_pWorklistSCPModel->submitAll();
			if( bRet == false )
				QMessageBox::critical(this, "EDAPdicom", tr("Error while modify record") + m_pWorklistSCPModel->lastError().text(), QMessageBox::Ok);
		}
	}
}

//---------------------------------------------------------------------------------
void WorklistSCPView::on_PB_Delete_clicked()
{
	if( Tools::isOneTableViewLineSelected(ui.TBL_WorklistSCP, this) == true )
	{
		//m_DBapp.deleteWorklistMember(m_WorklistMemberData);
		if(	QMessageBox::question(this,"EDAPdicom", tr("Do you really want to delete this record ?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes )
		{
			bool bRet = false;
			if( (bRet = m_pWorklistSCPModel->removeRows(m_currentSelectedIndex.row(), 1)) == true )
				bRet = m_pWorklistSCPModel->submitAll();
			if( bRet == false )
				QMessageBox::critical(this, "EDAPdicom", tr("Error while deleting record") + m_pWorklistSCPModel->lastError().text(), QMessageBox::Ok);

		}
	}
}

void WorklistSCPView::TBL_WorklistSCP_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	const QModelIndex modelIndex = ui.TBL_WorklistSCP->selectionModel()->currentIndex();
	on_TBL_WorklistSCP_clicked( modelIndex );
}



//----------------------------------------------------------------------------------------
void WorklistSCPView::on_TBL_WorklistSCP_activated( const QModelIndex& index )
{
	on_TBL_WorklistSCP_clicked( index );
}

//---------------------------------------------------------------------------------
void WorklistSCPView::on_TBL_WorklistSCP_clicked(const QModelIndex& index)
{
	ui.PB_Delete->setEnabled(index.isValid());
	ui.PB_Modify->setEnabled(index.isValid());
	ui.PB_CreateDcmWl->setEnabled(index.isValid());
	if( index.isValid() == true )
	{
		// get information of the selected index
		m_currentSelectedIndex = index;
		m_WorklistMemberData.SpecificCharacterSet				= index.sibling(index.row(), SpecificCharacterSetColumn9).data().toString();
	//	m_WorklistMemberData.ScheduledProcedureStepSequence		= index.sibling(index.row(), ScheduledProcedureStepSequenceColumn10).data().toString();
		m_WorklistMemberData.ScheduledStationAETitle			= index.sibling(index.row(), ScheduledStationAETitleColumn11).data().toString();
		m_WorklistMemberData.ScheduledProcedureStepStartDate	= index.sibling(index.row(), ScheduledProcedureStepStartDateColumn6).data().toString();
		m_WorklistMemberData.ScheduledProcedureStepStartTime	= index.sibling(index.row(), ScheduledProcedureStepStartTimeColumn7).data().toString();
		m_WorklistMemberData.Modality							= index.sibling(index.row(), ModalityColumn5).data().toString();
		m_WorklistMemberData.ScheduledPerformingPhysicianName	= index.sibling(index.row(), ScheduledPerformingPhysiciansNameColumn8).data().toString();
		m_WorklistMemberData.ScheduledProcedureStepDescription	= index.sibling(index.row(), ScheduledProcedureStepDescriptionColumn12).data().toString();
		m_WorklistMemberData.ScheduledStationName				= index.sibling(index.row(), ScheduledStationNameColumn13).data().toString();
		m_WorklistMemberData.ScheduledProcedureStepLocation		= index.sibling(index.row(), ScheduledProcedureStepLocationColumn14).data().toString();
		m_WorklistMemberData.PreMedication						= index.sibling(index.row(), PreMedicationColumn15).data().toString();
		m_WorklistMemberData.ScheduledProcedureStepID			= index.sibling(index.row(), ScheduledProcedureStepIDColumn16).data().toString();
		m_WorklistMemberData.RequestedContrastAgent				= index.sibling(index.row(), RequestedContrastAgentColumn17).data().toString();
		m_WorklistMemberData.RequestedProcedureID				= index.sibling(index.row(), RequestedProcedureIDColumn18).data().toString();
		m_WorklistMemberData.RequestedProcedureDescription		= index.sibling(index.row(), RequestedProcedureDescriptionColumn19).data().toString();
		m_WorklistMemberData.StudyInstanceUID					= index.sibling(index.row(), StudyInstanceUIDColumn20).data().toString();
		m_WorklistMemberData.RequestedProcedurePriority			= index.sibling(index.row(), RequestedProcedurePriorityColumn21).data().toString();
		m_WorklistMemberData.PatientTransportArrangements		= index.sibling(index.row(), PatientTransportArrangementsColumn22).data().toString();
		m_WorklistMemberData.AccessionNumber					= index.sibling(index.row(), AccessionNumberColumn3).data().toString();
		m_WorklistMemberData.RequestingPhysician				= index.sibling(index.row(), RequestingPhysicianColumn23).data().toString();
		m_WorklistMemberData.ReferringPhysicianName				= index.sibling(index.row(), ReferingPhysicianNameColumn24).data().toString();
		m_WorklistMemberData.AdmissionID						= index.sibling(index.row(), AdmissionIDColumn25).data().toString();
		m_WorklistMemberData.CurrentPatientLocation				= index.sibling(index.row(), CurrentPatientLocationColumn26).data().toString();
		m_WorklistMemberData.PatientName						= index.sibling(index.row(), PatientNameColumn0).data().toString();
		m_WorklistMemberData.PatientID							= index.sibling(index.row(), PatientIDColumn2).data().toString();
		m_WorklistMemberData.PatientBirthDate					= index.sibling(index.row(), PatientBirthDateColumn1).data().toString();
		m_WorklistMemberData.PatientSex							= index.sibling(index.row(), PatientSexColumn4).data().toString();
		m_WorklistMemberData.PatientWeight						= index.sibling(index.row(), PatientWeightColumn27).data().toString();
		m_WorklistMemberData.MedicalAlerts						= index.sibling(index.row(), MedicalAlertsColumn28).data().toString();
		m_WorklistMemberData.ContrastAllergies					= index.sibling(index.row(), ContrastAllergiesColumn29).data().toString();
	}
}


//---------------------------------------------------------------------------------
void WorklistSCPView::on_PB_CreateDcmWl_clicked()
{
    DcmDataset dataset;
     
//    DcmModulePatient patient;
//     
//    patient.setPatientId("BJ750315");
//    patient.setPatientName("Brown^John");
//     
//    dataset.attachModule(patient);
	dataset.setTagValue("FileMetaInformationGroupLength"	, "202");
	dataset.setTagValue("FileMetaInformationVersion"		, "00\01");
	dataset.setTagValue("MediaStorageSOPClassUID"			, "1.2.276.0.7230010.3.1.0.1");
	dataset.setTagValue("MediaStorageSOPInstanceUID"		, "1.2.276.0.7230010.3.1.4.2831176407.11154.1448031138.805061");
	dataset.setTagValue("ImplementationClassUID"			, "1.2.276.0.7230010.3.0.3.6.0");
	dataset.setTagValue("ImplementationVersionName"			, "OFFIS_DCMTK_360");



//	dataset.setTagValue("SpecificCharacterSet"				, Tools::removeStringBetween(m_WorklistMemberData.SpecificCharacterSet, "(", ")"));
	DcmTagCS *SpecificCharacterSet = new DcmTagCS(DcmTagKey			(0x0008,0x0005));
	SpecificCharacterSet->setString(Tools::removeStringBetween(m_WorklistMemberData.SpecificCharacterSet, "(", ")"));
	dataset.insertAndRetain(SpecificCharacterSet);

//	dataset.setTagValue("AccessionNumber"					, m_WorklistMemberData.AccessionNumber);
	DcmTagSH *AccessionNumber = new DcmTagSH(DcmTagKey				(0x0008,0x0050));
	AccessionNumber->setString(m_WorklistMemberData.AccessionNumber);
	dataset.insertAndRetain(AccessionNumber);

//	dataset.setTagValue("ReferringPhysicianName"			, m_WorklistMemberData.ReferringPhysicianName);
	DcmTagPN *ReferringPhysicianName = new DcmTagPN(DcmTagKey		(0x0008,0x0090));
	ReferringPhysicianName->setString(m_WorklistMemberData.ReferringPhysicianName);
	dataset.insertAndRetain(ReferringPhysicianName);

//	dataset.setTagValue("PatientName"						, m_WorklistMemberData.PatientName);
	DcmTagPN *PatientName = new DcmTagPN(DcmTagKey					(0x0010,0x0010));
	PatientName->setString(m_WorklistMemberData.PatientName);
	dataset.insertAndRetain(PatientName);

//	dataset.setTagValue("PatientId"							, m_WorklistMemberData.PatientID);
	DcmTagLO *PatientId = new DcmTagLO(DcmTagKey					(0x0010,0x0020));
	PatientId->setString(m_WorklistMemberData.PatientID);
	dataset.insertAndRetain(PatientId);

//	dataset.setTagValue("PatientBirthDate"					, QDate::fromString(m_WorklistMemberData.PatientBirthDate, "dd/MM/yyyy"));
	DcmTagDA *PatientBirthDate = new DcmTagDA(DcmTagKey				(0x0010,0x0030));
	PatientBirthDate->setValue(QDate::fromString(m_WorklistMemberData.PatientBirthDate, "dd/MM/yyyy"));
	dataset.insertAndRetain(PatientBirthDate);

//	dataset.setTagValue("PatientSex"						, Tools::removeStringBetween(m_WorklistMemberData.PatientSex, "(", ")"));
	DcmTagCS *PatientSex = new DcmTagCS(DcmTagKey					(0x0010,0x0040));
	PatientSex->setString(Tools::removeStringBetween(m_WorklistMemberData.PatientSex, "(", ")"));
	dataset.insertAndRetain(PatientSex);

//	dataset.setTagValue("PatientSize"						, m_WorklistMemberData.PatientSize);
//	dataset.setTagValue("PatientWeight"						, m_WorklistMemberData.PatientWeight);
	DcmTagDS *PatientWeight = new DcmTagDS(DcmTagKey				(0x0010,0x1030));
	PatientWeight->setString(m_WorklistMemberData.PatientWeight);
	dataset.insertAndRetain(PatientWeight);

//	dataset.setTagValue("MedicalAlerts"						, m_WorklistMemberData.MedicalAlerts);
	DcmTagLO *MedicalAlerts = new DcmTagLO(DcmTagKey				(0x0010,0x2000));
	MedicalAlerts->setString(m_WorklistMemberData.MedicalAlerts);
	dataset.insertAndRetain(MedicalAlerts);

//	dataset.setTagValue("Allergies"							, m_WorklistMemberData.ContrastAllergies);
	DcmTagLO *Allergies = new DcmTagLO(DcmTagKey					(0x0010,0x2110));
	Allergies->setString(m_WorklistMemberData.ContrastAllergies);
	dataset.insertAndRetain(Allergies);

//	dataset.setTagValue("StudyInstanceUID"					, m_WorklistMemberData.StudyInstanceUID);
	DcmTagUI *StudyInstanceUID = new DcmTagUI(DcmTagKey				(0x0020,0x000d));
	StudyInstanceUID->setString(m_WorklistMemberData.StudyInstanceUID);
	dataset.insertAndRetain(StudyInstanceUID);

//	dataset.setTagValue("RequestingPhysician"				, m_WorklistMemberData.RequestingPhysician);
	DcmTagPN *RequestingPhysician = new DcmTagPN(DcmTagKey			(0x0032,0x1032));
	RequestingPhysician->setString(m_WorklistMemberData.RequestingPhysician);
	dataset.insertAndRetain(RequestingPhysician);

//	dataset.setTagValue("RequestedProcedureDescription"		, m_WorklistMemberData.RequestedProcedureDescription);
	DcmTagLO *RequestedProcedureDescription = new DcmTagLO(DcmTagKey(0x0032,0x1060));
	RequestedProcedureDescription->setString(m_WorklistMemberData.RequestedProcedureDescription);
	dataset.insertAndRetain(RequestedProcedureDescription);

	DcmTagSQ *ScheduledProcedureStepSequence = new DcmTagSQ(DcmTagKey(0x0040, 0x0100));
	DcmTagItem item;
	DcmTagCS modality(DcmTagKey								(0x0008,0x0060));
	modality.setString(Tools::removeStringBetween(m_WorklistMemberData.Modality, "(", ")"));
	item.insert(modality);

	DcmTagLO RequestedContrastAgent(DcmTagKey				(0x0032,0x1070));
	RequestedContrastAgent.setString(m_WorklistMemberData.RequestedContrastAgent);
	item.insert(RequestedContrastAgent);

	DcmTagAE ScheduledStationAETitle(DcmTagKey				(0x0040,0x0001));
	ScheduledStationAETitle.setString(m_WorklistMemberData.ScheduledStationAETitle);
	item.insert(ScheduledStationAETitle);

	DcmTagDA ScheduledProcedureStepStartDate(DcmTagKey		(0x0040,0x0002));
	ScheduledProcedureStepStartDate.appendValue(QDate::fromString(m_WorklistMemberData.ScheduledProcedureStepStartDate, "dd/MM/yyyy"));
	item.insert(ScheduledProcedureStepStartDate);

	DcmTagTM ScheduledProcedureStepStartTime(DcmTagKey		(0x0040,0x0003));
	ScheduledProcedureStepStartTime.appendValue(QTime::fromString(m_WorklistMemberData.ScheduledProcedureStepStartTime, "HH:mm:ss"));
	item.insert(ScheduledProcedureStepStartTime);

	DcmTagPN ScheduledPerformingPhysicianName(DcmTagKey		(0x0040,0x0006));
	ScheduledPerformingPhysicianName.setString(m_WorklistMemberData.ScheduledPerformingPhysicianName);
	item.insert(ScheduledPerformingPhysicianName);

	DcmTagLO ScheduledProcedureStepDescription(DcmTagKey	(0x0040,0x0007));
	ScheduledProcedureStepDescription.setString(m_WorklistMemberData.ScheduledProcedureStepDescription);
	item.insert(ScheduledProcedureStepDescription);

	DcmTagSH ScheduledProcedureStepID(DcmTagKey				(0x0040,0x0009));
	ScheduledProcedureStepID.setString(m_WorklistMemberData.ScheduledProcedureStepID);
	item.insert(ScheduledProcedureStepID);

	DcmTagSH ScheduledStationName(DcmTagKey					(0x0040,0x0010));
	ScheduledStationName.setString(m_WorklistMemberData.ScheduledStationName);
	item.insert(ScheduledStationName);

	DcmTagSH ScheduledProcedureStepLocation(DcmTagKey		(0x0040,0x0011));
	ScheduledProcedureStepLocation.setString(m_WorklistMemberData.ScheduledProcedureStepLocation);
	item.insert(ScheduledProcedureStepLocation);

	DcmTagLO PreMedication(DcmTagKey						(0x0040,0x0012));
	PreMedication.setString(m_WorklistMemberData.PreMedication);
	item.insert(PreMedication);

	DcmTagLT CommentsOnTheScheduledProcedureStep(DcmTagKey	(0x0040,0x0400));
	CommentsOnTheScheduledProcedureStep.setString("No comment");
	item.insert(CommentsOnTheScheduledProcedureStep);

	ScheduledProcedureStepSequence->insert(item);
	dataset.insertAndRetain(ScheduledProcedureStepSequence);

	DcmTagSH *RequestedProcedureID = new DcmTagSH(DcmTagKey			(0x0040,0x1001));
	RequestedProcedureID->setString(m_WorklistMemberData.RequestedProcedureID);
	dataset.insertAndRetain(RequestedProcedureID);

	DcmTagSH *RequestedProcedurePriority = new DcmTagSH(DcmTagKey	(0x0040,0x1003));
	RequestedProcedurePriority->setString(m_WorklistMemberData.RequestedProcedurePriority);
	dataset.insertAndRetain(RequestedProcedurePriority);

	DcmTagLO *PatientTransportArrangements = new DcmTagLO(DcmTagKey	(0x0040,0x1004));
	PatientTransportArrangements->setString(m_WorklistMemberData.PatientTransportArrangements);
	dataset.insertAndRetain(PatientTransportArrangements);

	QString qsFileWL = settings->getOption("activeWLMSCPdirectory") + QString("/") + settings->getOption("calledAETitle")+ QString("/");
	qsFileWL +=  m_WorklistMemberData.PatientName.remove("^") + QString(".wl");
	DcmFile file(qsFileWL);
	file.write(dataset, DcmTransferSyntax::ExplicitLittleEndian);
	if( file.isError() == true )
	{
		qDebug() << "wl error : " << file.errorText();
	}

	ui.TBL_WorklistSCP->clearSelection();
	ui.PB_Delete->setEnabled(false);
	ui.PB_Modify->setEnabled(false);
	ui.PB_CreateDcmWl->setEnabled(false);
}
