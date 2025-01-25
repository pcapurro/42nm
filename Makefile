# # #==============# # #
# # ### MAKEFILE ### # #
# # #==============# # #

## === VARIABLES === ##

NAME = ft_nm

SRCS = srcs/main.c \
	srcs/help.c \
	srcs/heart/paths.c \
	srcs/heart/options.c \
	srcs/heart/read.c \
	srcs/heart/symbols/64.c \
	srcs/heart/symbols/32.c \
	srcs/heart/symbols/address.c \
	srcs/heart/symbols/type.c \
	srcs/heart/symbols/name.c \
	srcs/heart/symbols/symbols.c \
	srcs/tools/tools.c \
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
