/*
Version 1.0.0.1
Add QDCM library for writing Dcm Dataset .wl for each database enr
Add called AETitle in SettingsManager options
* 20200508
	- Add port hostname calledAETitle in header
*/
#include "DicomWorklist.h"
#include <QtGui/QApplication>

#include <ActiveQt/QAxBindable>	
#include <ActiveQt/QAxFactory>	

#include "..\..\DicomCommon\EDAPdicom.h"
#include "Defines.h"
#include <QtCore\QFile>
#include <QtCore\QDir>
#include <QtGui\QApplication>
#include <QtGui\QMessageBox>

#include <windows.h> // for debug console window

QAXFACTORY_DEFAULT(DicomWorklist,
	   "{2AB242BC-81E8-4BAE-9056-ED38079434F7}",	//Class ID
	   "{7C6C0415-9D3C-4508-BEF0-1000A40EF92E}",	// interface ID
	   "{DAD5F848-DE66-45E9-8F82-00D3CF6E68CF}",	// event interface ID
	   "{67BD3004-E45E-405F-BF0A-8294FDC7B11C}",	// type library ID
	   "{6779141D-6538-4C1E-BDE1-BBCAB54275B3}")	// application ID
//BMA_ADD Fin



EDAPdicom::EDAPdicom() {


}

EDAPdicom::~EDAPdicom()
{
}



bool EDAPdicom::checkExternalDependencies( QWidget* mainWnd )
{

	QStringList dependencies = QStringList()			
        << EDAP_DCMTK_DCMQRIDX_EXE
		<< EDAP_DCMTK_ECHOSCU_EXE
		<< EDAP_DCMTK_FINDSCU_EXE
		<< EDAP_DCMTK_MOVESCU_EXE
		<< EDAP_DCMTK_DCMQRSCP_EXE
		<< EDAP_DCMTK_DCMDUMP_EXE
		<< EDAP_DCMTK_DCMJ2PNM_EXE
		<< EDAP_DCMTK_IMG2DCM_EXE
        << EDAP_DCMTK_DUMP2DCM_EXE
        << EDAP_DCMTK_DCMODIFY_EXE
		;
	for ( int i = 0; i < dependencies.count(); i++ )
	{
		if ( !QFile::exists( dependencies[i] ) )
		{
			QMessageBox::critical(
				mainWnd,
				"EDAPdicom",
				"Unable to locate application dependency:\n" + dependencies[i],
				QMessageBox::Ok );	
			return false;
		}
	}
	QDir qtDir(DICOMdirectory);
	if( qtDir.exists() == false )
	{
		if( qtDir.mkdir(DICOMdirectory) == false )
		{
			QMessageBox::critical(
				mainWnd,
				"EDAPdicom",
				"Unable to create directory:\n" + QString(DICOMdirectory),
				QMessageBox::Ok );	
			return false;
		}
	}
	return true;

}





int main(int argc, char *argv[])
{
	if ( !QAxFactory::isServer() )
	{
		#if defined( WIN32 ) && defined( _DEBUG )
			::AllocConsole();
			FILE* f;
			freopen_s( &f, "conin$", "r", stdin );
			freopen_s( &f, "conout$", "w", stdout );
			freopen_s( &f, "conout$", "w", stderr );
		#endif
	}
	QApplication a(argc, argv);
	if ( !QAxFactory::isServer() )
	{
		EDAPdicom edap;
		DicomWorklist w;
		w.setObjectName("From DicomWorklist");
		int res = QAxFactory::registerActiveObject(&w);
		w.show();
		//return a.exec();
		if( edap.checkExternalDependencies( &w ) )
		{
			// dependencies ok, proceed the program execution
			return a.exec();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return a.exec();
	}    

}
