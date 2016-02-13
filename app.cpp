#include <string>
#include <iostream>
#include <sys/time.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTTPStatusHeader.h>

#include "UrlShortener.h"

// This encapsulates the different user scenarios the service is suppose to handle.
class UrlShortenerApp
{
private:
	struct QueryStrings
	{
		std::string c_AddString = "add";
		std::string c_ListString = "list";
		std::string c_AddAndListString = "addlist";
		std::string c_gotoString = "goto";
		std::string c_dnsName = "ec2-52-53-217-102.us-west-1.compute.amazonaws.com";
		std::string c_pathString = "/c";
		std::string c_appName = "urlshortener";
		std::string c_shortUrlPrefix = "go";
	};
	
public:
	UrlShortenerApp(std::string args) 
	{ 
		this->args = args;
		const cgicc::CgiEnvironment& env = cgi.getEnvironment();
	}
	
	bool AddNewUrl( std::string queryString, bool fPrint = false )
	{
		std::string longUrl;
		if( !FindUrl(queryString, longUrl) )
		{
			return false;
		}
		
		try
		{
			UrlMapping mapping;
			if( longUrl.find_first_of("://") != std::string::npos )
			{
				mapping = m_shortener.AddNewUrlMapping( longUrl );
			}
			else
			{
				std::string qualifiedStr = "http://" + longUrl;
				mapping = m_shortener.AddNewUrlMapping( qualifiedStr );
			}
			
			if( fPrint )
				PrintSingleMapping( mapping.GetShortUrl() );
			
			return true;
		}
		catch( ... )
		{
			return false;
		}
		
	}
	
	bool GotoLongUrlFromQueryString( std::string queryString )
	{
		std::string shortUrl;
		if( !FindUrl(queryString, shortUrl) )
		{
			return false;
		}
		
		try
		{
			std::string longUrl = m_shortener.GetLongUrlMapping(shortUrl).GetLongUrl();
			std::cout << cgicc::HTTPRedirectHeader(longUrl);
			
			return true;
		}
		catch( URLNotFoundException& e )
		{
			std::cout << cgicc::HTTPStatusHeader(404, "URL was not found!") << std::endl;
			std::cout << "URL was not found!" << std::endl;
			return false;
		}
		catch( ... )
		{
			return false;
		}
	}
	
        bool GotoLongUrlFromShortUrl( std::string& shortUrl )
        {
                try
                {
                        std::string longUrl = m_shortener.GetLongUrlMapping(shortUrl).GetLongUrl();
                        std::cout << cgicc::HTTPRedirectHeader(longUrl);

                        return true;
                }
                catch( URLNotFoundException& e )
                {
                        std::cout << cgicc::HTTPStatusHeader(404, "URL was not found!") << std::endl;
                        std::cout << "URL was not found!" << std::endl;
                        return false;
                }
                catch( ... )
                {
                        return false;
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
					cgicc::td() << cgicc::a(m.GetShortUrl()).set("href", "/" + queryStrings.c_shortUrlPrefix + "/" + m.GetShortUrl()) << cgicc::td();
					std::cout << cgicc::td() << cgicc::a(m.GetLongUrl()).set("href",m.GetLongUrl()) << cgicc::td();
					std::cout << cgicc::tr() << std::endl;
			}
			std::cout << cgicc::table() << std::endl;
		}
		else
		{
			std::cout << cgicc::h2("No URLs present!") << std::endl;
		}
		std::cout << cgicc::h3("Enter new Url to shorten:") << std::endl; 
		
		std::cout<< "<form action=\"" + queryStrings.c_pathString + "/" + queryStrings.c_appName + "\" method=\"get\" >" << 
			"<textarea name=\"addlist\" cols=\"100\" rows=\"1\" ></textarea><br/>" <<
			"<input type=\"submit\" value=\"Shorten!\" /> </form>" <<	std::endl;
			
		std::cout << cgicc::body() << std::endl;
		std::cout << cgicc::html() << std::endl;
	}
	
	void Perform()
	{
		const cgicc::CgiEnvironment& env = cgi.getEnvironment();
		
		std::string queryString;
		
		if( args.length() > 0 )
		{
			queryString = args;
		}
		else
		{
			queryString = env.getQueryString();
		}

		if( queryString.find( queryStrings.c_AddAndListString ) != std::string::npos )
		{
			if( AddNewUrl( queryString ) )
				PrintMappings( queryString, "URL added succesfully");
			else
				PrintMappings( queryString, "Invalid parameters given to the service!");
		}
		else if( queryString.find( queryStrings.c_AddString ) != std::string::npos )
		{
			if( !AddNewUrl( queryString, true ) )
			{
				std::cout << cgicc::HTTPHTMLHeader() << std::endl;
				std::cout << "Couldn't add new url!" << std::endl;
			}
		}
		else if( queryString == "" || queryString.find( queryStrings.c_ListString ) != std::string::npos )
		{
			PrintMappings( queryString );
		}
		else if( queryString.find( queryStrings.c_gotoString ) != std::string::npos )
		{
			GotoLongUrlFromQueryString( queryString );
		}
		else{
			PrintMappings( queryString, "Invalid Action specified!");
		}
	}
	
private:	
	void PrintSingleMapping( std::string shortUrl )
	{
		std::cout << cgicc::HTTPHTMLHeader() << std::endl;
		std::cout << "http://" << queryStrings.c_dnsName << "/" << queryStrings.c_shortUrlPrefix << "/" <<
			shortUrl << std::endl;
	}
	
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
		
		url = cgicc::form_urldecode(url);
		
		return !url.empty();
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
			// This is used for local debugging purposes.
			UrlShortenerApp app(args[1]);
		
			app.Perform();
			
			return 0;
		}
		
		UrlShortenerApp app("");
		
		app.Perform();
	}
	catch(std::exception& e)
	{
		exit(1);
	}
	
	return 0;
}
