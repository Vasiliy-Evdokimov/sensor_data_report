@echo off
gcc -c -o localization.o localization.c
gcc -c -o structs.o structs.c

gcc -c -o utils.o utils.c
gcc -c -o interface.o interface.c
gcc -c -o read_file.o read_file.c
gcc -c -o arguments.o arguments.c
gcc -c -o sensors.o sensors.c
gcc -c -o report.o report.c

gcc -c -o main.o main.c
gcc -o app main.o localization.o structs.o utils.o interface.o read_file.o arguments.o sensors.o report.o
del /q *.o
app.exe -f temperature_small.csv -y 2021
pause
