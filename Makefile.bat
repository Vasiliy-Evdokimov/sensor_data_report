@echo off
gcc -c -o localization.o localization.c
gcc -c -o temp_api.o temp_api.c
gcc -c -o main.o main.c
gcc -o app main.o temp_api.o
del /q *.o
app.exe -f temperature_small.csv -y 2020 -m 5 -l 1
pause
