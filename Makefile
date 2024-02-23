# --- VARIABLES --- #

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3# -fsanitize=address
SRCS_DIR = ./srcs/
SRCS =	main.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		utils.cpp \
		pass.cpp \
		nick.cpp \
		user.cpp \
		privmsg.cpp \
		quit.cpp \
		join.cpp \
		motd.cpp \
		part.cpp \
		ping.cpp \
		invite.cpp \
		kick.cpp \
		topic.cpp \
		who.cpp \
		mode.cpp \
		bot.cpp \
		list.cpp
SRCS_B	=	bot.cpp \
		Server_bonus.cpp \
		main.cpp \
		Client.cpp \
		Channel.cpp \
		utils.cpp \
		pass.cpp \
		nick.cpp \
		user.cpp \
		privmsg.cpp \
		quit.cpp \
		join.cpp \
		motd.cpp \
		part.cpp \
		ping.cpp \
		invite.cpp \
		kick.cpp \
		topic.cpp \
		who.cpp \
		mode.cpp \
		list.cpp
OBJ_DIR = objs/
OBJB_DIR = objs_b/
OBJ = ${SRCS:%.cpp=${OBJ_DIR}%.o}
OBJB = ${SRCS_B:%.cpp=${OBJB_DIR}%.o}
NAME = ircserv
NAMEB	=	bonus
RM = rm -f
HEADERS_DIR = ./incs/
HEADERS_FILES = Server.hpp Client.hpp Channel.hpp irc.hpp config.hpp rpl.hpp
HEADERS = $(addprefix ${HEADERS_DIR}/, ${HEADERS_FILES})

# --- COLORS --- #

_NOC	:=	\033[0m
_RED	:=	\033[1;31m
_GREEN	:=	\033[1;32m

# --- RULES --- #

${NAME} :	${OBJ}
		${CC} ${CFLAGS} ${OBJ} -o ${NAME}
		@echo "${_GREEN}### ${NAME} created ###${_NOC}\n"

${NAMEB} :	${OBJB}
		${CC} ${CFLAGS} ${OBJB} -o ${NAMEB}
		@echo "${_GREEN}### ${NAMEB} created ###${_NOC}\n"

${OBJ}: ${OBJ_DIR}%.o :	${SRCS_DIR}%.cpp ${HEADERS}
		@mkdir -p objs
		${CC} ${CFLAGS} -I/usr/include -I ${HEADERS_DIR} -c $< -o $@

${OBJB}: ${OBJB_DIR}%.o :	${SRCS_DIR}%.cpp ${HEADERS}
		@mkdir -p objs_b
		${CC} ${CFLAGS} -I/usr/include -I ${HEADERS_DIR} -c $< -o $@

all :	${NAME} ${NAMEB}

clean :
		${RM} -rf objs/
		${RM} -rf objs_b/
		@echo "${_RED}### Removed ${NAME} object files ###${_NOC}"

fclean :	clean
		${RM} ${NAME}
		${RM} ${NAMEB}
		@echo "${_RED}### Removed ${NAME} ###${_NOC}"

re : 	fclean
	${MAKE} all

.PHONY : all clean fclean re
