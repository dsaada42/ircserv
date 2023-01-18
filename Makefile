# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/08 08:53:39 by dylan             #+#    #+#              #
#    Updated: 2023/01/18 15:23:04 by dsaada           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX 		= c++

CXXFLAGS 	= -Wall -Werror -Wextra -std=c++98

SRCS 		= $(wildcard ./srcs/*.cpp)

INCLUDES 	=  $(wildcard ./includes/*.hpp)

INCPATH 	= -I./includes -I.

NAME 		= ircserv

OBJS 		= $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ -c $<

$(NAME) : $(OBJS) $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all : $(NAME)

proxy : 
	@cd proxy && make

clean : 
	@rm -f $(OBJS)
	@cd proxy && make clean

fclean : clean
	@rm -f $(NAME)
	@cd proxy && make fclean


re: fclean all

.PHONY: all clean fclean re proxy
	