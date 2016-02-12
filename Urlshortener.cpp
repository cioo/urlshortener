#include <string>
#include <iostream>
#include <sys/time.h>
#include <cstdlib>
#include <ctime>

#include "UrlShortener.h"
#include "DBManager.h"

UrlShortener::UrlShortener() 
{
	m_pDBManager = CreateDBManager();
	m_pDBManager->Init();
}

static const char alphanum[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

char genRandom()
{
    return alphanum[rand() % (sizeof(alphanum) - 1)];
}

std::string UrlShortener::GenerateRandomString()
{
	std::string result;
	srand(time(0));
    for(unsigned int i = 0; i < 5; ++i)
    {
		result += genRandom();
    }
	
	return result;
}

UrlMapping UrlShortener::AddNewUrlMapping( std::string& longUrl )
{
	UrlMapping newMapping;
	newMapping.SetLongUrl( longUrl );
	
	UrlMapping existingMapping;
	if( !m_pDBManager->GetShortUrlMapping( longUrl, &existingMapping ) )
	{
		std::string shortUrl = GenerateRandomString();
		newMapping.SetShortUrl(shortUrl);
		if( !m_pDBManager->AddUrlMapping(&newMapping) )
			throw;
		
		return newMapping;
	}
	
	return existingMapping;
}

UrlMapping UrlShortener::GetLongUrlMapping( std::string& shortUrl )
{
	UrlMapping existingMapping;
	if( !m_pDBManager->GetLongUrlMapping( shortUrl, &existingMapping ) )
	{
		throw URLNotFoundException( shortUrl );
	}
	
	return existingMapping;
}

UrlMapping UrlShortener::GetShortUrlMapping( std::string& longUrl )
{
	UrlMapping existingMapping;
	if( !m_pDBManager->GetShortUrlMapping( longUrl, &existingMapping ) )
	{
		throw URLNotFoundException( longUrl );
	}
	
	return existingMapping;
}

std::vector<UrlMapping> UrlShortener::GetAllMappings()
{
	return m_pDBManager->ListAllMappings();
}