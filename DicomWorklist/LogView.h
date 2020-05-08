#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>
#include "ui_LogView.h"
#include "..\..\DicomCommon\Util\Logger.h"
#include "Util\SettingsManager.h"

class LogView : public QWidget
{
	Q_OBJECT

public:
	LogView(QWidget *parent = 0);
	~LogView();

public slots:

//	void updateClientLog();
	void updateServerLog();

private:
	Ui::LogView ui;
	Logger* log;
	SettingsManager* settings;
//	qint64 m_ulClientLogPos;
//	QByteArray m_qbaClientLogBuffer;
	qint64 m_ulServerLogPos;
	QByteArray m_qbaServerLogBuffer;

private slots:
//	void on_PB_ClientRefreshButton_clicked();
//	void on_PB_ClientClearButton_clicked();
	void on_PB_ServerClearButton_clicked();
	void on_PB_ServerRefreshButton_clicked();


};

#endif // LOGVIEW_H
