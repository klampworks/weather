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
	auto a = pt.get_child("data.current_condition..weatherDesc..");
	std::cout  << a.begin()->second.data() << std::endl;

	/* Yes the two dots at the end are fucking important. */
	for (ptree::value_type &v: pt.get_child("data.current_condition..")) {

		/* TODO It's late and I don't give a shit about
		   learning this retarded API.*/
		if (v.first.data() == std::string("temp_C")) {
			std::cout << v.second.data() << std::endl;
		}

		if (v.first.data() == std::string("weatherDesc")) {

			
			std::cout << v.second.get_child(".value").begin()->first.data() << std::endl;

		}

			std::cout << v.first.data() << std::endl;


//weatherIconUrl


	}



	return EXIT_SUCCESS;
}
