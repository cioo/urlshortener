#include <string>
#include <iostream>
#include <sys/time.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include "UrlShortener.h"

class UrlShortenerApp
{
private:
	struct QueryStrings
	{
		std::string c_AddString = "add";
		std::string c_ListString = "list";
		std::string c_AddAndListString = "addlist";
		std::string c_gotoString = "goto";
	};
	
public:
	UrlShortenerApp() 
	{ 
		std::cout << cgicc::HTTPHTMLHeader() << std::endl;
	}
	
	bool AddNewUrl( const cgicc::CgiEnvironment& env )
	{
		std::string longUrl;
		if( !FindUrl(env, longUrl) )
		{
			return false;
		}
		
		try
		{
			m_shortener.AddNewUrlMapping( longUrl );
			return true;
		}
		catch( ... )
		{
			return false;
		}
		
	}
	
	bool GotoLongUrl( const cgicc::CgiEnvironment& env )
	{
		std::string shortUrl;
		if( !FindUrl(env, shortUrl) )
		{
			return false;
		}
		
		try
		{
			std::string longUrl = m_shortener.GetShortUrlMapping(shortUrl).GetLongUrl();
			return true;
		}
		catch( URLNotFoundException& e )
		{
			std::cout << "URL was not found" << std::endl;
			return false;
		}
		catch( ... )
		{
			return false;
		}
	}
	
	void PrintSingleMapping( const cgicc::CgiEnvironment& env )
	{
		std::string longUrl;
		if( !FindUrl(env, longUrl) )
		{
			return;
		}
		
		try
		{
			std::string shortUrl = m_shortener.GetShortUrlMapping(longUrl).GetShortUrl();
		}
		catch( URLNotFoundException& e )
		{
			std::cout << "URL was not found" << std::endl;
		}
		catch( ... )
		{
			std::cout << "Exception!" << std::endl;	
		}
	}
	
	void PrintMappings( const cgicc::CgiEnvironment& /*env*/, std::string premessage = "" )
	{
		std::cout << cgicc::html() << std::endl;
		std::cout << cgicc::head(cgicc::title("URL Shortener")) << std::endl;
		
		std::cout << cgicc::body() << std::endl;
		std::cout << premessage << std::endl;
		
		std::vector<UrlMapping> allMappings = m_shortener.GetAllMappings();
		if( allMappings.size() > 0 )
		{
			std::cout << cgicc::h2("URLs:") << std::endl;
			std::cout << cgicc::table().set("border","1") << std::endl;
			std::cout << cgicc::tr() << cgicc::th("Short URLs") << cgicc::th("Long URLs") << 
					cgicc::tr() << std::endl;
			for( UrlMapping& m : allMappings )
			{
				std::cout << cgicc::tr() << 
					cgicc::td() << cgicc::a(m.GetShortUrl()).set("href", "/cgi-bin/?goto=" + m.GetShortUrl()) << cgicc::td() <<
					cgicc::td() << cgicc::a(m.GetLongUrl()).set("href",m.GetLongUrl()) << cgicc::td() <<
					cgicc::tr() << std::endl;
			}
			std::cout << cgicc::table() << std::endl;
		}
		else
		{
			std::cout << cgicc::h2("No URLs present!") << std::endl;
		}
		
		std::cout << cgicc::body() << std::endl;
		std::cout << cgicc::html() << std::endl;
	}
	
	void perform()
	{
		const cgicc::CgiEnvironment& env = cgi.getEnvironment();
		std::string queryString = env.getQueryString();
		std::size_t foundString;
		if( (foundString = queryString.find( queryStrings.c_AddAndListString )) != std::string::npos )
		{
			if( AddNewUrl( env ) )
				PrintMappings( env, "URL added succesfully");
			else
				PrintMappings( env, "Invalid parameters given to the service!");
		}
		else if( (foundString = queryString.find( queryStrings.c_AddString )) != std::string::npos )
		{
			if( AddNewUrl( env ) )
				PrintSingleMapping( env );
			else
				std::cout << "Did not work" << std::endl;
		}
		else if( queryString == "" || (foundString = queryString.find( queryStrings.c_ListString )) != std::string::npos )
		{
			PrintMappings( env );
		}
		else if( (foundString = queryString.find( queryStrings.c_gotoString )) != std::string::npos )
		{
			if( !GotoLongUrl( env ) )
			{
				std::cout << "Did not work" << std::endl;
			}
		}
		else{
			PrintMappings( env, "Invalid Action specified!");
		}
	}
	
private:	

	bool FindUrl( const cgicc::CgiEnvironment& env, std::string& url )
	{
		std::size_t foundEq = env.getQueryString().find("=");
		if( foundEq == std::string::npos )
		{
			return false;
		}
		
		std::size_t foundAmp = env.getQueryString().find("&");
		if( foundAmp == std::string::npos )
		{
			url = env.getQueryString().substr(foundEq, env.getQueryString().size() );
		}
		else
		{
			url = env.getQueryString().substr(foundEq, foundAmp );
		}
		
		return true;
	}
	
	UrlShortener m_shortener;
	QueryStrings queryStrings;
	cgicc::Cgicc cgi;
};

int main()
{
	try{
		UrlShortenerApp app;
		
		app.perform();
	}
	catch(std::exception& e)
	{
		exit(1);
	}
	
	return 0;
}