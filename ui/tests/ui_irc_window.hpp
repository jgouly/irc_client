#ifndef UI_IRC_WINDOW_HPP
#define UI_IRC_WINDOW_HPP

#include <console.hpp>

#include <sstream>


#include <thread>
#include <chrono>
#include <iostream>

using namespace cons;
class irc_ui_window {
//Member types
	using text_list    =stenciled_list<string_stencil>;
	using text_box     =stenciled_frame<string_stencil>;
	using anchor_top   =anchor_view<anchors::top>;
	using anchor_bottom=anchor_view<anchors::bottom>;
	using anchor_left  =anchor_view<anchors::left>;
//Member variables
	anchor_top          parent;
	anchor_bottom&      input_anchor;
	anchor_bottom&      status_anchor;
	anchor_left&      channel_anchor;
	bordered&          channel_border;
	text_list&          channel_list;
	text_list&          message_list;
	text_box&           title;
	text_box&           status;
	input_box&          input;
public: 
	irc_ui_window() 

	:	parent         { make_window(), 1 }	
	,	input_anchor   (parent.emplace_fill<anchor_bottom>(1))
	//,	channel_anchor (input_anchor.emplace_fill<anchor_left>(20))

	,	status_anchor  (input_anchor.emplace_fill<anchor_bottom>(1))
	,	channel_anchor (status_anchor.emplace_fill<anchor_left>(20))

	,	channel_border(channel_anchor.emplace_anchor<bordered>(borders::right))
	,	channel_list   (channel_border.emplace_element<text_list>()) 

	,	message_list   (channel_anchor.emplace_fill<text_list>())
	,	title          (parent.emplace_anchor<text_box>("my irc client") )

	//,	input          (input_anchor.emplace_anchor<text_box> ("input box"))

	,	status         (status_anchor.emplace_anchor<text_box>("channel's status"))

	,	input          (input_anchor.emplace_anchor<input_box>())
	{
		/*
		input.reg_on_grow(
			[&](const point& pt) {
				input_anchor.set_partition(pt.y);
				input.set_background(COLOR_RED);
				std::ostringstream oss;
				oss << status_anchor.get_dimension() << "  " << status_anchor.get_position();
				status_anchor.refresh();
				title.set_content(oss.str());
				title.refresh();
				return true;
			}
		);
		*/

		channel_list.insert(channel_list.begin(), "hello");
		channel_list.insert(channel_list.begin(), "world");
		channel_list.insert(channel_list.begin(), "01234567890123456789");

		message_list.insert(message_list.begin(), "msg1");
		message_list.insert(message_list.begin(), std::string(400, 'a'));

		status.set_background(COLOR_CYAN);
		status.set_content("Hi");

		title.set_background(COLOR_CYAN);

		parent.refresh();
	}

	void refresh() {
		parent.refresh();
	}

	void run() {
		cbreak();
		noecho();
		while(input.get_char()!='q') {
			input_anchor.set_partition(input_anchor.get_partition()+1);

			std::ostringstream ss;
			ss << channel_list.get_dimension();

			channel_list.insert(channel_list.begin(), ss.str());
			parent.refresh();
		}
	}
}; //class irc_ui_window

#endif //UI_IRC_WINDOW_HPP
