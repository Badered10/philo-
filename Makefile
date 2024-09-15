# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 18:07:11 by baouragh          #+#    #+#              #
#    Updated: 2024/09/15 13:56:01 by baouragh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME			:= philo
NAME_B			:= philo_bonus

CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror -pthread #-fsanitize=thread

SRCS_M			:=	philo_m/philo.c philo_m/ft_isdigit.c philo_m/ft_atol.c \
					philo_m/simulation_utils.c philo_m/set_and_get.c \
					philo_m/scaner.c philo_m/init_utils.c philo_m/init_data.c

SRCS_B			:=	philo_b/philo_bonus.c

					
OBJS_M			:=	$(SRCS_M:.c=.o)
OBJS_B			:=	$(SRCS_B:.c=.o)

all:	$(NAME)

$(NAME):$(OBJS_M)
	$(CC) $(CFLAGS) $(OBJS_M) -o $(NAME) 

BONUS: $(NAME_B)

$(NAME_B):$(OBJS_B)
	$(CC) $(CFLAGS) $(OBJS_B) -o $(NAME) 

clean:
	@rm -f $(OBJS_M) $(OBJS_B)

fclean: clean
	@rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY: clean
.SECONDERY: $(OBJS_M) $(OBJS_B)