# =======================================================
#                     Dependencies
# =======================================================
./obj/MessageQueue.o : ./src/MessageQueue.c ./inc/MessageQueue.h
	cc -c ./src/MessageQueue.c -o ./obj/MessageQueue.o

# =======================================================
# Other targets
# =======================================================
all: ./obj/MessageQueue.o

clean:
	rm -f ./obj/*.o

