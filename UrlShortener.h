#include <unordered_map>
#include "UrlMapping.h"
#include <vector>

class IDBManager;

class URLNotFoundException : std::exception
{
	
public:
	URLNotFoundException() {}
	URLNotFoundException(std::string& url) : m_url(url) { };
	
private:
	std::string m_url;
};

class UrlShortener
{
public:
	UrlShortener();
	
	UrlMapping AddNewUrlMapping( std::string& longUrl );
	
	UrlMapping GetLongUrlMapping( std::string& shortUrl );
	
	UrlMapping GetShortUrlMapping( std::string& longUrl );
	
	std::vector<UrlMapping> GetAllMappings();
	
private:
	std::string GenerateRandomString();
	
	IDBManager* m_pDBManager;
	//std::unordered_map<UrlMapping> m_cachedUrlMappings;
};