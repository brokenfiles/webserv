NAME		= webserv
CC			= clang++
RM			= rm -f
OBJS_DIR	= ./objects/
START		= 1
CFLAGS		= -Wall -Wextra -Werror -std=c++98

SRCSC		= srcs/webserv.cpp \
			 srcs/classes/server/Server.cpp  \
			 srcs/classes/server/ServerManager.cpp  \
			 srcs/classes/client/Client.cpp  \
			 srcs/classes/queries/Query.cpp  \
			 srcs/classes/queries/Request.cpp  \
			 srcs/classes/config/Config.cpp  \
			 srcs/classes/config/ServerConfig.cpp  \
			 srcs/classes/config/LocationConfig.cpp  \
			 srcs/classes/queries/Response.cpp  \
			 srcs/classes/parser/Parser.cpp  \
			 srcs/classes/cgi/Cgi.cpp \
			 srcs/utils/utils.cpp

OBJS		= ${SRCSC:%.cpp=${OBJS_DIR}/%.o}

${OBJS_DIR}/%.o: %.cpp
			@mkdir -p ${@D}
			@printf "\033[2K\033[0;35mWEBSERV\033[0;0m:    \033[0;33mCompilation...    \033[0;31m%-15.15s\033[0;0m\r" $(notdir ${<})
			@${CC} ${CFLAGS} -c $< -o $@

${NAME}:	header ${OBJS}
			@printf "\033[2K\033[0;35mWEBSERV\033[0;0m:    \033[0;32mCompleted         \033[0;31m----\033[0;0m          \r"
			@printf "\n\033[0;0m⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n\033[0;35mWEBSERV\033[0;0m:    \033[0;32mReady             \033[0;31m----          \033[0;0m\n⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n"
			@${CC} ${CFLAGS} ${OBJS} -o ${NAME}

run:		${NAME}
			@echo "\033[0;34mRunning ${NAME}...\033[0;0m"
			@./${NAME}

client:
			@${CC} srcs/classes/client/Client_TCP.cpp -o client
			./client

test:		re
			@echo "\033[0;34mRunning ${NAME}...\033[0;0m"
			@./${NAME}

header:
			@printf "\033[0;0m⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n\033[0;35mNAME          \033[0;0mSTATUS            \033[0;31mFILE          \033[0;0m\n⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n"

all:		${NAME}

clean:
			@${RM} ${OBJS}
			@printf "\033[0;35mPROJECT\033[0;0m:      \033[0;32mCleaned\033[0;0m\n"

fclean:		clean
			@${RM} ${NAME}

re:			fclean all

silent:
			@:

.PHONY:		all clean fclean re