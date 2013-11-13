#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <boost/asio/io_service.hpp>

#include "ui/console.hpp"

#include "message.hpp"
#include "message_stencil.hpp"
#include "unique_connection.hpp"

class buffer;

/*
** This a user defined UI element, it will in essence be a view onto a single
** buffer, there maybe multiple view
*/
namespace ui_impl {

using namespace cons;

class window : public base {
//Member types
	using anchor_top   =anchor_view<anchors::top>;
	using anchor_bottom=anchor_view<anchors::bottom>;
	using text_box     =stenciled_frame<string_stencil>;
	using msg_list     =stenciled_list<message_stencil>;
//Member variables
	unique_connection con_topic_change, 
	                  con_new_msg;

	std::reference_wrapper<buffer> buf_;
//UI elements
	anchor_top          title_anchor;
	anchor_bottom&      input_anchor;
	anchor_bottom&      status_anchor;

	text_box&           title;
	async_input_box&    input;
	text_box&           status;
	msg_list&           message_list;

//helpers
	//resets the internal routing for the buffer stored in buf_
	void retarget_buffer();
public:
	window(unique_window_ptr        handle, 
	       boost::asio::io_service& io_service, //for async_input_box
	       buffer&                  buf);
	buffer&       get_buffer();
	const buffer& get_buffer() const;
	void set_target_buffer(buffer& buf);

//Overrides :- all of these will be forwarded on to the top level anchor
	void clear()                                      override;
	void refresh()                                    override;
	void set_position(const point& position)          override;
	void set_dimension(const point& dimension)        override;
	point get_position()  const                       override;
	point get_dimension() const                       override;
	unique_window_ptr reset(unique_window_ptr handle) override;

	template<typename F> cons::bsig::connection connect_on_text_input(F&& f);
}; //class window

template<typename F>
bsig::connection window::connect_on_text_input(F&& f) {
	return input.connect_on_input(std::forward<F>(f));
}

} //namespace ui_impl

using ui_impl::window;

#endif //WINDOW_HPP
