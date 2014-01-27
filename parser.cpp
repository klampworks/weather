#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <sstream>
#include <string>
 
using namespace boost;
using namespace property_tree;;

int main()
{
	std::stringstream ss;
	ss << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";

	ptree pt;
	read_json(ss, pt);

	for(ptree::value_type &v: pt.get_child("root.values")) {

		assert(v.first.empty()); // array elements have no names
		std::cout << v.second.data() << std::endl;
	}

	return EXIT_SUCCESS;
}
