#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
class weather_day;

namespace parser {

	std::vector<weather_day> parse_file(const char *filename);
	std::vector<weather_day> parse_string(const std::string &input);
}
#endif
