#include <vector>
#include <string>

class UrlMapping;

// Interface class for a communicating with a Generic DataStore
class IDBManager
{
public:
	//Perform any initialization steps here
	virtual void Init() = 0;
	
	//Add the given UrlMapping to the persistent store
	virtual bool AddUrlMapping(UrlMapping* pUrlMapping) = 0;
	
	//Query persisted store with the given short url
	virtual bool GetLongUrlMapping(std::string& shortUrl, UrlMapping* mapping ) = 0;
	
	//Query persisted store with the given long url
	virtual bool GetShortUrlMapping(std::string& longUrl, UrlMapping* mapping ) = 0;
	
	//List of all persisted urls
	virtual std::vector<UrlMapping> ListAllMappings() = 0;
	
	//Uninitilization step here, if any
	virtual void Close() = 0;
};

IDBManager* CreateDBManager();
