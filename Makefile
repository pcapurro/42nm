# # #==============# # #
# # ### MAKEFILE ### # #
# # #==============# # #

## === VARIABLES === ##

NAME = ft_nm

SRCS = srcs/main.c \
	srcs/help.c \
	srcs/heart/paths.c \
	srcs/heart/read.c \
	srcs/heart/symbols.c \
	srcs/tools/library.c \
	srcs/tools/print.c \

CC = gcc

CFLAGS = # -Wall -Wextra -Werror

## == .C TO .O == ##

OBJS = $(SRCS:.c=.o)

## === RULES === ##

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJS)

re: fclean all

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
