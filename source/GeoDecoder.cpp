#include "GeoDecoder.h"
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;

const std::string API = "2298cf256f06437088b7f89347fc0b43";

GeoDecoder::GeoDecoder()
{
	// TODO Auto-generated constructor stub
	std::cout << "GeoCoder was started\n";
}

GeoDecoder::~GeoDecoder() {
	// TODO Auto-generated destructor stub
}

std::string GeoDecoder::getCity(double lat, double lon) {
	http_client client("https://api.geoapify.com/v1/geocode/reverse?lang=ru&lat=" + std::to_string(lat) + "&lon=" + std::to_string(lon) + "&apiKey="+API);
	std::string location = "Unknown location";
	client.request(methods::GET).then([&location](http_response response)
	{
	  if(response.status_code() == status_codes::OK)
	  {
	    response.extract_json().then([&location](json::value jsonValue)
	    {
	    	try {
	    		std::string suburb = utility::conversions::to_utf8string(jsonValue.at("features")[0].at("properties").at("suburb").serialize());
	    		location = suburb.substr(1, suburb.size() - 2);
	    		return;
	    	}
	    	catch (web::json::json_exception &e) {
	    		try {
	    			    		//if is city
	    			    		std::string city = utility::conversions::to_utf8string(jsonValue.at("features")[0].at("properties").at("city").serialize());
	    			    		//std::string county = utility::conversions::to_utf8string(jsonValue.at("features")[0].at("properties").at("county").serialize());
	    				    	location =  city.substr(1, city.size() - 2);// + ", " +
	    				    			   // county.substr(1, county.size() - 2);

	    			    		return;
	    			    	}
	    			    	catch (web::json::json_exception &e) {
	    			    		// not city
	    			    		try {
	    			    		//std::cout<< e.what();
	    			    		std::string county = utility::conversions::to_utf8string(jsonValue.at("features")[0].at("properties").at("county").serialize());
	    			    		location = county.substr(1, county.size() - 2);
	    			    		return;
	    			    		} catch (web::json::json_exception &e) {
	    			    			std::cout<< jsonValue<<endl;
	    			    			location = "Somewhere in Russia";
	    			    			return;
	    			    		}
	    			    	}
	    	}
	    }).get();
	  }
	}).get();
	return location;
}
