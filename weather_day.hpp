#ifndef WEATHER_DAY
#define WEATHER_DAY

#include <string>

struct weather_day {

	std::string temp, desc, url;

	weather_day(const std::string &temp_,
		const std::string &desc_,
		const std::string &url_) :
			temp(temp_),
			desc(desc_),
			url(url_) {}
	
};
#endif
