#include "QWLSqlTableModel.h"
#include <qcolor.h>
#include "..\..\DicomCommon\CommonDefines.h"

QWLSqlTableModel::QWLSqlTableModel(QObject *parent, QSqlDatabase db )
	: QSqlTableModel(parent, db)
{

}

QWLSqlTableModel::~QWLSqlTableModel()
{
}

//--------------------------------------------------------------------------------- 
QVariant QWLSqlTableModel::data(const QModelIndex &index, int role) const
{
	switch(role)
	{
	case Qt::TextColorRole	:
								if( index.sibling(index.row(), PatientSexColumn4).data().toString().at(0) == 'F' )
									return QVariant(QColor(255, 0, 255, 255));
								else if( index.sibling(index.row(), PatientSexColumn4).data().toString().at(0) == 'M' )
									return QVariant(QColor(0, 0, 255, 255));
								else
									return QSqlTableModel::data(index, role);
								break;
/*	case Qt::BackgroundRole	:
								if( index.sibling(index.row(), PatientSexColumn4).data().toString().at(0) == 'F' )
									return QVariant(QColor(255, 170, 255, 255));
								else if( index.sibling(index.row(), PatientSexColumn4).data().toString().at(0) == 'M' )
									return QVariant(QColor(170, 255, 255, 255));
								else
									return QSqlTableModel::data(index, role);
								break;*/
	default:					return QSqlTableModel::data(index, role);
	}
}
