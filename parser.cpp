#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
 
#include "weather_day.hpp"

using namespace boost;
using namespace property_tree;;

/* Debug function for recursivly printing out a node and its
 * children. */
void print(const ptree &pt)
{
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
        std::cout << it->first << ": " << 
		it->second.get_value<std::string>() << std::endl;

        print(it->second);
    }
}

/* For nodes embedded inside single element arrays.
 * I'm no JSON expert so I assume they have a good reason
 * for doing this. */
std::string get_value(const ptree::value_type &v)
{
	return std::string(v.second.begin()->second.begin()->second.data()); 
}

weather_day parse_day(const ptree &pt)
{
	weather_day ret;
	for (const auto &v: pt) {

		if (v.first.data() == std::string("tempMaxC")
			|| v.first.data() == std::string("temp_C")) {

			ret.temp = v.second.data();
		} else if (v.first.data() == std::string("weatherDesc")) {
			ret.desc =  get_value(v);
		} else if (v.first.data() == std::string("weatherIconUrl")) {
			ret.url =  get_value(v);
			break;
		}
	}

	return std::move(ret);
}

std::vector<weather_day> parse_days(std::stringstream &&ss)
{
	ptree pt;
	read_json(ss, pt);

	std::vector<weather_day> ret;

	auto a = pt.get_child("data.current_condition.");
	auto day = parse_day(a.begin()->second);
	ret.push_back(std::move(day));

	for (ptree::value_type &v: pt.get_child("data.weather.")) {
		auto day = parse_day(v.second);
		ret.push_back(std::move(day));
	}

	return std::move(ret);
}

std::vector<weather_day> parse_file(const char *filename)
{
	std::ifstream ifs;
	ifs.open(filename);
	std::string tmp;

	std::stringstream ss;
	while(std::getline(ifs, tmp)) 
		ss << tmp;

	return parse_days(std::move(ss));
}

std::vector<weather_day> parse_string(const std::string &input)
{
	std::stringstream ss;
	ss << input;
	return parse_days(std::move(ss));
}

int main()
{
	auto res = parse_file("input");

	for (const auto &r: res) {
		std::cout <<
			"\ntemp = " << r.temp <<
			"\ndesc = " << r.desc <<
			"\nurl  = " << r.url  <<
			std::endl;
	}

	return EXIT_SUCCESS;
}
