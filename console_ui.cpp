#include "console_ui.hpp"

#include "ui/console.hpp"

namespace ui_impl {

ui::ui(boost::asio::io_service& io_service_)  
:	parent         { make_window(), 1 }	
,	input_anchor   (parent.emplace_fill<anchor_bottom>(1))
,	status_anchor  (input_anchor.emplace_fill<anchor_bottom>(1))
,	channel_anchor (status_anchor.emplace_fill<anchor_left>(20))
,	channel_border (channel_anchor.emplace_anchor<bordered>(borders::right))
,	channel_list   (channel_border.emplace_element<text_list>()) 
,	message_list   (channel_anchor.emplace_fill<msg_list>())
,	title          (parent.emplace_anchor<text_box>("my irc client") )
,	status         (status_anchor.emplace_anchor<text_box>("channel's status"))
,	input          (input_anchor.emplace_anchor<async_input_box>(io_service_))

,	io_service     { &io_service_ }
{	
	title.set_background(COLOR_CYAN);
	status.set_background(COLOR_CYAN);

	input.connect_on_grow(
		[&](const point& pt) {
			input_anchor.set_partition(pt.y);
			input_anchor.refresh();
			return true;
		}
	);

	input.connect_on_ctrl_char(
		[&](cons::ctrl_char ch) {
			on_ctrl_char(ch);
		}
	);

	message_list.selected_idx(1000);
	refresh();
}

void ui::refresh() {
	parent.refresh();
	input.refresh();
} 

void ui::stop() {
	input.stop();
}

//TODO: note to self, what happens if you change this on THIS callback?
//perhaps use the event system
void ui::reg_on_text_input(std::function<void(std::string)> action){
	on_text_input=std::move(action);
	input.connect_on_input(on_text_input);
}

void ui::set_input(const std::string& str) {
		input.set_value(str);
		input.refresh();
}

//setters
void ui::set_title(const std::string& text){ 
	title.set_content(text);
	title.refresh();
}

void ui::append_message(const message_p& msg) {
	message_list.insert(message_list.end(), msg);
	message_list.refresh();
}

} //namespace ui_impl
