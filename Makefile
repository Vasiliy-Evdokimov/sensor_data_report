all: app

temp_api.o: temp_api.c
	gcc -c -o temp_api.o temp_api.c

main.o: main.c
	gcc -c -o main.o main.c
	
app: main.o temp_api.o
	gcc -o app main.o temp_api.o
	
clean:
	del /q *.o
