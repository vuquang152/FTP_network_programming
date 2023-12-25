CC = gcc

SERVER_SRC = Server/server.c  \
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

CLIENT_SRC = Client/client.c

SERVER_OBJS = $(SERVER_SRC:.c=.o)
CLIENT_OBJS = $(CLIENT_SRC:.c=.o)

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -I ./include

SERVER_NAME = myftp_server
CLIENT_NAME = myftp_client

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_OBJS)

clean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	$(RM) $(SERVER_NAME) $(CLIENT_NAME)

re: fclean all
