target = server
obj = server.c
CC = gcc
$(target):$(obj)
	@$(CC) $(obj) -o $(target) -lpthread -lsqlite3
clean:
	@rm -rf *.log *.db server
