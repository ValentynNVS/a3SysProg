#
#   FILE          : Makefile
#   PROJECT       : Hoochamacillit - A3
#   PROGRAMMER    : Valentyn, Juan Jose, Warren, Ahmed
#   FIRST VERSION : 03/07/2025
#   DESCRIPTION   :
#      TTis is the Makefile which puts all the source codes together
#		and compile it into the executable file
#

# FINAL BINARY Target
./bin/DataCreator : ./obj/DataCreator.o 
	cc ./obj/DataCreator.o -o ./bin/DataCreator

# =======================================================
#                     Dependencies
# =======================================================
./obj/DataCreator.o : ./src/DC.c 
	cc -c ./src/DC.c -o ./obj/DataCreator.o 

# =======================================================
# Other targets
# =======================================================
all : ./bin/DataCreator

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o