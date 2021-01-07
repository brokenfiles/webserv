GREEN	= \033[0;32m
GREEN_B	= \033[1;32m
RESET	= \033[0m

SRCS	=	srcs/webserv.cpp \
            srcs/classes/server/Server.cpp  \
            srcs/classes/client/Client.cpp  \
            srcs/classes/queries/Query.cpp  \
            srcs/classes/queries/Request.cpp  \
            srcs/classes/queries/Response.cpp  \
            srcs/classes/parser/Parser.cpp  \
            srcs/utils.utils.cpp \

OBJS	= $(SRCS:.cpp=.o)

CC		= clang++
CFLAGS	= -Wall -Wextra -Werror -D MAKE=yes -std=c++98
RM		= rm -f
NAME	= webserv

all:		$(NAME)

%.o:		%.cpp
			@$(CC) $(CFLAGS) -c $< -o $@
#        	@$(CC) $(CFLAGS) -c -MD $< -o $@

$(NAME):	$(OBJS)
			@$(CC) $(CFLAGS) $^ -o $@
#			@$(RM) *.d
#			@$(RM) *.o
			@printf "$(GREEN_B)$(NAME) $(GREEN)Created.$(RESET)\033[0;0m\n"


include_dep:
			@$(RM) *.d

clean:		include_dep
			@$(RM) $(OBJS)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re

#-include *.d

# $^ = liste des dependance
# $@ = cible de la règle.
# $< = première dependance
