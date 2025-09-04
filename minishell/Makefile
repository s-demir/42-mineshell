# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/31 14:59:44 by amsagman          #+#    #+#              #
#    Updated: 2025/08/06 18:22:06 by sedemir          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
CC 			= @cc
CFLAGS 		= -Wall -Wextra -Werror -g -Iinclude -I./lib/libft
LDFLAGS 	= -lreadline
SRCDIR		= ./src
OBJDIR		= ./obj

LIBFT 		= ./lib/libft/libft.a

SRC = \
	$(SRCDIR)/main.c \
	$(SRCDIR)/builtin/builtin.c \
	$(SRCDIR)/builtin/builtin1.c \
	$(SRCDIR)/builtin/builtin2.c \
	$(SRCDIR)/builtin/builtin3.c \
	$(SRCDIR)/builtin/builtin4.c \
	$(SRCDIR)/builtin/builtin5.c \
	$(SRCDIR)/builtin/builtin6.c \
	$(SRCDIR)/env/path_utils.c \
	$(SRCDIR)/env/path_utils2.c \
	$(SRCDIR)/env/path_utils3.c \
	$(SRCDIR)/env/start_shell.c \
	$(SRCDIR)/env/start_utils.c \
	$(SRCDIR)/executor/builtin_exec.c \
	$(SRCDIR)/executor/builtin_exec2.c \
	$(SRCDIR)/executor/builtin_exec3.c \
	$(SRCDIR)/executor/builtin_exec4.c \
	$(SRCDIR)/executor/builtin_exec5.c \
	$(SRCDIR)/executor/cmd_exec_init.c \
	$(SRCDIR)/executor/cmd_exec_init2.c \
	$(SRCDIR)/executor/cmd_exec.c \
	$(SRCDIR)/executor/cmd_exec2.c \
	$(SRCDIR)/executor/cmd_exec3.c \
	$(SRCDIR)/executor/cmd_exec4.c \
	$(SRCDIR)/executor/exec.c \
	$(SRCDIR)/expander/env_handler.c \
	$(SRCDIR)/expander/env_handler3.c \
	$(SRCDIR)/expander/env_mngmnt.c \
	$(SRCDIR)/expander/process_pipeline.c \
	$(SRCDIR)/expander/var_expand.c \
	$(SRCDIR)/expander/var_expand2.c \
	$(SRCDIR)/expander/var_expand3.c \
	$(SRCDIR)/lexer/heredoc_utils.c \
	$(SRCDIR)/lexer/heredoc.c \
	$(SRCDIR)/lexer/input_syntax.c \
	$(SRCDIR)/lexer/input_syntax2.c \
	$(SRCDIR)/parser/parser_utils.c \
	$(SRCDIR)/parser/parser_utils2.c \
	$(SRCDIR)/parser/parser_utils3.c \
	$(SRCDIR)/parser/parser.c \
	$(SRCDIR)/parser/token_utils.c \
	$(SRCDIR)/parser/token.c \
	$(SRCDIR)/utils/signal.c \
	$(SRCDIR)/utils/utils.c \
	$(SRCDIR)/utils/utils2.c \
	$(SRCDIR)/utils/utils3.c \


OBJ 		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	@echo "🔧 Compiling Minishell..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)
	@echo "✅ Minishell compiled successfully."

$(OBJDIR)/%.o: $(SRCDIR)/%.c ./include/minishell.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I./include -I./lib/libft -c $< -o $@

$(LIBFT):
	@make -C ./lib/libft

clean:
	@rm -rf $(OBJDIR)
	@make clean -C ./lib/libft
	@echo "🧹 Object files cleaned."

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C ./lib/libft
	@echo "🧼 Full clean completed."

re: fclean all

.PHONY: all clean fclean re
