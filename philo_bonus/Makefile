# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 18:07:11 by baouragh          #+#    #+#              #
#    Updated: 2024/09/29 10:50:01 by baouragh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= philo_bonus

CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror -pthread #-fsanitize=address -g3 

SRCS_B			:=	main_bonus.c ft_isdigit_bonus.c \
					ft_atol_bonus.c \
					simulation_utils_bonus.c \
					set_and_get_bonus.c scaner_bonus.c \
					init_utils_bonus.c init_data_bonus.c \
					ft_itoa_bonus.c ft_strjoin_bonus.c ft_strlen_bonus.c
				

OBJS_B			:=	$(SRCS_B:.c=.o)

all:	bonus

bonus: $(NAME)

$(NAME):$(OBJS_B)
	$(CC) $(CFLAGS) $(OBJS_B) -o $(NAME) 

clean:
	@rm -f $(OBJS_B)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
