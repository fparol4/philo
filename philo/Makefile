NAME = philo

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
INCLUDES = -Iheaders

SRCS = source/action.c \
	source/context.c \
	source/init.c \
	source/main.c \
	source/monitor.c \
	source/philo.c \
	source/philo_action.c \
	source/simulation.c \
	source/time.c \
	source/util.c

OBJ_DIR = build
OBJS = $(SRCS:source/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -pthread -o $(NAME)

$(OBJ_DIR)/%.o: source/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
