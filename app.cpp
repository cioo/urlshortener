#include <string>
#include <iostream>
#include <sys/time.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPRedirectHeader.h>

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
	UrlShortenerApp(std::string args) 
	{ 
		this->args = args;
	}
	
	bool AddNewUrl( std::string queryString )
	{
		std::string longUrl;
		if( !FindUrl(queryString, longUrl) )
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
	
	bool GotoLongUrl( std::string queryString )
	{
		std::string shortUrl;
		if( !FindUrl(queryString, shortUrl) )
		{
			return false;
		}
		
		try
		{
			std::string longUrl = m_shortener.GetLongUrlMapping(shortUrl).GetLongUrl();
			std::cout << cgicc::HTTPRedirectHeader(longUrl) << std::endl;
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
	
	void PrintSingleMapping( std::string queryString )
	{
		std::string longUrl;
		if( !FindUrl(queryString, longUrl) )
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
	
	void PrintMappings( std::string /*queryString*/, std::string premessage = "" )
	{
		std::cout << cgicc::HTTPHTMLHeader() << std::endl;
		
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
					cgicc::td() << cgicc::a(m.GetShortUrl()).set("href", "/cgi-bin/urlshortener?goto=" + m.GetShortUrl()) << cgicc::td() <<
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
		
		std::string queryString;
		std::size_t foundString;
		
		if( args.length() > 0 )
		{
			queryString = args;
		}
		else
		{
			queryString = env.getQueryString();
		}
		
		if( (foundString = queryString.find( queryStrings.c_AddAndListString )) != std::string::npos )
		{
			if( AddNewUrl( queryString ) )
				PrintMappings( queryString, "URL added succesfully");
			else
				PrintMappings( queryString, "Invalid parameters given to the service!");
		}
		else if( (foundString = queryString.find( queryStrings.c_AddString )) != std::string::npos )
		{
			if( AddNewUrl( queryString ) )
				PrintSingleMapping( queryString );
			else
				std::cout << "Did not work1" << std::endl;
		}
		else if( queryString == "" || (foundString = queryString.find( queryStrings.c_ListString )) != std::string::npos )
		{
			PrintMappings( queryString );
		}
		else if( (foundString = queryString.find( queryStrings.c_gotoString )) != std::string::npos )
		{
			if( !GotoLongUrl( queryString ) )
			{
				std::cout << "Did not work2" << std::endl;
			}
		}
		else{
			PrintMappings( queryString, "Invalid Action specified!");
		}
	}
	
private:	

	bool FindUrl( std::string queryString, std::string& url )
	{
		std::size_t foundEq = queryString.find("=");
		if( foundEq == std::string::npos )
		{
			return false;
		}
		
		std::size_t foundAmp = queryString.find("&");
		if( foundAmp == std::string::npos )
		{
			url = queryString.substr(foundEq + 1, queryString.size() );
		}
		else
		{
			url = queryString.substr(foundEq + 1, foundAmp );
		}
		
		return true;
	}
	
	UrlShortener m_shortener;
	QueryStrings queryStrings;
	cgicc::Cgicc cgi;
	std::string args;
};

int main(int argc, char** args)
{
	try{
		if( argc > 1 )
		{
			UrlShortenerApp app(args[1]);
		
			app.perform();
			
			return 0;
		}
		
		UrlShortenerApp app("");
		
		app.perform();
	}
	catch(std::exception& e)
	{
		exit(1);
	}
	
	return 0;
}