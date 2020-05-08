#ifndef WORKLISTENRDLG_H
#define WORKLISTENRDLG_H

#include <QDialog>
#include "ui_WorklistEnrDlg.h"
#include "..\..\DicomCommon\CommonDefines.h"
#include <QSqlRecord.h>
#include "Util/SettingsManager.h"

class WorklistEnrDlg : public QDialog
{
	Q_OBJECT

public:
	WorklistEnrDlg(const structWorklistMemberTable data, QWidget *parent = 0);
	~WorklistEnrDlg();
	const structWorklistMemberTable getWorklistMemberTableData(void){return m_WorklistMemberData;};
	QSqlRecord getWorklistMemberSqlRecord(void){return m_sqlRecord;};
	void setWorklistMemberSqlRecord(QSqlRecord sqlR){m_sqlRecord = sqlR;};

	bool eventFilter(QObject *object, QEvent *e);

private slots:
	void on_PB_Ok_clicked();
	void on_PB_Cancel_clicked();

private:
	Ui::WorklistEnrDlg ui;
	SettingsManager* settings;
	structWorklistMemberTable m_WorklistMemberData;
	QSqlRecord m_sqlRecord;
	bool verifyInputs(void);
	void initializeScheduledStationAETitle(void);
	void on_CBX_ScheduledStationAETitle_focusOut();

	void initializeScheduledPerformingPhysicianName(void);
	void on_CBX_ScheduledPerformingPhysicianName_focusOut();
	void initializeRequestingPhysician(void);
	void on_CBX_RequestingPhysician_focusOut();
	void initializeReferringPhysicianName(void);
	void on_CBX_ReferringPhysicianName_focusOut();
	void initializePreMedication(void);
	void on_CBX_PreMedication_focusOut();
	void initializeMedicalAlerts(void);
	void on_CBX_MedicalAlerts_focusOut();
	void initializeContrastAllergies(void);
	void on_CBX_ContrastAllergies_focusOut();
	void initializeScheduledStationName(void);
	void on_CBX_ScheduledStationName_focusOut();
};

#endif // WORKLISTENRDLG_H
