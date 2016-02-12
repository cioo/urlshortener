#include <string>
#include <iostream>
#include <mysql++/mysql++.h>
#include <sys/time.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;

#include "DBParams.h"

struct DBParams dBParams;

void connectToDB()
{
	if( !dBParams.conn.connect(dBParams.c_DBName.c_str(), dBParams.c_DBHost.c_str(), "", "" ) )
	{
		cout << "db connection failed" << endl;
		exit(1);
	}
}

int main()
{
	try
	{
		cgicc::Cgicc cgi;

		cout << cgicc::HTTPHTMLHeader() << endl;

		connectToDB();

		string queryStr = "select " + dBParams.LongURLColumnName +
				" from " + dBParams.c_tableName + " where " +
				dBParams.ShortURLColumnName + "='http://bsd' ";
		cout << queryStr << endl;

		mysqlpp::Query query = dBParams.conn.query(queryStr.c_str());

		if (mysqlpp::StoreQueryResult res = query.store()) {
			cout << "We have:" << endl;
			mysqlpp::StoreQueryResult::const_iterator it;
			for (it = res.begin(); it != res.end(); ++it) {
				mysqlpp::Row row = *it;
				cout << '\t' << row[0] << endl;
			}
		}

		cout << query.affected_rows() << endl;
	}
	catch(exception& e)
	{
		cerr << "Exception thrown!\n";
		exit(1);
	}

	exit(0);
}


