#include <mysql++/mysql++.h>

#include "DBManager.h"
#include "DBParams.h"
#include "UrlShortener.h"

class DBManagerImpl : public IDBManager
{
public:
	DBManagerImpl() {}
	
	virtual void Init()
	{
		if( !dBParams.conn.connect(dBParams.c_DBName.c_str(), dBParams.c_DBHost.c_str(), "", "" ) )
		{
			std::cout << "db connection failed" << std::endl;
			exit(1);
		}
	}
	
	virtual bool GetLongUrlMapping(std::string& shortUrl, UrlMapping* mapping )
	{
		std::string queryStr = "select " + dBParams.LongURLColumnName + "," + dBParams.ShortURLColumnName +
				" from " + dBParams.c_tableName + " where " +
				dBParams.ShortURLColumnName + "='" + shortUrl.c_str() + "'";
				
		//std::cout << queryStr << std::endl;

		return ListQuery(queryStr,mapping);
	}
	
	virtual bool GetShortUrlMapping(std::string& longUrl, UrlMapping* mapping )
	{
		std::string queryStr = "select " + dBParams.LongURLColumnName + "," + dBParams.ShortURLColumnName +
				" from " + dBParams.c_tableName + " where " +
				dBParams.LongURLColumnName + "='" + longUrl.c_str() + "'";
				
		//::cout << queryStr << std::endl;

		
		return ListQuery(queryStr,mapping);
	}
	
	virtual bool AddUrlMapping(UrlMapping* pUrlMapping)
	{
		std::string queryStr = "insert into `" + dBParams.c_tableName + "` (`" + 
		dBParams.LongURLColumnName + "`,`" + dBParams.ShortURLColumnName + "`) VALUES('" + 
		pUrlMapping->GetLongUrl() + "','"  + pUrlMapping->GetShortUrl() + "')";
		
		//std::cout << queryStr << std::endl;
		
		return AddQuery(queryStr);
	}
	
	virtual std::vector<UrlMapping> ListAllMappings()
	{
		std::vector<UrlMapping> results;
		
		std::string queryStr = "select " + dBParams.LongURLColumnName + "," + dBParams.ShortURLColumnName +
				" from " + dBParams.c_tableName;
		
		mysqlpp::Query query = dBParams.conn.query(queryStr.c_str());
		
		if (mysqlpp::StoreQueryResult res = query.store()) {
			mysqlpp::StoreQueryResult::const_iterator it;
			for (it = res.begin(); it != res.end(); ++it) {
				mysqlpp::Row row = *it;
				UrlMapping curMapping;
				
				curMapping.SetLongUrl(row[0].c_str());
				curMapping.SetShortUrl(row[1].c_str());
				
				results.push_back(curMapping);
			}
		}
		
		return results;
	}
	
	virtual void Close()
	{
		
	}
	
private:

	bool AddQuery( std::string& queryStr )
	{
		mysqlpp::Query query = dBParams.conn.query(queryStr.c_str());
		query.exec();
		
		return query.affected_rows() == 1;
	}
	
	bool ListQuery( std::string& queryStr, UrlMapping* mapping )
	{
		mysqlpp::Query query = dBParams.conn.query(queryStr.c_str());
		
		if (mysqlpp::StoreQueryResult res = query.store()) {
			mysqlpp::StoreQueryResult::const_iterator it;
			for (it = res.begin(); it != res.end(); ++it) {
				mysqlpp::Row row = *it;
				mapping->SetLongUrl(row[0].c_str());
				mapping->SetShortUrl(row[1].c_str());
				return true;
			}
		}
		
		return false;
	}
	
	struct DBParams dBParams;
};

IDBManager* CreateDBManager()
{
	return new DBManagerImpl();
}