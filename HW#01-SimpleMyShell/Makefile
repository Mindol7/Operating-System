CC = g++ -O2
OBJS = config.o main.o Parser.o shell_func.o
TARGET = MyShell

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

config.o: config.cpp MyShell.h
	$(CC) -c config.cpp

Parser.o: Parser.cpp MyShell.h
	$(CC) -c Parser.cpp

shell_func.o: shell_func.cpp MyShell.h
	$(CC) -c shell_func.cpp

main.o: MyShell.h main.cpp
	$(CC) -c main.cpp