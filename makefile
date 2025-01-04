OBJS=MCVar.c Screen.c StartMenu.c SignUp.c SignIn.c PreGameMenu.c Main.c

all: exec

run: exec
	@./exec
test: test.c
	@gcc test.c -lncurses -o test 

exec: $(OBJS)
	@gcc $(OBJS) -lncurses -o exec
