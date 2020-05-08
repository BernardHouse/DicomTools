#ifndef DICOMWORKLIST_H
#define DICOMWORKLIST_H

#include <QtGui/QWidget>
#include "ui_DicomWorklist.h"
#include "ConfigureView.h"
#include "WorklistSCPView.h"
#include "LogView.h"
#include "AboutView.h"
#include "Util\SettingsManager.h"
#include "..\..\DicomCommon\Model\LocalWLServerModel.h"

class DicomWorklist : public QWidget
{
	Q_OBJECT

public:
	DicomWorklist(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DicomWorklist();

private slots:
	 void changeView( QListWidgetItem* current, QListWidgetItem* previous );

private:
	Ui::DicomWorklistClass ui;
//	LocalServerModel *m_pLocalServerModel;

	SettingsManager *settings;
	LocalWLServerModel *m_pLocalWLServerModel;

	ConfigureView *m_pConfigureView;
	WorklistSCPView *m_pWorklistSCPView;
	LogView *m_pLogView;
	AboutView *m_pAboutView;
	int m_iConfigureViewIndex;
	int m_iWorklistSCPViewIndex;
	int m_iLogViewIndex;
	int m_iAboutViewIndex;
};

#endif // DICOMWORKLIST_H
