# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/08 08:53:39 by dylan             #+#    #+#              #
#    Updated: 2023/01/29 21:57:32 by dsaada           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX 		= c++

CXXFLAGS 	= -Wall -Wextra -std=c++98

SRCS 		= $(wildcard ./srcs/*.cpp)

INCLUDES 	=  $(wildcard ./includes/*.hpp)

INCPATH 	= -I./includes -I.

NAME 		= ircserv

OBJS 		= $(SRCS:.cpp=.o)

CHUT		= --no-print-directory

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ -c $<

$(NAME) : $(OBJS) $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all : $(NAME) proxy

proxy : 
	@cd proxy && make $(CHUT)

clean : 
	@rm -f $(OBJS)
	@cd proxy && make clean $(CHUT)

fclean : clean
	@rm -f $(NAME)
	@cd proxy && make fclean $(CHUT)


re: fclean all

cleaner: re clean

.PHONY: all clean cleaner fclean re proxy
	
