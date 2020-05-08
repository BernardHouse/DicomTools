#include "DICOMFile.h"

#include "Defines.h"

#include <QtCore\QDir>
#include <QtCore\QUuid>

DICOMFile::DICOMFile( QString file ) {
	this->file = file;
	dcmdump = NULL;
	dcm2pnm = NULL;
	this->log = Logger::instance();
	this->settings = SettingsManager::instance();
}

DICOMFile::~DICOMFile() {
	if ( dcm2pnm != NULL ) {
		delete dcm2pnm;
	}
	if ( dcmdump != NULL ) {
		delete dcmdump;
	}
}

QByteArray DICOMFile::getDataDump() {

    dcmdump = new QProcess();
	
	// for reading stdout and stderr
	connect( dcmdump, SIGNAL( readyReadStandardOutput() ),
             this, SLOT( readOutput() ) );

	QStringList arguments;
	arguments << "--load-short";
	arguments << "--print-short";
	arguments << "--print-all";
	arguments << file;

	// merge stdout and stderr and redirect them
	dcmdump->setProcessChannelMode( QProcess::MergedChannels );

	log->dbg( "Starting process dcmdump (file: " + file + ")..." );
	dcmdump->start( EDAP_DCMTK_DCMDUMP_EXE, arguments );

	if ( !dcmdump->waitForStarted( 30000 ) ) {
		log->dbg( "Failed to initialize process dcmdump" );
		return QByteArray();
	}

	if ( !dcmdump->waitForFinished( 600000 ) ) {
		log->dbg( "Process dcmdump timed out" );
		return QByteArray();
	}

	// make sure we have read all output
	readOutput();
	
	if ( dcmdump->state() == QProcess::Running ) {
		log->dbg( "Killing process dcmdump..." );
		dcmdump->kill();			
		dcmdump->waitForFinished( 10000 );
	}	
	delete dcmdump;
	dcmdump = NULL;

	log->dbg( "Process dcmdump finished" );

	return outputBuffer;

}

QPixmap DICOMFile::getPixmap( bool useVOIwindow )
{

    dcm2pnm = new QProcess();	
	QString targetFile( QDir::tempPath() + "/" + QUuid::createUuid().toString() + ".bmp" );

	QStringList arguments;
	arguments << "--write-bmp";
	arguments << "--no-overlays";
	if ( useVOIwindow )
	{
		arguments << "--use-window" << "1";
	}
	arguments << file;
	arguments << targetFile;

	log->dbg( "Starting process dcmj2pnm (file: " + file + ")..." );
	dcm2pnm->start( EDAP_DCMTK_DCMJ2PNM_EXE, arguments );

	if ( !dcm2pnm->waitForStarted( 30000 ) ) {
		log->dbg( "Failed to initialize process dcmj2pnm" );
		return QByteArray();
	}

	if ( !dcm2pnm->waitForFinished( 600000 ) ) {
		log->dbg( "Process dcmj2pnm timed out" );
		return QByteArray();
	}

	if ( dcm2pnm->state() == QProcess::Running ) {
		log->dbg( "Killing process dcmj2pnm..." );
		dcm2pnm->kill();			
		dcm2pnm->waitForFinished( 10000 );
	}	
	delete dcm2pnm;
	dcm2pnm = NULL;

	log->dbg( "Process dcmj2pnm finished" );

	// read the produced bitmap into a QPixmap and return it
	QPixmap pixmap( targetFile );

	// delete the temp file
	QFile::remove( targetFile );
	
	return pixmap;

}

void DICOMFile::readOutput() {
	if ( dcmdump != NULL ) {
		outputBuffer.append( dcmdump->readAllStandardOutput() );
	}
}
