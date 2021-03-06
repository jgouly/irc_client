#ifndef USER_HPP
#define USER_HPP

#include "types.hpp"
#include "prefix.hpp"

#include <string>

namespace irc {

class user {
	std::string  nick;
	prefix       pfx;
//signals
	sig_ch_usr_s on_channel_message;
	sig_usr_s    on_direct_message;
	sig_usr_s    on_nick_change;
	sig_usr_s    on_notice;

//deleted functions
	user(const user&)           =delete;
	user(user&&)                =delete;
	user& operator=(const user&)=delete;
	user& operator=(user&&)     =delete;
public:
	user(std::string nick_);
	user(std::string nick_, prefix pfx_);

	//USER INTERFACE
	const std::string& get_nick()    const;
	const prefix&      get_prefix()  const;

	//SYSTEM INTERFACE 
	void set_nick(std::string nick_);
	void set_prefix(prefix pfx_);

	void channel_message(const shared_channel& chan, 
	                     const std::string& message);
	void direct_message(const std::string& message);
	void notice(const std::string& notice);
	
	template<typename F>
	bsig::connection connect_on_channel_message(F&& f);
	template<typename F>
	bsig::connection connect_on_direct_message(F&& f);
	template<typename F>
	bsig::connection connect_on_nick_change(F&& f);
	template<typename F>
	bsig::connection connect_on_notice(F&& f);
}; //class user

template<typename F>
bsig::connection user::connect_on_channel_message(F&& f) {
	return on_channel_message.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_direct_message(F&& f) {
	return on_direct_message.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_nick_change(F&& f) {
	return on_nick_change.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection user::connect_on_notice(F&& f) {
	return on_notice.connect(std::forward<F>(f));
}

} //namespace irc

#endif //USER_HPP
