#include "parser.hpp"
#include "weather_day.hpp"
#include <iostream>
#include <string>

int main()
{
	auto res = parser::parse_file("input");

	for (const auto &r: res) {
		std::cout <<
			"\ndate = " << r.date <<
			"\ntemp = " << r.temp <<
			"\ndesc = " << r.desc <<
			"\nurl  = " << r.url  <<
			std::endl;
	}

	return 0;
}
