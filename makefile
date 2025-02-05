OBJS=MCVar.c MenuScreen.c StartMenu.c SignUp.c SignIn.c PreGameMenu.c SettingMenu.c Scoreboard.c CreateMap.c GameScreen.c InGame.c Main.c

all: exec

run: exec
	@./exec
rtest: test
	@./test
test: test.c
	@gcc test.c -lSDL2 -lSDL2_mixer -lncursesw -o test 
exec: $(OBJS)
	@gcc $(OBJS) -lSDL2 -lSDL2_mixer -lncursesw -lncurses -o exec
