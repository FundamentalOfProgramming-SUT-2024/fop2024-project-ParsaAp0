OBJS=SignUp.c SignIn.c Screen.c StartMenu.c Main.c

all: exec

run: exec
	./exec
test:
	@gcc test.c -lncurses -o test 

exec: $(OBJS)
	@gcc $(OBJS) -lncurses -o exec
