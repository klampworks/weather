#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
 
using namespace boost;
using namespace property_tree;;

void print(boost::property_tree::ptree const& pt)
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

	//ss << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";

	ptree pt;
	read_json(ss, pt);

	//print(pt);
	//auto a = pt.get_child("data.current_condition..weatherDesc..");
	//std::cout  << a.begin()->second.data() << std::endl;

	auto a = pt.get_child("data.current_condition.");
	parse_weather_day(a.begin()->second);
	//a = pt.get_child("data.current_condition..weatherIconUrl..");
	//std::cout  << a.begin()->second.data() << std::endl;

	/* Yes the two dots at the end are fucking important. */
	for (ptree::value_type &v: pt.get_child("data.weather.")) {

		parse_weather_day(v.second);
		
#if 0
		/* TODO It's late and I don't give a shit about
		   learning this retarded API.*/
		if (v.first.data() == std::string("temp_C")) {
			std::cout << v.second.data() << std::endl;
		}

/*
		if (v.first.data() == std::string("weatherDesc")) {
			std::cout << 
				v.second.get_child(".value").begin()->first.data() 
				<< std::endl;
		}
		if (v.first.data() == std::string("weatherIconUrl")) {
			std::cout << 
				v.second.get_child(".value").begin()->first.data() 
				<< std::endl;
		}
		*/

		for (const auto &b: v.second) {
			std::cout << b.first.data() << std::endl;
		}


//weatherIconUrl
#endif

	}



	return EXIT_SUCCESS;
}
