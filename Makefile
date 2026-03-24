NAME		= fractol
CC			= cc
CFLAGS		= -Wall -Wextra -Werror

# MLX42 Paths
MLX_DIR		= ./MLX42
MLX_LIB		= $(MLX_DIR)/build/libmlx42.a

# Libft Paths
LIBFT_DIR	= ./libft
LIBFT_LIB	= $(LIBFT_DIR)/libft.a

# Includes
INCLUDES	= -I$(MLX_DIR)/include -I$(LIBFT_DIR)

# Flags for linking
LIBS		= $(MLX_LIB) $(LIBFT_LIB) -lglfw -lm

SRCS		= src/main.c src/render.c src/utils.c src/event_hooks.c src/fractol_sets.c src/utils2.c
OBJS		= $(SRCS:.c=.o)

all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)

$(MLX_LIB):
	@cmake $(MLX_DIR) -B $(MLX_DIR)/build && make -C $(MLX_DIR)/build -j4

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@rm -rf $(MLX_DIR)/build
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
