#ifndef ASYNC_INPUT_BOX_HPP
#define ASYNC_INPUT_BOX_HPP

#include "basic.hpp"
#include "signals.hpp"
#include "ctrl_char.hpp"
#include "input_manager.hpp"
#include "string_stencil.hpp"

#include <functional>
#include <string>

namespace cons {

class async_input_box : public base {
public:
	//TODO use boost::signals
	//using grow_cb     =std::function<bool(point)>;
	//reference of some kind
	//using input_cb    =std::function<void(std::string)>;
	//using ctrl_char_cb=std::function<void(ctrl_char)>;
private:
	sig_pt                  on_grow;
	sig_s                   on_input;
	sig_ctrl_ch             on_ctrl_char;	
	
	frame                    frame_;
	std::string              value;
	unsigned                 pos = 0;
	string_stencil           stencil;
	input_manager            in_manager;
	boost::asio::io_service *io_service;
public:
	async_input_box(unique_window_ptr ptr, 
	                boost::asio::io_service& io_service);
	void stop();
	point calc_cursor_position() const;

	void handle_read_error(const boost::system::error_code&);
	void handle_read_complete(std::string str);
	void set();
	bool grow(point pt);
	void refresh();
	void clear();
	void set_value(const std::string& str);
	const std::string& get_value() const;
//Overrides
	void set_position(const point& position) override;
	void set_dimension(const point& dimension) override;
	point get_position()  const override;
	point get_dimension() const override;

	unique_window_ptr reset(unique_window_ptr handle);
//Colour
	void set_background(short bg);
	void set_foreground(short fg);
	short get_background() const;
	short get_foreground() const;

	//watch thread safety
	//what happens if a thread on this callback modifies this value?
	template<typename F>
	bsig::connection connect_on_grow(F&& f);

	template<typename F>
	bsig::connection connect_on_input(F&& f);

	template<typename F>
	bsig::connection connect_on_ctrl_char(F&& f);
}; //class async_input_box



template<typename F>
bsig::connection async_input_box::connect_on_grow(F&& f) {
	return on_grow.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection async_input_box::connect_on_input(F&& f) {
	return on_input.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection async_input_box::connect_on_ctrl_char(F&& f) {
	return on_ctrl_char.connect(std::forward<F>(f));
}


} //namespace cons

#endif //ASYNC_INPUT_BOX_HPP
