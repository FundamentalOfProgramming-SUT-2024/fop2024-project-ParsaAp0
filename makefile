OBJS=MCVar.c MenuScreen.c StartMenu.c SignUp.c SignIn.c PreGameMenu.c SettingMenu.c CreateMap.c GameScreen.c InGame.c Main.c

all: exec

run: exec
	@./exec
rtest: test
	@./test
test: test.c
	@gcc test.c -lncurses -o test 
exec: $(OBJS)
	@gcc $(OBJS) -lncurses -o exec
