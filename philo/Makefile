# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 18:07:11 by baouragh          #+#    #+#              #
#    Updated: 2024/09/28 20:55:54 by baouragh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			:= philo

CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror -pthread #-fsanitize=address -g3

SRCS_M			:=	philo.c ft_isdigit.c ft_atol.c \
					simulation_utils.c set_and_get.c \
					scaner.c init_utils.c init_data.c
								
OBJS			:=	$(SRCS_M:.c=.o)

all:	$(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
