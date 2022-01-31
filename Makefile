all: app

localization.o: localization.c
	gcc -c -o localization.o localization.c

temp_api.o: temp_api.c
	gcc -c -o temp_api.o temp_api.c

main.o: main.c
	gcc -c -o main.o main.c
	
app: main.o localization.o temp_api.o
	gcc -o app main.o localization.o temp_api.o
	
clean:
	del /q *.o
