#include <string>
#include <mysql++/mysql++.h>

struct DBParams
{
	mysqlpp::Connection     conn;
	const std::string c_DBHost = "localhost";
	const std::string c_DBSock =  "/var/lib/mysql/mysql.sock";
	const std::string c_DBName = "URLs";
	const std::string c_tableName = "UrlMappings";
	const std::string LongURLColumnName = "LongURL";
	const std::string ShortURLColumnName = "ShortURL";

	DBParams() : conn(false) {};
};
