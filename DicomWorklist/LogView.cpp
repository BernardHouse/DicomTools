#include "LogView.h"
#include <QtCore\QFile>

LogView::LogView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    log = Logger::instance();	
	settings = SettingsManager::instance();

//	m_ulClientLogPos = 0;
	m_ulServerLogPos = 0;
}

LogView::~LogView()
{
	// clear temporary log files
//	QFile::remove( settings->getOption( "clientOutputLog" ) );
	QFile::remove( settings->getOption( "serverOutputLog" ) );
}


// --- CLIENT ------------------------------------------------------------------
/*
void LogView::updateClientLog()
{
	QFile file( settings->getOption( "clientOutputLog" ) );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {		
		file.seek( m_ulClientLogPos );
		m_qbaClientLogBuffer.append( file.readAll() );
		int lineCount = m_qbaClientLogBuffer.count( '\n' );
		int idx = 0;
		while ( lineCount > ui.SB_ClientLineBufferSize->value() ) {
			idx = m_qbaClientLogBuffer.indexOf( '\n', idx + 1 );
			lineCount--;
		}
		m_ulClientLogPos = file.pos();
		file.close();
		if ( idx != 0 ) {
			ui.LE_ClientLog->setText( m_qbaClientLogBuffer.mid( idx + 1 ) );
		} else {
			ui.LE_ClientLog->setText( m_qbaClientLogBuffer.mid( idx ) );
		}
		ui.LE_ClientLog->moveCursor( QTextCursor::End );
	} else {
		log->dbg( "Warning: cannot open client output log." );
	}
}
*/
//void LogView::on_PB_ClientRefreshButton_clicked()
//{
//	updateClientLog();
//}
//
//void LogView::on_PB_ClientClearButton_clicked()
//{
//	m_qbaClientLogBuffer.clear();
//	ui.LE_ClientLog->clear();
//}

// --- SERVER ------------------------------------------------------------------

void LogView::updateServerLog()
{
	QFile file( settings->getOption( "serverOutputLog" ) );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {		
		file.seek( m_ulServerLogPos );
		m_qbaServerLogBuffer.append( file.readAll() );
		int lineCount = m_qbaServerLogBuffer.count( '\n' );
		int idx = 0;
		while ( lineCount > ui.SB_ServerLineBufferSize->value() ) {
			idx = m_qbaServerLogBuffer.indexOf( '\n', idx + 1 );
			lineCount--;
		}
		m_ulServerLogPos = file.pos();
		file.close();
		if ( idx != 0 ) {
			ui.LE_ServerLog->setText( m_qbaServerLogBuffer.mid( idx + 1 ) );
		} else {
			ui.LE_ServerLog->setText( m_qbaServerLogBuffer.mid( idx ) );
		}
		ui.LE_ServerLog->moveCursor( QTextCursor::End );
	} else {
		log->dbg( "Warning: cannot open server output log." );
	}
}

void LogView::on_PB_ServerRefreshButton_clicked() {
	updateServerLog();
}

void LogView::on_PB_ServerClearButton_clicked() {
	m_qbaServerLogBuffer.clear();
	ui.LE_ServerLog->clear();
}
