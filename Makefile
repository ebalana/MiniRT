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

SRCS = src/main.c \
# Agrega aquí tus otros archivos fuente

OBJS = $(SRCS:.c=.o)

# Regla principal
all: $(NAME)

# Verificar e inicializar submódulos si es necesario
check-submodules:
    @if [ ! -f $(MLX_DIR)/CMakeLists.txt ]; then \
        echo "$(YELLOW)Inicializando submódulos...$(DEF_COLOR)"; \
        git submodule update --init --recursive; \
    fi

# Compilar MLX42 (depende de check-submodules)
$(MLX_LIB): check-submodules
    @echo "$(CYAN)Compilando MLX42...$(DEF_COLOR)"
    cmake -S $(MLX_DIR) -B $(MLX_DIR)/build
    cmake --build $(MLX_DIR)/build --parallel

# Compilar ultimate_libft
$(LIBFT_LIB):
    @echo "$(CYAN)Compilando ultimate_libft...$(DEF_COLOR)"
    $(MAKE) -C $(LIBFTDIR)

# Compilar archivos objeto
%.o: %.c Makefile $(HEADER)
    @echo "$(GREEN)Compilando $<...$(DEF_COLOR)"
    $(CC) $(CFLAGS) $(MLX_INCLUDE) -Ilib -c $< -o $@

# Compilar el ejecutable principal
$(NAME): $(OBJS) $(LIBFT_LIB) $(MLX_LIB)
    @echo "$(BLUE)Linking $(NAME)...$(DEF_COLOR)"
    $(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(MLX_LIB) $(MLX_FLAGS) -o $(NAME)
    @echo "$(GREEN)$(NAME) compilado exitosamente!$(DEF_COLOR)"

# Limpiar archivos objeto
clean:
    $(RM) $(OBJS)
    $(MAKE) -C $(LIBFTDIR) clean

# Limpiar todo
fclean: clean
    $(RM) $(NAME)
    $(MAKE) -C $(LIBFTDIR) fclean
    $(RM) -rf $(MLX_DIR)/build

# Recompilar todo
re: fclean all

# Reglas que no son archivos
.PHONY: all clean fclean re check-submodules

#-----------------------------------------------------------------------------------#

# # Colors
# DEF_COLOR = \033[0;39m
# YELLOW = \033[0;93m
# CYAN = \033[0;96m
# GREEN = \033[0;92m
# BLUE = \033[0;94m
# RED = \033[0;91m

# NAME = miniRT
# CC = cc
# CFLAGS = -Werror -Wall -Wextra -g -fsanitize=address
# RM = rm -f
# HEADER = include/miniRT.h

# # Configuración de ultimate_libft
# LIBFTDIR = lib/ultimate_libft/
# LIBFT_LIB = $(LIBFTDIR)/ultimate_libft.a

# # Configuración de MLX42
# MLX_DIR = lib/MLX42
# MLX_LIB = $(MLX_DIR)/build/libmlx42.a
# MLX_INCLUDE = -I$(MLX_DIR)/include
# MLX_FLAGS = -ldl -lglfw -pthread -lm

# SRCS = 	src/main.c \

# OBJS = $(SRCS:.c=.o)

# all: $(NAME)

# %.o: %.c Makefile $(HEADER)
# 	$(CC) $(CFLAGS) $(MLX_INCLUDE) -Ilib -c $< -o $@
# 	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"

# $(LIBFT_LIB):
# 	@make -C $(LIBFTDIR)

# $(MLX_LIB):
# 	cmake -S $(MLX_DIR) -B $(MLX_DIR)/build
# 	cmake --build $(MLX_DIR)/build


# $(NAME): $(LIBFT_LIB) $(MLX_LIB) $(OBJS)
# 	@echo "$(GREEN)Compiling minishell!$(DEF_COLOR)"
# 	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(MLX_LIB) $(MLX_FLAGS) -o $(NAME)
# 	@echo "$(GREEN)Minishell compiled!$(DEF_COLOR)"

# clean:
# 	@$(MAKE) clean -C $(LIBFTDIR)
# 	@$(RM) $(OBJS)
# 	@echo "$(RED)Cleaned object files$(DEF_COLOR)"

# fclean: clean
# 	@$(MAKE) fclean -C $(LIBFTDIR)
# 	@$(RM) -rf $(MLX_DIR)/build
# 	@$(RM) $(NAME)
# 	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

# re: fclean all

# .PHONY: all clean fclean re