all: app

localization.o: localization.c
	gcc -c -o localization.o localization.c	

utils.o: utils.c
	gcc -c -o utils.o utils.c	

functions.o: functions.c
	gcc -c -o functions.o functions.c

main.o: main.c
	gcc -c -o main.o main.c
	
app: main.o localization.o utils.o functions.o
	gcc -o app main.o localization.o utils.o functions.o
	
clean:
	del /q *.o
