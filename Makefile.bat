@echo off
gcc -c -o localization.o localization.c
gcc -c -o utils.o utils.c
gcc -c -o functions.o functions.c
gcc -c -o main.o main.c
gcc -o app main.o localization.o utils.o functions.o
del /q *.o
app.exe -f temperature_big.csv -y 2021
pause
