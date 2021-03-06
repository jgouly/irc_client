#export CPP   =clang++
export CPP  =g++
export LNK =$(CPP)

#export OPTS         =-Os
export OPTS =-O0 -ggdb 
#-DCONS_FAST_COMPILE -D_GLIBCXX_DEBUG

INC          =-I /usr/include/python2.7
LIB          =-lncurses -lboost_python -lboost_system -lboost_program_options -lboost_signals -lpython2.7 -lpthread
#LIB          =-Wl,-Bstatic -lncurses -lboost_python -lboost_system -lboost_program_options -lboost_signals -Wl,-Bdynamic -lpython2.7 -lpthread
#-DNDEBUG
export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter -Wfatal-errors -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -DBOOST_RESULT_OF_USE_DECLTYPE -DUSING_PYTHON 
export LFLAGS=$(OPTS) -flto

#slow objects are library elements and spirit parsers 
SLOW_OBJS=ui/console.o controller_parse_text.o irc/irc.o python_interface.o
FAST_OBJS=controller.o program.o console_ui.o message.o message_stencil.o channel_buffer.o session_buffer.o error_buffer.o buffer.o unique_connection.o window.o util.o
OBJS=$(SLOW_OBJS) $(FAST_OBJS)

all: irc_client

irc_client: $(OBJS)
	cd ui && $(MAKE)
	cd irc && $(MAKE)
	$(LNK) $(OPTS) $^ -o $@ $(LIB)

ui/console.o:
	cd ui && $(MAKE)

irc/irc.o:
	cd irc && $(MAKE)

%.o: %.cpp 
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJS) irc_client
	cd ui && $(MAKE) clean
	cd irc && $(MAKE) clean

#quick clean for development
qclean:
	rm -rf $(FAST_OBJS) irc_client

stat:
	wc -l *.hpp *.cpp
