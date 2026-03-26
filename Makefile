NAME			= fractol
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -O3

# MLX42 Paths
MLX_DIR			= ./MLX42
MLX_LIB			= $(MLX_DIR)/build/libmlx42.a
MLX_URL			= https://github.com/codam-coding-college/MLX42.git

# Libft Paths
LIBFT_DIR		= ./libft
LIBFT_LIB		= $(LIBFT_DIR)/libft.a

# Includes
HEADER			= includes/fractol.h
INCLUDES		= -I$(MLX_DIR)/include -I$(LIBFT_DIR)

# Flags for linking
LIBS			= $(MLX_LIB) $(LIBFT_LIB) -lglfw -lm

SRCS			= src/main.c src/render.c src/utils.c src/event_hooks.c src/fractol_sets.c
OBJS			= $(SRCS:.c=.o)

# Bonus Files
HEADER_BONUS	= includes/fractol_bonus.h
SRCS_BONUS		= src/main_bonus.c src/render_bonus.c src/utils_bonus.c src/event_hooks_bonus.c src/fractol_sets_bonus.c
OBJS_BONUS		= $(SRCS_BONUS:.c=.o)

all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "Cloning MLX42..."; \
		git clone $(MLX_URL) $(MLX_DIR); \
	fi	
	@cmake $(MLX_DIR) -B $(MLX_DIR)/build && make -C $(MLX_DIR)/build -j4

# Mandatory executable
$(NAME): $(LIBFT_LIB) $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

bonus: .bonus

# Bonus executable
.bonus: $(LIBFT_LIB) $(MLX_LIB) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBS) -o $(NAME)
	@touch .bonus

# Mandatory object comp
$(OBJS): %.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Bonus object comp
$(OBJS_BONUS): %.o: %.c $(HEADER_BONUS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_BONUS)
	@rm -rf $(MLX_DIR)/build
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus
