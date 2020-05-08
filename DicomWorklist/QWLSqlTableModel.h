#ifndef QWLSQLTABLEMODEL_H
#define QWLSQLTABLEMODEL_H

#include <QSqlTableModel>

class QWLSqlTableModel : public QSqlTableModel
{
public:
	QWLSqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
	~QWLSqlTableModel();

	virtual QVariant data(const QModelIndex &index, int role) const;

private:
	
};

#endif // QWLSQLTABLEMODEL_H
