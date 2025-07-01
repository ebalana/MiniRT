# Colors
DEF_COLOR = \033[0;39m
YELLOW = \033[0;93m
CYAN = \033[0;96m
GREEN = \033[0;92m
BLUE = \033[0;94m
RED = \033[0;91m

NAME = miniRT
CC = cc
CFLAGS = -Werror -Wall -Wextra -g -fsanitize=address
RM = rm -f
HEADER = include/miniRT.h

# Configuración de ultimate_libft
LIBFTDIR = lib/ultimate_libft/
LIBFT_LIB = $(LIBFTDIR)/ultimate_libft.a

# Configuración de MLX42
MLX_DIR = lib/MLX42
MLX_LIB = $(MLX_DIR)/build/libmlx42.a
MLX_INCLUDE = -I$(MLX_DIR)/include
MLX_FLAGS = -ldl -lglfw -pthread -lm

SRCS =	src/main2.c \
		src/ray.c \
		src/vec3.c


OBJS = $(SRCS:.c=.o)

all: $(NAME)

# Verificar e inicializar submódulos si es necesario
check-submodules:
	@if [ ! -f $(MLX_DIR)/CMakeLists.txt ]; then \
		echo "$(YELLOW)Initializing submodules...$(DEF_COLOR)"; \
		git submodule update --init --recursive; \
	fi

# Compilar MLX42 (depende de check-submodules)
$(MLX_LIB): check-submodules
	@echo "$(CYAN)Compiling: MLX42...$(DEF_COLOR)"
	cmake -S $(MLX_DIR) -B $(MLX_DIR)/build
	cmake --build $(MLX_DIR)/build --parallel

# Compilar ultimate_libft
$(LIBFT_LIB):
	@echo "$(CYAN)Compiling: ultimate_libft...$(DEF_COLOR)"
	$(MAKE) -C $(LIBFTDIR)

# Compilar archivos objeto
%.o: %.c Makefile $(HEADER)
	$(CC) $(CFLAGS) $(MLX_INCLUDE) -Ilib -c $< -o $@
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"

# Compilar el ejecutable principal
$(NAME): check-submodules $(OBJS) $(LIBFT_LIB) $(MLX_LIB)
	@echo "$(CYAN)Linking $(NAME)...$(DEF_COLOR)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(MLX_LIB) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled! $(DEF_COLOR)"

# Limpiar archivos objeto
clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean
	@echo "$(RED)Cleaned object files$(DEF_COLOR)"

# Limpiar todo
fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean
	$(RM) -rf $(MLX_DIR)/build
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

# Recompilar todo
re: fclean all

# Reglas que no son archivos
.PHONY: all clean fclean re check-submodules
