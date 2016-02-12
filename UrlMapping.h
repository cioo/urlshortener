#include <string>

// Represents a basic mapping of ShortUrl <=> Url
class UrlMapping
{
public:
	UrlMapping() { }
	
	UrlMapping(std::string& longUrl, std::string& shortUrl) : m_longUrl(longUrl), m_shortUrl(shortUrl)	{ }
	
	UrlMapping( const UrlMapping& other )
	{
		m_longUrl = other.m_longUrl;
		m_shortUrl = other.m_shortUrl;
	}
	
	void SetLongUrl( std::string url ){ this->m_longUrl = url; }
	
	std::string GetLongUrl() const { return m_longUrl; }
	
	void SetShortUrl( std::string url ){ this->m_shortUrl = url; }
	
	std::string GetShortUrl() const { return m_shortUrl; }
	
private:
	std::string m_longUrl;
	std::string m_shortUrl;
};