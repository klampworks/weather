#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
 
using namespace boost;
using namespace property_tree;;

/* Debug function for recursivly printing out a node and its
 * children. */
void print(const boost::property_tree::ptree &pt)
{
    using boost::property_tree::ptree;
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

void parse_weather_day(const ptree &pt)
{
	for (const auto &v: pt) {

		if (v.first.data() == std::string("tempMaxC")
			|| v.first.data() == std::string("temp_C")) {

			std::cout << v.second.data() << std::endl;
		} else if (v.first.data() == std::string("weatherDesc")) {
			std::cout << get_value(v) << std::endl;
		} else if (v.first.data() == std::string("weatherIconUrl")) {
			std::cout << get_value(v) << std::endl;
		}
	}
}

int main()
{
	std::ifstream ifs;
	ifs.open("input");
	std::string tmp;

	std::stringstream ss;
	while(std::getline(ifs, tmp)) 
		ss << tmp;

	ptree pt;
	read_json(ss, pt);

	auto a = pt.get_child("data.current_condition.");
	parse_weather_day(a.begin()->second);

	for (ptree::value_type &v: pt.get_child("data.weather.")) {

		parse_weather_day(v.second);
		
	}

	return EXIT_SUCCESS;
}
