# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 00:43:12 by mjusta            #+#    #+#              #
#    Updated: 2025/12/27 02:47:25 by mjusta           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)/include
LIBS = -lreadline -lncurses

# ---- pretty colors ----
RESET := \033[0m
BOLD  := \033[1m
RED   := \033[31m
GRN   := \033[32m
YEL   := \033[33m
BLU   := \033[34m

SRC = \
		$(SRC_DIR)/main.c \
		$(SRC_DIR)/env/env_get_set.c \
		$(SRC_DIR)/env/env_get_set_helpers.c \
		$(SRC_DIR)/env/env_init.c \
		$(SRC_DIR)/env/env_free.c \
		$(SRC_DIR)/env/env_wrappers.c \
		$(SRC_DIR)/env/env_environ.c \
		$(SRC_DIR)/parser/lexer.c \
		$(SRC_DIR)/parser/lexer_utils.c \
		$(SRC_DIR)/parser/tokenizer.c \
		$(SRC_DIR)/parser/parser.c \
		$(SRC_DIR)/parser/parser_utils.c \
		$(SRC_DIR)/parser/parser_redirs.c \
		$(SRC_DIR)/parser/expander.c \
		$(SRC_DIR)/parser/expander_utils.c \
		$(SRC_DIR)/executor/child_exec.c \
		$(SRC_DIR)/executor/executor.c \
		$(SRC_DIR)/executor/builtin_redirs.c \
		$(SRC_DIR)/executor/exec_external.c \
		$(SRC_DIR)/executor/redirections.c \
		$(SRC_DIR)/executor/heredoc_collect.c \
		$(SRC_DIR)/executor/heredoc_utils.c \
		$(SRC_DIR)/executor/pipes.c \
		$(SRC_DIR)/executor/pipeline.c \
		$(SRC_DIR)/executor/pipeline_utils.c \
		$(SRC_DIR)/executor/path_utils.c \
		$(SRC_DIR)/executor/prepare_env_and_path.c \
		$(SRC_DIR)/executor/get_line.c \
		$(SRC_DIR)/builtins/echo.c \
		$(SRC_DIR)/builtins/pwd.c \
		$(SRC_DIR)/builtins/env.c \
		$(SRC_DIR)/builtins/export_print.c \
		$(SRC_DIR)/builtins/cd.c \
		$(SRC_DIR)/builtins/export.c \
		$(SRC_DIR)/builtins/unset.c \
		$(SRC_DIR)/builtins/exit.c \
		$(SRC_DIR)/utils/signals_child.c \
		$(SRC_DIR)/utils/signals_parent.c \
		$(SRC_DIR)/utils/cleanup_sh.c \

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

bonus: $(BONUS)

$(NAME): $(OBJ) $(LIBFT)
	@printf "$(BLU)Link$(RESET)   $(NAME)... "
	@$(CC) $(CFLAGS) $^ $(LIBS) -o $(NAME) && \
		printf "%-23s[$(GRN)OK$(RESET)]\n" || \
		(printf "[$(RED)FAIL$(RESET)]\n"; exit 1)


$(LIBFT):
	@printf "$(BLU)Libft $(RESET) build "
	@$(MAKE) -C $(LIBFT_DIR) -s >/dev/null 2>&1 && \
		printf "%-30s[$(GRN)OK$(RESET)]\n" || \
		(printf "[$(RED)FAIL$(RESET)]\n"; exit 1)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "CC     %-35s " "$<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ && \
		printf "[$(GRN)OK$(RESET)]\n" || \
		(printf "[$(RED)FAIL$(RESET)]\n"; exit 1)

clean:
	@printf "$(YEL)Objects has been deleted.$(RESET)\n"
	@$(RM) $(OBJ)
	@# remove empty build/ dirs
	@test -d $(OBJ_DIR) && find $(OBJ_DIR) -type d -empty -delete || true
	@$(MAKE) -C $(LIBFT_DIR) clean -s >/dev/null 2>&1

fclean: clean
	@printf "$(YEL)Binary has been deleted.$(RESET)\n"
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean -s >/dev/null 2>&1

re: fclean all

valgrind:
	@valgrind --leak-check=full --track-origins=yes --suppressions=readline.supp ./$(NAME)

log:
	@valgrind --leak-check=full --log-file=valgrind.log --track-origins=yes --show-leak-kinds=all --suppressions=readline.supp ./$(NAME) || true

.PHONY: all clean fclean re bonus valgrind log
