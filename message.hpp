#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "irc/channel.hpp"
#include "irc/user.hpp"
#include "irc/prefix.hpp"

#include <chrono>
#include <string>

class message_vistor;

class message {
	using time_point=std::chrono::system_clock::time_point;
	time_point time_stamp;
public:
	message(time_point time_stamp_);
	message(); //time_stamp == now()

	message(message&&)                =default;
	message(const message&)           =default;
	message& operator=(message&&)     =default;
	message& operator=(const message&)=default;
	virtual ~message();

	const time_point& get_time_stamp() const;

	virtual void visit(message_vistor&)=0;
}; //class message


class chan_message : public message {
	std::string sender;
	std::string content;
public:
	chan_message(std::string sender_, std::string content_);
	chan_message(chan_message&&)                =default;
	chan_message(const chan_message&)           =default;
	chan_message& operator=(chan_message&&)     =default;
	chan_message& operator=(const chan_message&)=default;

	void visit(message_vistor& visitor) override;

	const std::string& get_sender() const;
	const std::string& get_content() const;
}; //chan_message

class join_message : public message {
	irc::prefix prefix;
public:
	join_message(irc::prefix prefix_);
	join_message(join_message&&)                =default;
	join_message(const join_message&)           =default;
	join_message& operator=(join_message&&)     =default;
	join_message& operator=(const join_message&)=default;

	void visit(message_vistor& visitor) override;

	const irc::prefix& get_prefix() const;
}; //chan_message


class part_message : public message {
	irc::prefix          prefix;
	irc::optional_string message;
public:
	part_message(irc::prefix prefix_, irc::optional_string message_);
	part_message(part_message&&)                =default;
	part_message(const part_message&)           =default;
	part_message& operator=(part_message&&)     =default;
	part_message& operator=(const part_message&)=default;

	void visit(message_vistor& visitor) override;

	const irc::prefix&          get_prefix()  const;
	const irc::optional_string& get_message() const;
}; //chan_message

class motd_message : public message {
	std::string motd;
public:
	motd_message(std::string motd_);
	motd_message(motd_message&&)                =default;
	motd_message(const motd_message&)           =default;
	motd_message& operator=(motd_message&&)     =default;
	motd_message& operator=(const motd_message&)=default;

	void visit(message_vistor& visitor) override;

	const std::string& get_motd() const;
}; //chan_message


class error_message : public message {
	std::string error;
public:
	error_message(std::string error_);
	error_message(error_message&&)                =default;
	error_message(const error_message&)           =default;
	error_message& operator=(error_message&&)     =default;
	error_message& operator=(const error_message&)=default;

	void visit(message_vistor& visitor) override;

	const std::string& get_error() const;
}; //chan_message

/*
class names_list : public message {
	struct nick_details { std::string user_nick; bool is_operator; };
	std::vector<nick_details> nicks;

	template<typename Iter>
	names_list(Iter first, Iter last, const irc::channel& ch) {
		std::transform(first, last, std::back_inserter(nicks),
			[&](const irc::user& u) {
				return nick_details { u.get_nick(), ch.is_operator(u) };
			}
		);
	}
};
*/

class list_message : public message {
public:
	//text, colour
	using value_type     =std::pair<std::string, short>;
	using const_reference=const value_type&;
	using container_type =std::vector<value_type>;
	using const_iterator =container_type::const_iterator;
	
	template<typename Iter>
	list_message(Iter first, Iter last) 
	:	store(first, last)
	{   }

	void visit(message_vistor& visitor) override;
	std::size_t max_element_size() const;

	const_iterator begin() const;
	const_iterator end() const;
private:
	container_type store;
}; //list_message

#endif //MESSAGE_HPP
