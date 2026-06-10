NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
INCLUDES = -I. -Iheaders -Ilibraries -Ilibraries/libft

SRCS = source/main.c
OBJ_DIR = build
OBJS = $(SRCS:source/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: source/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
