# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/06 17:40:55 by ecullier          #+#    #+#              #
#    Updated: 2023/11/11 22:09:30 by ecullier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philosopher
HEADER	= philosopher.h
CC	= gcc
CFLAGS	= -Werror -Wall -Wextra  -pthread -g -g3

SRC_PATH = srcs/
OBJ_PATH = objects/

SRC	= philosopher.c \
			#check.c \
				init.c \
				next.c \
				routine.c \
				utils.c

INC		= -I ./includes/

SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "Philo compile"

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)
	@echo "🧽 Philosophers cleaned"

re: fclean all

.PHONY: all re clean fclean
