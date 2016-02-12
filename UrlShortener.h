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

// Url Shortener logic is defined here.
// This class is in charge of creating random shorturls for corresponding long urls 
// and persisting them to the data store.
class UrlShortener
{
public:
	UrlShortener();
	
	UrlMapping AddNewUrlMapping( std::string& longUrl );
	
	UrlMapping GetLongUrlMapping( std::string& shortUrl );
	
	UrlMapping GetShortUrlMapping( std::string& longUrl );
	
	std::vector<UrlMapping> GetAllMappings();
	
	~UrlShortener();
	
private:
	std::string GenerateRandomString();
	
	IDBManager* m_pDBManager;
};