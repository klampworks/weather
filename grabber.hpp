#ifndef GRABBER_HPP
#define GRABBER_HPP

#include <string>
#include <curl/curl.h>
#include <curl/easy.h>

class grabber {

	public: 
		grabber();
		std::string grab_to_file(const std::string&);
		std::string grab_to_string(const std::string&);

	private:
		std::string grab(const std::string&, bool);
		CURL *curl;
		static size_t write_data_string(char *ptr, size_t size, 
			size_t nmemb, std::string *stream);
		static size_t write_data_file(char *ptr, size_t size, 
			size_t nmemb, FILE *stream);
};
#endif
