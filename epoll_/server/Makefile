src = $(wildcard ./*.c)
obj = $(patsubst %.c,%.o,$(src))
target = server
CC = gcc
$(target):$(obj)
	@$(CC) $(obj) -o $(target) -lsqlite3 -lpthread -Wall
%.o:%.c
	@$(CC) -c $< -o $@ 
clean:
	@rm -rf *.o
