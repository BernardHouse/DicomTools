#ifndef WORKLISTSCPVIEW_H
#define WORKLISTSCPVIEW_H

#include <QWidget>
#include "ui_WorklistSCPView.h"
#include <QtGui\QSortFilterProxyModel>
//#include "..\..\DicomCommon\Model\BasicWorklistQueryModel.h"
#include <QSqlQueryModel.h>
#include "Sqlite\AppSqliteDatabase.h"
#include "Util/SettingsManager.h"
#include "QWLSqlTableModel.h"


class WorklistSCPView : public QWidget
{
	Q_OBJECT

public:
	WorklistSCPView(QWidget *parent = 0);
	~WorklistSCPView();
private slots:
	void on_PB_Add_clicked();
	void on_PB_Modify_clicked();
	void on_PB_Delete_clicked();
	void on_TBL_WorklistSCP_activated( const QModelIndex& index );
	void on_TBL_WorklistSCP_clicked(const QModelIndex& index);
	void TBL_WorklistSCP_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void on_PB_CreateDcmWl_clicked();

private:
	Ui::WorklistSCPView ui;
//	QSqlTableModel	*m_pWorklistSCPModel;
	QWLSqlTableModel *m_pWorklistSCPModel;
	SettingsManager* settings;
	structWorklistMemberTable m_WorklistMemberData;
	QModelIndex m_currentSelectedIndex;
	void cleanWorklistMemberData(void);
//	QVariant data(const QModelIndex &index, int role) const;

};

#endif // WORKLISTSCPVIEW_H
