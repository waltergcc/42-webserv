# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wcorrea- <wcorrea-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/30 11:36:55 by wcorrea-          #+#    #+#              #
#    Updated: 2023/10/07 20:22:54 by wcorrea-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC = src/main.cpp \
		src/Service.cpp \
		src/FileChecker.cpp \
		src/ServerInfo.cpp

OBJS = ${SRC:.cpp=.o}

INCLUDE = -I include

CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -pedantic -std=c++98

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