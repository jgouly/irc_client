#ifndef STRING_STENCIL_HPP
#define STRING_STENCIL_HPP

#include "basic.hpp"

#include <string>

namespace cons {

class string_stencil {
	int calculate_y(int max_width, std::string::size_type chars) const;
	int calculate_x(int max_width, int chars) const;
public:
//types
	using value_type=std::string;
//members
	string_stencil()                     =default;
	string_stencil(const string_stencil&)=default;
//STENCIL INTERFACE
	int required_y(int max_width, std::string::size_type chars) const;
	point write_to(frame& frame_, const std::string& str) const;
}; //class string_stencil

} //namespace cons

#endif //STRING_STENCIL_HPP
