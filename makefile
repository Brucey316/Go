all: engine checkScore ai player

#to be depricated in the future 
#engine and check score should not have a main after debugging
engine: engine.c engine.h go.h
	gcc -Wall engine.c -o engine.app

checkScore: checkScore.c checkScore.h go.h 
	gcc -Wall checkScore.c -o checkScore.app

ai: ai.c go.h
	gcc -Wall ai.c -o ai.app

player: player.c go.h
	gcc -Wall player.c -o player.app

clean:
	rm *.app