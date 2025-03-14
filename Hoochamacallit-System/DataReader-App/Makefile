#
#   FILE          : Makefile
#   PROJECT       : Hoochamacallit - A3
#   PROGRAMMER    : Valentyn, Juan Jose, Warren, Ahmed
#   FIRST VERSION : 03/09/2025
#   DESCRIPTION   :
#      TTis is the Makefile which puts all the source codes together
#		and compile it into the executable file
#

# FINAL BINARY Target
./bin/DataReader : ./obj/DataReader.o 
	cc ./obj/DataReader.o -o ./bin/Common

# =======================================================
#                     Dependencies
# =======================================================
./obj/DataReader.o : ./src/DR.c ./inc/DR.h
	cc -c ./src/DR.c -o ./obj/DataReader.o 

# =======================================================
# Other targets
# =======================================================
all : ./bin/Common

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o