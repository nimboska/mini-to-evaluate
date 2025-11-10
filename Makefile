# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/04 17:23:16 by ochmurzy          #+#    #+#              #
#    Updated: 2025/08/28 16:09:00 by ochmurzy         ###   ########.fr        #
#    Updated: 2025/08/28 16:09:00 by ochmurzy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME := minishell

# Compiler and flags
CC := cc
CFLAGS := -Wall -Wextra -Werror -g
LDFLAGS := -lreadline -lncurses

# Directories
LIBFT_DIR = libft

INCLUDES := -I$(LIBFT_DIR)

# Libraries
LIBFT_LIB := $(LIBFT_DIR)/libft.a

# Source and object files
SRC := \
	main.c \
	builtins1.c builtins2.c builtins3.c builtins4.c builtins5.c \
	env_things.c env_update.c \
	error.c \
	exec_single_utils.c exec_single.c\
	exec_single_cmd.c executor.c pipes.c\
	free_func.c \
	mini_init.c \
	handle_quote.c \
	tokenization.c tokens_utils.c \
	command_init.c command_init_utils.c\
	heredock.c \
	read_input.c \
	signals.c \
	utils.c \
	redir.c \
	get_next_line/get_next_line.c \
	get_next_line/get_next_line_utils.c \

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT_LIB) $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJ) $(LIBFT_LIB) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re