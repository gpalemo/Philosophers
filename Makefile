# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/21 21:16:01 by cmauley           #+#    #+#              #
#    Updated: 2026/06/02 00:48:41 by cmauley          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RESET			= "\033[0m"
BLACK    		= "\033[30m"    # Black
RED      		= "\033[31m"    # Red
GREEN    		= "\033[32m"    # Green
YELLOW   		= "\033[33m"    # Yellow
BLUE     		= "\033[34m"    # Blue
MAGENTA  		= "\033[35m"    # Magenta
CYAN     		= "\033[36m"    # Cyan
WHITE    		= "\033[37m"    # White

# Compiler
NAME			= philo
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
MKDIR			= mkdir -p
RM				= rm -rf
LINKER  	    = -lpthread

# Includes
INCLUDES_DIR 	= includes
INCLUDES_FLAG 	= -I$(INCLUDES_DIR)
INCLUDES		= $(wildcard $(INCLUDES_DIR)/*.h)

# Sources
SRCS_DIR		= srcs/
PARSING_DIR		= srcs/parsing/
SRC_FILES		= main.c \
				  init.c \
				  simulation.c \
				  str_utils.c
PARSING_FILES		= parse.c \
				  utils.c

# Objects
OBJS_DIR		= objs/
OBJ_FILES		= $(SRC_FILES:.c=.o)
PARSING_OBJ		= $(PARSING_FILES:.c=.o)
OBJS			= $(addprefix $(OBJS_DIR), $(OBJ_FILES)) $(addprefix $(OBJS_DIR)parsing/, $(PARSING_OBJ))


all : $(OBJS_DIR) $(NAME)

$(OBJS_DIR) :
	@$(MKDIR) $(OBJS_DIR)
	@$(MKDIR) $(OBJS_DIR)parsing/

$(NAME) : $(OBJS) Makefile
	@echo $(GREEN) " - Compiling $(NAME)..." $(RESET)
	@$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME)
	@echo $(YELLOW) " - Compiling FINISHED" $(RESET)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c $(INCLUDES)
	@$(CC) $(CFLAGS) $(INCLUDES_FLAG) -c $< -o $@

$(OBJS_DIR)parsing/%.o : $(PARSING_DIR)%.c $(INCLUDES)
	@$(CC) $(CFLAGS) $(INCLUDES_FLAG) -c $< -o $@	

clean :
	@$(RM) $(OBJS_DIR)
	@echo $(RED) " - Cleaned!" $(RESET)

fclean : clean
	@$(RM) $(NAME)
	@echo $(RED) " - Full Cleaned!" $(RESET)

re: fclean all

.PHONY: all clean fclean re