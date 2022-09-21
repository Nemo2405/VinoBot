/*
 * GeoDecoder.h
 *
 *  Created on: Aug 24, 2022
 *      Author: eddyneshton
 */

#ifndef GEODECODER_H_
#define GEODECODER_H_
#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <utility>
#include <future>
//#include <nlohmann/json.hpp>

class GeoDecoder {

public:
	GeoDecoder();
	virtual ~GeoDecoder();

	std::string getCity(double lat, double lon);

	//std::future<std::string> getCity(double lat, double lon);
private:
};

#endif /* GEODECODER_H_ */
