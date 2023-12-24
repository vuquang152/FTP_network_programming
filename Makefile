
CC = gcc

SRC = 	Server/server.c  \
		Server/parser.c  \
		Server/passive_mode.c \
		Server/active_mode.c \
		Server/server_accept_send_receive.c \
		Server/server_functions.c \
		Server/commands/basic_commands.c \
		Server/commands/list_functions.c \
		Server/commands/download_upload.c \
		Server/commands/get_nb_files.c \
		Server/commands/delete_noop.c \
		Server/commands/user_pass.c

OBJS = $(SRC:.c=.o)

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -I ./include
LDFLAGS = -lws2_32


NAME = myftp

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
