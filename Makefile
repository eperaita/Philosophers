NAME = philo

CC = clang
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -pthread #-g -fsanitize=thread

SRC_DIR = src/
OBJ_DIR = obj/

FILES = main.c\
	  philo.c\
	  utils.c\
	  time.c\
	  routine.c\

SRC = $(addprefix $(SRC_DIR), $(FILES))
OBJ = $(addprefix $(OBJ_DIR), $(FILES:.c=.o))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ): $(SRC)
	@$(CC) $(CFLAGS) -c $^
	@mv $(FILES:.c=.o) $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
