OBJS=SignUp.c Screen.c Main.c

all: exec

run: exec
	./exec

exec: $(OBJS)
	@gcc $(OBJS) -fsanitize=address -lncurses -o exec
