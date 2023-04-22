all: go

go: go.c go.h
	gcc go.c -o go.app

clean:
	rm go.app