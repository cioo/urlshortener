#include <string>

class UrlMapping
{
public:
	UrlMapping() { }
	
	UrlMapping(std::string& longUrl, std::string& shortUrl) : m_longUrl(longUrl), m_shortUrl(shortUrl), id(-1) 	{ }
	
	UrlMapping( const UrlMapping& other )
	{
		m_longUrl = other.m_longUrl;
		m_shortUrl = other.m_shortUrl;
	}
	
	void SetLongUrl( std::string url ){ this->m_longUrl = url; }
	
	std::string GetLongUrl() const { return m_longUrl; }
	
	void SetShortUrl( std::string url ){ this->m_shortUrl = url; }
	
	std::string GetShortUrl() const { return m_shortUrl; }
	
	void SetId(int id){ this->id = id; }
	
	int GetId() const { return id; }
	
private:
	std::string m_longUrl;
	std::string m_shortUrl;
	int id;
};