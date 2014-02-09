#include "grabber.hpp"

#include <sstream>
#include <unistd.h>

grabber::grabber() 
{
	curl = curl_easy_init();
}

std::string grabber::grab_to_string(const std::string &url)
{
	return grab(url, true);
}

std::string grabber::grab_to_file(const std::string &url)
{
	return grab(url, false);
}

std::string grabber::grab(const std::string &url, bool tostring) 
{
	//Filename or HTML goes here.
	std::string mi;

	FILE *my_file = 0;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	//Do we want this data as a file or string?
	if (tostring) {
		//string
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mi);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_string);
	} else {
		//file
		unsigned int offset = url.find_last_of('/') + 1;

		if (offset < url.size())
			mi = url.substr(offset);
		else
			mi = "dsklhf";

		my_file = fopen(mi.c_str(), "wb+");

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, my_file);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_file);
	}

	curl_easy_perform(curl);
	
	//It is important to close the file otherwise a chunk of the end may not be written.
	if (my_file)
		fclose(my_file);
	
	return mi;
}

size_t grabber::write_data_file(char *ptr, size_t size, 
	size_t nmemb, FILE *stream) 
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

size_t grabber::write_data_string(char *ptr, size_t size, 
	size_t nmemb, std::string *stream) 
{
	int result = 0;

	if (stream != NULL) {
	    stream->append(ptr, size * nmemb);
	    result = size * nmemb;
	}

	return result;
}
