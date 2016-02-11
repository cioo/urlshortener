#include <string>
#include <iostream>
#include <mysql++/mysql++.h>
#include <sys/time.h>
using namespace std;

struct DBParams
{
	mysqlpp::Connection     conn;
	const string c_DBHost = "localhost";
	const string c_DBSock =  "/var/lib/mysql/mysql.sock";
	const string c_DBName = "URLs";
	const string c_tableName = "UrlMappings";
	const string LongURLColumnName = "LongURL";
	const string ShortURLColumnName = "ShortURL";

	DBParams() : conn(false)
	{

	}
};

struct DBParams dBParams;

void connectToDB()
{
	if( !dBParams.conn.connect(dBParams.c_DBName.c_str(), dBParams.c_DBHost.c_str(), "", "" ) )
	{
		cout << "bok" << endl;
		exit(1);
	}
}

int main()
{
	cout << "Content-type: text/html\n\n";

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

	exit(0);
}

