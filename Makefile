objects = SQLConnection.o Connection.o helpers.o parser.o
sniffy_main = main.o
builder_main = builder.o

MYSQL_INCLUDE_PATH = /usr/include/cppconn
MYSQL_LINK_PATH = /usr/lib

CXXFLAGS = -std=c++11
CFLAGS = -Wall -pedantic -g
CPPFLAGS = -I $(MYSQL_INCLUDE_PATH)
LDLIBS = -L$(MYSQL_LINK_PATH)
LDFLAGS = -lmysqlcppconn

.PHONY: clean

sniffy: $(objects) $(sniffy_main)
	g++ -o sniffy $(objects) $(sniffy_main) $(LDLIBS) $(LDFLAGS)

$(objects) $(sniffy_main) $(builder_main): helpers.h
Connection.o: Connection.h
parser.o: parser.h
SQLConnection.o: SQLConnection.h
main.o: builder.cpp

clean:
	rm -f $(objects) sniffy
	rm -f $(sniffy_main) $(builder_main)

build: $(builder_main)
	g++ -o builder.out $(objects) $(builder_main) -lmysqlcppconn
	./builder.out
