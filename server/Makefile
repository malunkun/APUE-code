src = $(wildcard ./*.c)
obj = $(patsubst %.c,%.o,$(src))
target = server
CC = gcc
$(target):$(obj)
	@$(CC) $(obj) -o $(target) -lpthread -lsqlite3 -Wall
%.o:%.c
	@$(CC) -c $< -o $@ 
clean:
	@rm -rf *.o
