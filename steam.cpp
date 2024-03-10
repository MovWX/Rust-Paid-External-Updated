#include "include.hpp"
#include "steam.hpp"
#ifdef _DEBUG
#pragma comment( lib, "libcurl_a_debug.lib")
#else
#pragma comment( lib, "libcurl_a.lib")
#endif

static size_t WriteData ( void* buffer, size_t size, size_t nmemb, void* param )
{
	std::string& text = *static_cast<std::string*>( param );
	size_t totalsize = size * nmemb;
	text.append ( static_cast<char*>( buffer ), totalsize );

	return totalsize;
}

CURLcode RequestData ( const char* url, std::string& data )
{
    CURLcode res = CURLE_FAILED_INIT;

    CURL* curl = curl_easy_init ( );
    if ( curl ) {
        curl_easy_setopt ( curl, CURLOPT_URL, url );
        curl_easy_setopt ( curl, CURLOPT_WRITEFUNCTION, WriteData );
        curl_easy_setopt ( curl, CURLOPT_WRITEDATA, &data );
        res = curl_easy_perform ( curl );
        curl_easy_cleanup ( curl );
    }

    return res;
}

void steam::get_username_steam64 ( ULONG64 steam64, std::string& name )
{
    std::string url = _ ( "https://ricochet.gg/api/steam.php?steamid=" ) + std::to_string ( steam64 );
    RequestData ( url.c_str ( ), name );
}