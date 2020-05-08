#pragma once

#include "..\..\DicomCommon\Util\Logger.h"
#include "Util\SettingsManager.h"

#include <QtCore\QByteArray>
#include <QtCore\QProcess>
#include <QtCore\QString>
#include <QtGui\QPixmap>

/** A DICOM file residing in a local filesystem.
 */
class DICOMFile : public QObject {

	Q_OBJECT

public:

	DICOMFile( QString file );
	~DICOMFile();

	QPixmap getPixmap( bool useVOIwindow );
	QByteArray getDataDump();

private:

	Logger* log;
	SettingsManager* settings;
	QString file;
	QByteArray outputBuffer;
	QProcess* dcm2pnm;	
	QProcess* dcmdump;	

private slots:

	void readOutput();

};
