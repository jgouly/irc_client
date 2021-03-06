#include "message.hpp"
#include "buffer.hpp"

#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <string>



channel_buffer::channel_buffer(irc::channel& chan_) 
:	buffer        { chan_.get_name()     }
,	channel_bound ( chan_                )
,	session_bound ( chan_.get_session()  )
,	chan          ( chan_                )
{
	//TODO: make the signal handlers exception safe
	auto usr_sig=chan.connect_on_user_join(
		[&](const irc::channel& chan_, const irc::user& user) {
			assert(&chan_==&chan);
			messages.push_back(
				std::make_shared<join_message>(user.get_prefix())
			);
			on_new_msg(*this, messages.back());
		}
	);

	auto usr_prt_sig=chan.connect_on_user_part(
		[&](const irc::channel& chan_, const irc::user& user, 
		                               const irc::optional_string& str) {
			assert(&chan_==&chan);
			messages.push_back(
				std::make_shared<part_message>(user.get_prefix(), str)
			);
			on_new_msg(*this, messages.back());
		}
	);

	auto msg_sig=chan.connect_on_message(
		[&](const irc::channel& chan_, const irc::user& user, 
			                           const std::string& str) {
			assert(&chan_==&chan);
			messages.push_back(
				std::make_shared<chan_message>(user.get_nick(), str)
			);
			on_new_msg(*this, messages.back());
		}
	);

	auto topic_sig=chan.connect_on_topic_change(
		[&](const irc::channel& chan_, const std::string& topic) {
			assert(&chan_==&chan);
			on_topic_change(*this, topic);
		}
	);


	//Didn't really want any more nested functions
	connections.assign( { 
		std::move(usr_sig), std::move(usr_prt_sig), 
		std::move(msg_sig), std::move(topic_sig) 
	} );

	connections.push_back(
		chan.connect_on_list_users(
			std::bind(&channel_buffer::list_names, this))
	);
}

void channel_buffer::list_names() {
	auto first=chan.begin_users(),
	     last=chan.end_users();
	
	auto to_list_pair=[](const irc::user& u) {
		return std::make_pair(u.get_nick(), -1);
	};

	messages.push_back(std::make_shared<list_message>(
			boost::make_transform_iterator(first, to_list_pair),
			boost::make_transform_iterator(last, to_list_pair)
		)
	);
	on_new_msg(*this, messages.back());
}

channel_buffer::~channel_buffer() {
	for(auto& con : connections) con.disconnect();
}

const std::string& channel_buffer::get_topic() const {
	return chan.get_topic();
}
