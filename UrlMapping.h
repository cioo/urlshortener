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
	
	//void SetLongUrl( const std::string& url ){ this->m_longUrl = url; }
	void SetLongUrl( std::string url ){ this->m_longUrl = url; }
	
	std::string GetLongUrl() const { return m_longUrl; }
	
	//void SetShortUrl( const std::string& url ){ this->m_shortUrl = url; }
	void SetShortUrl( std::string url ){ this->m_shortUrl = url; }
	
	std::string GetShortUrl() const { return m_shortUrl; }
	
	void SetId(int id){ this->id = id; }
	
	int GetId() const { return id; }
	
private:
	std::string m_longUrl;
	std::string m_shortUrl;
	int id;
};

namespace std
{
    template<> struct hash<UrlMapping>
    {
        typedef UrlMapping argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& u) const
        {
            result_type const h1 ( std::hash<std::string>()(u.GetShortUrl()) );
            result_type const h2 ( std::hash<std::string>()(u.GetLongUrl()) );
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };
}