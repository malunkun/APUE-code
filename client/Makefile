src = $(wildcard ./*.c)
obj = $(patsubst %.c,%.o,$(src))
target = client
CC = gcc
$(target):$(obj)
	@$(CC) $(obj) -o $(target) -Wall
%.o:%.c
	@$(CC) -c $< -o $@ 
clean:
	@rm -rf *.o
