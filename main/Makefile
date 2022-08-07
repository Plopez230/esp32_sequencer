PATH = C:/Users/Pablo/Documents/GitHub/esp32_sequencer/main/
NAME =	$(PATH)esp32_sequencer
SRCS = 	$(PATH)seq_mpr121.cpp \
		$(PATH)seq_ring.cpp \
		$(PATH)seq_sdcard.cpp \
		$(PATH)seq_st7920.cpp \
		$(PATH)seq_ym2413.cpp
OBJS= $(SRCS:.cpp=.o)
RM = rm -rf

$(NAME): 
	gcc $(SRCS) -o $(NAME)

all: $(NAME)

clean:
	$(RM) $(NAME)

fclean:	clean

re:	fclean all

.PHONY: all, clean, fclean, re