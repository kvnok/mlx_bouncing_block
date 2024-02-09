SHELL := /bin/bash

NAME := bounce

SRC_FILES	:=	dvdbounce.c

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
MLX_DIR := MLX42

OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
DEP_FILES := $(OBJ_FILES:.o=.d)

# SRC_SUBD = $(shell find $(SRC_DIR) -type d)
# OBJ_SUBD = $(subst $(SRC_DIR), $(OBJ_DIR), $(SRCSUBD))

MLX42 := $(MLX_DIR)/build/libmlx42.a

LIBFT_DIR := libft
LIBFT := libft/libft.a

CC  := cc
IFLAGS := -I$(INC_DIR) -I$(MLX_DIR)/include -I$(MLX_DIR)/include/$(MLX_DIR) -I$(LIBFT_DIR)
CFLAGS := -Wall -Wextra -Werror -MMD -MP -g
LFLAGS := -L$(MLX_DIR)/build -lmlx42 -lglfw -ldl -pthread -lm -L$(LIBFT_DIR) -lft

_DEBUG := 1
ifeq ($(_DEBUG),1)
	LFLAGS += -g3 -fsanitize=address
endif

all: $(MLX42) $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --quiet

$(MLX42):
	git submodule update --init
	@cmake $(MLX_DIR) -B $(MLX_DIR)/build
	$(MAKE) -C $(MLX_DIR)/build -j4 --quiet

$(NAME): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

-include $(DEP_FILES)

clean:
	@$(MAKE) clean -C $(MLX_DIR)/build -j4 --quiet
	@$(MAKE) clean -C $(LIBFT_DIR) --quiet
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) clean/fast -C $(MLX_DIR)/build -j4 --quiet
	$(MAKE) fclean -C $(LIBFT_DIR) --quiet
	-rm -f $(NAME)

re: fclean all

test: all
	./test.sh

.PHONY: all, clean, fclean, re