#ifndef MESSAGE_VISTOR_HPP
#define MESSAGE_VISTOR_HPP

class message;
class chan_message;// : public message;

class message_vistor {
public:
	virtual void operator()(message&)     =0;
	virtual void operator()(chan_message&)=0;
	virtual void operator()(join_message&)=0;
	virtual void operator()(part_message&)=0;
	virtual void operator()(motd_message&)=0;
	virtual void operator()(error_message&)=0;
	virtual void operator()(list_message&)=0;
};

#endif //MESSAGE_HPP
