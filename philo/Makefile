# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/11 23:36:10 by clu               #+#    #+#              #
#    Updated: 2025/05/31 18:04:29 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = philo

# Comiler and Flags
CC 		= cc
CFLAGS 	= -Wall -Werror -Wextra
# Directories
SRC_DIR = ./src
OBJ_DIR = ./obj

# Libraries

# Header files
HEADERS = -I include
HEADERS_DEP = include/philo.h

# Source files and object files
SRC = 	$(SRC_DIR)philo.c \
		$(SRC_DIR)init.c \
		$(SRC_DIR)routines.c \
		$(SRC_DIR)routines_utils.c \
		$(SRC_DIR)threads.c \
		$(SRC_DIR)threads_utils.c \
		$(SRC_DIR)utils.c \
		$(SRC_DIR)parse.c

OBJ = 	$(SRC:$(SRC_DIR)%=$(OBJ_DIR)/%)
OBJ := 	$(OBJ:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling $(NAME) ..."
	$(CC) $(CFLAGS) $(OBJ) $(HEADERS) -o $(NAME)
	@echo "Done"

# Compile the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS_DEP)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Cleaned up all object files"

fclean: clean
	@rm -f $(NAME)
	@echo "Cleaned up $(NAME)"
	@echo "Cleaned up everything"

re: fclean all
	@echo "Remade everything"

.SECONDARY: $(OBJ)

.PHONY: all clean fclean re
