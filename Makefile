objects = src/SQLConnection.o src/Connection.o src/helpers.o src/parser.o
sniffy_main = src/main.o
builder_main = src/builder.o

MYSQL_INCLUDE_PATH = /usr/include/cppconn
MYSQL_LINK_PATH = /usr/lib
INCLUDE_PATH = include/

CXXFLAGS = -std=c++11
CFLAGS = -Wall -pedantic -g
CPPFLAGS = -I $(MYSQL_INCLUDE_PATH) -I $(INCLUDE_PATH)
LDLIBS = -L$(MYSQL_LINK_PATH)
LDFLAGS = -lmysqlcppconn

.PHONY: clean

sniffy: $(objects) $(sniffy_main)
	g++ -o sniffy $(objects) $(sniffy_main) $(LDLIBS) $(LDFLAGS)

$(objects) $(sniffy_main) $(builder_main): include/helpers.h
Connection.o: include/Connection.h
parser.o: include/parser.h
SQLConnection.o: include/SQLConnection.h
main.o: src/builder.cpp

clean:
	rm -f $(objects) sniffy
	rm -f $(sniffy_main) $(builder_main)

build: $(builder_main)
	g++ -o builder.out $(objects) $(builder_main) -lmysqlcppconn
	./builder.out
