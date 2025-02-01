OBJS=MCVar.c MenuScreen.c StartMenu.c SignUp.c SignIn.c PreGameMenu.c SettingMenu.c Scoreboard.c CreateMap.c GameScreen.c InGame.c Main.c

all: exec

run: exec
	@./exec
rtest: test
	@./test
test: test.c
	@gcc test.c -lncursesw -o test 
exec: $(OBJS)
	@gcc $(OBJS) -lncursesw -lncurses -o exec
