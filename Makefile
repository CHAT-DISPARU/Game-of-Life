# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: titan <titan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/13 12:35:12 by gajanvie          #+#    #+#              #
#    Updated: 2025/12/23 13:36:33 by titan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = game_of_life
SRCD = src/
OBJDIR      = objs/
LIBFT = ./src/libft/libft.a
GREEN=\033[0;32m
YELLOW=\033[0;33m
RED=\033[0;31m
BLUE=\033[0;34m
PURPLE=\033[0;35m
NC=\033[0m
LIB = MacroLibX
LIB_URL = https://github.com/seekrs/MacroLibX.git

SRCS = $(SRCD)jdlv.c \
		$(SRCD)draw_points.c \
		$(SRCD)save_map.c \
		$(SRCD)load_binary.c \
		$(SRCD)routine.c \
		$(SRCD)zoom.c

OBJS = $(SRCS:$(SRCD)%.c=$(OBJDIR)%.o)

CC = clang
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g -O3 -I ./include
MLX_DIR = MacroLibX
MLX = $(MLX_DIR)/libmlx.so

all: $(LIB) $(NAME) banner

banner :                                                           
	@echo "$(GREEN) ▄████   ▄▄▄  ▄▄   ▄▄ ▄▄▄▄▄ $(NC) $(BLUE) ▄████▄ ▄▄▄▄▄ $(NC) $(PURPLE) ██     ▄▄ ▄▄▄▄▄ ▄▄▄▄▄ $(NC)"
	@echo "$(GREEN)██  ▄▄▄ ██▀██ ██▀▄▀██ ██▄▄  $(NC) $(BLUE) ██  ██ ██▄▄  $(NC) $(PURPLE) ██     ██ ██▄▄  ██▄▄  $(NC)"
	@echo "$(GREEN) ▀███▀  ██▀██ ██   ██ ██▄▄▄ $(NC) $(BLUE) ▀████▀ ██    $(NC) $(PURPLE) ██████ ██ ██    ██▄▄▄ $(NC)"

$(NAME): src/libft/libft.a $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) -lSDL2 -lm -o $(NAME)
	@echo "$(GREEN)✅ Compilation of Game of Life finished !$(NC)"

$(LIB):
	@git clone https://github.com/seekrs/MacroLibX.git -b v2.2.2 MacroLibX --depth=1
	@$(MAKE) --no-print-directory -C MacroLibX -j
	@echo "$(GREEN)✅ Importation of MacroLibX finished!$(NC)"

$(OBJDIR)%.o: $(SRCD)%.c
	@mkdir -p objs/
	@$(CC) $(CFLAGS) -I MacroLibX/includes $< -c -o $@

$(LIBFT):
	@make --no-print-directory -C src/libft
	@echo "$(GREEN)✅ Compilation of libft finished !$(NC)"

clean:
	@make --no-print-directory -C src/libft clean
	@rm -rf $(OBJDIR)
	@echo "$(YELLOW)🧹 file .o cleaned successfully $(NC)"

fclean: clean
	@make --no-print-directory -C src/libft fclean
	@rm -f $(NAME)
	@rm -rf $(LIB)
	@echo "$(RED)🧨 libft.a and Game of Life deleted and MacroLibX$(NC)"

re:		fclean all


.PHONY: all clean fclean re libft banner