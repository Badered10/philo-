# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 18:07:11 by baouragh          #+#    #+#              #
#    Updated: 2024/09/23 20:03:23 by baouragh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			:= philo_m
NAME_B			:= philo_b

CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror -pthread #-fsanitize=thread -g3

SRCS_M			:=	philo/philo.c philo/ft_isdigit.c philo/ft_atol.c \
					philo/simulation_utils.c philo/set_and_get.c \
					philo/scaner.c philo/init_utils.c philo/init_data.c

SRCS_B			:=	philo_bonus/philo_bonus.c philo_bonus/ft_isdigit_bonus.c \
					philo_bonus/ft_atol_bonus.c \
					philo_bonus/simulation_utils_bonus.c \
					philo_bonus/set_and_get_bonus.c philo_bonus/scaner_bonus.c \
					philo_bonus/init_utils_bonus.c philo_bonus/init_data_bonus.c \
					philo_bonus/ft_itoa_bonus.c philo_bonus/garbage_collector.c\
					philo_bonus/ft_strjoin_bonus.c philo_bonus/ft_strlen_bonus.c \
					
					
OBJS_M			:=	$(SRCS_M:.c=.o)
OBJS_B			:=	$(SRCS_B:.c=.o)

all:	$(NAME)

$(NAME):$(OBJS_M)
	$(CC) $(CFLAGS) $(OBJS_M) -o $(NAME) 

bonus: $(NAME_B)

$(NAME_B):$(OBJS_B)
	$(CC) $(CFLAGS) $(OBJS_B) -o $(NAME_B) 

clean:
	@rm -f $(OBJS_M) $(OBJS_B)

fclean: clean
	@rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY: clean
