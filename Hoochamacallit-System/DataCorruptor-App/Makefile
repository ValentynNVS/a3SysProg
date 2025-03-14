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
./bin/DataCorruptor : ./obj/DataCorruptor.o 
	cc ./obj/DataCorruptor.o -o ./bin/Common

# =======================================================
#                     Dependencies
# =======================================================
./obj/DataCorruptor.o : ./src/DX.c ./inc/DX.h
	cc -c ./src/DX.c -o ./obj/DataCorruptor.o 

# =======================================================
# Other targets
# =======================================================
all : ./bin/Common

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o