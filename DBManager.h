#include <vector>
#include <string>

class UrlMapping;

class IDBManager
{
public:
	virtual void Init() = 0;
	virtual void AddUrlMapping(UrlMapping* pUrlMapping) = 0;
	virtual bool GetLongUrlMapping(std::string& shortUrl, UrlMapping* mapping ) = 0;
	virtual bool GetShortUrlMapping(std::string& longUrl, UrlMapping* mapping ) = 0;
	virtual std::vector<UrlMapping> ListAllMappings() = 0;
	virtual void Close() = 0;
};

IDBManager* CreateDBManager();
