# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/20 17:34:18 by kjimenez          #+#    #+#              #
#    Updated: 2023/06/22 22:12:40 by kjimenez         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIR_BIN				:= bin
DIR_OBJ				:= build

CLIENT_NAME			:= $(DIR_BIN)/client
SERVER_NAME			:= $(DIR_BIN)/server

SERVER_SRC			:= server.c
CLIENT_SRC			:= client.c
SERVER_OBJ			:= $(addprefix $(DIR_OBJ)/,$(SERVER_SRC:c=o))
CLIENT_OBJ			:= $(addprefix $(DIR_OBJ)/,$(CLIENT_SRC:c=o))

LIBS_PATH			:= libs
LIBFT_PATH			:= $(LIBS_PATH)/libft

CC					:= clang
CFLAGS				:= -Wall -Wextra -Werror -I$(LIBFT_PATH)/include -Iinclude -g
CLIENT_LDFLAGS		:= -L$(LIBFT_PATH)/bin -lft_stdio -lft_stdlib -lft_string
SERVER_LDFLAGS		:= -L$(LIBFT_PATH)/bin -lft_stdio -lft_math -lft_stdlib -lft_string

GIT					:= git

MKDIR				:= mkdir -p

INIT_SUB			:= $(shell $(GIT) submodule init)
UPDATE_SUB			:= $(shell $(GIT) submodule update)
BUILD_LIBFT			:= $(shell $(MAKE) -C $(LIBFT_PATH))

all: client server

client: $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) $(CLIENT_LDFLAGS) -o $(CLIENT_NAME)

server: $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) $(SERVER_LDFLAGS) -o $(SERVER_NAME)

$(DIR_OBJ)/%.o: %.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(CLIENT_OBJ) $(SERVER_OBJ)

fclean: clean
	$(RM) $(CLIENT_NAME) $(SERVER_NAME)

re: fclean client server

.PHONY: all client server clean fclean re

vpath %.c src
