CC = g++ -O2
OBJS = config.o main.o parser.o shellFunc.o
TARGET = MyShell

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

config.o: config.cpp MyShell.h
	$(CC) -c config.cpp

Parser.o: parser.cpp MyShell.h
	$(CC) -c Parser.cpp

shell_func.o: shellFunc.cpp MyShell.h
	$(CC) -c shellFunc.cpp

main.o: MyShell.h main.cpp
	$(CC) -c main.cpp