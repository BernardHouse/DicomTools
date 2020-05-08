#pragma once
#include "..\..\DicomCommon\Sqlite\SqliteDatabase.h"

class AppSqliteDatabase: public CSqliteDatabase
{
public:
	AppSqliteDatabase(void);
	virtual ~AppSqliteDatabase(void);

	//application
	virtual bool CreateTables(void);


    bool addModifyWorklistMember(const structWorklistMemberTable data);
    bool deleteWorklistMember(const structWorklistMemberTable data);
	int readNextSequence(const QString &qsTable, const bool &bWriteIt);

	QStringList readOneTableField(const QString &qsTable, const QString &qsFieldName);

private:
};
