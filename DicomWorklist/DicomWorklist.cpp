#include "DicomWorklist.h"
#include <QDesktopWidget.h>

DicomWorklist::DicomWorklist(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
	, m_pConfigureView(NULL)
	, m_pWorklistSCPView(NULL)
	, m_pLogView(NULL)
	, m_pAboutView(NULL)
	, m_iConfigureViewIndex(0)
	, m_iWorklistSCPViewIndex(1)
	, m_iLogViewIndex(2)
	, m_iAboutViewIndex(3)
	, m_pLocalWLServerModel(NULL)
{
	ui.setupUi(this);
	QDesktopWidget qdpW;
	QRect qrPrimaryScreen = qdpW.screenGeometry(qdpW.primaryScreen());
	if( qrPrimaryScreen.width() < width() )
		resize(qrPrimaryScreen.width(), height());
	if( qrPrimaryScreen.height() < height() )
		resize(width(), qrPrimaryScreen.height());

	settings = SettingsManager::instance();
	settings->initDB();
	
	m_pLocalWLServerModel = new LocalWLServerModel();

	m_pConfigureView	= new ConfigureView(this);
	m_pWorklistSCPView	= new WorklistSCPView(this);
	m_pLogView				= new LogView(this);
	m_pAboutView			= new AboutView(this);
	// compose the view widget (QStackedWidget)
	ui.viewsStackedWidget->addWidget( m_pConfigureView );
	ui.viewsStackedWidget->addWidget( m_pWorklistSCPView );
	ui.viewsStackedWidget->addWidget( m_pLogView );
	ui.viewsStackedWidget->addWidget( m_pAboutView );
	connect( ui.contentsListWidget, SIGNAL( currentItemChanged( QListWidgetItem*, QListWidgetItem* ) ),
		     this, SLOT( changeView( QListWidgetItem*, QListWidgetItem* ) ) );
	// set the Query view active
	ui.contentsListWidget->setCurrentRow( m_iWorklistSCPViewIndex );
	ui.viewsStackedWidget->setCurrentIndex( m_iWorklistSCPViewIndex );



	// start server if selected
	if ( settings->getOption( "serverStartAutomatically" ) == "true" )
	{
		m_pLocalWLServerModel->startServer();
	}

}

DicomWorklist::~DicomWorklist()
{
	if( m_pLocalWLServerModel != NULL )
	{
		delete m_pLocalWLServerModel;
		m_pLocalWLServerModel = NULL;
	}
	if( m_pAboutView != NULL )
	{
		delete m_pAboutView;
		m_pAboutView = NULL;
	}
	if( m_pLogView != NULL )
	{
		delete m_pLogView;
		m_pLogView = NULL;
	}
	if( m_pWorklistSCPView != NULL )
	{
		delete m_pWorklistSCPView;
		m_pWorklistSCPView = NULL;
	}
	if( m_pConfigureView != NULL )
	{
		delete m_pConfigureView;
		m_pConfigureView = NULL;
	}
}

//---------------------------------------------------------------------------------
void DicomWorklist::changeView( QListWidgetItem* current, QListWidgetItem* previous )
{
	if( !current )
	{
		current = previous;
	}
	ui.viewsStackedWidget->setCurrentIndex( ui.contentsListWidget->row( current ) );
	if ( m_iLogViewIndex != -1 )
	{
		if ( ui.contentsListWidget->currentRow() == m_iLogViewIndex )
		{
			if( m_pLogView != NULL )
			{
				//m_pLogView->updateClientLog();
				m_pLogView->updateServerLog();
			}
		}
		if ( ui.contentsListWidget->currentRow() != m_iConfigureViewIndex )
		{
			if( (previous != NULL) && (m_pConfigureView != NULL) )
			{
				//if( previous->text() == "Configure" )
				//	settings->saveSettings();
			}
		}
	}
}
