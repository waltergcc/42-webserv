# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/30 11:36:55 by wcorrea-          #+#    #+#              #
#    Updated: 2023/10/25 10:09:45 by wcorrea-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC = src/main.cpp \
		src/Service.cpp \
		src/FileChecker.cpp \
		src/ServerInfo.cpp \
		src/Client.cpp \
		src/Script.cpp \
		src/utils.cpp

OBJS = ${SRC:.cpp=.o}

INCLUDE = -I include

CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -pedantic -std=c++98 -g

.cpp.o:
		${CC} ${CPPFLAGS} ${INCLUDE} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
		${CC} ${CPPFLAGS} ${INCLUDE} ${OBJS} -o ${NAME}

all:	${NAME}

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re: clean all

.PHONY: all clean re