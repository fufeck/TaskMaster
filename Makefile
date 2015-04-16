#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/20 15:15:50 by ftaffore          #+#    #+#              #
#    Updated: 2015/01/18 16:24:19 by chery            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		=	Taskmaster

CC			=	g++

RM			=	rm -rf

INCLUDES	=	-Iincludes

LIBS		=	-std=c++0x

CFLAGS		=	-Werror -Wall -Wextra

SRCS		=	srcs/main.cpp			\
				srcs/ParseYaml.cpp 		\
				srcs/Taskmaster.cpp		\
				srcs/ProgramFeature.cpp	\
				srcs/Program.cpp
				
				
OBJS		=	$(SRCS:.cpp=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS)
				$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES)

.cpp.o		:
				$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re
