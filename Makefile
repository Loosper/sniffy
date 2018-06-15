objects = SQLConnection.o Connection.o helpers.o
sniffy_main = main.o
builder_main = builder.o

MYSQL_INCLUDE_PATH = /usr/include/cppconn
MYSQL_LINK_PATH = /usr/lib

CXXFLAGS = -std=c++11
CPPFLAGS = -I $(MYSQL_INCLUDE_PATH)
LDLIBS = -L$(MYSQL_LINK_PATH)
LDFLAGS = -lmysqlcppconn

.PHONY: clean

sniffy: $(objects) $(sniffy_main)
	g++ -o sniffy $(objects) $(sniffy_main) $(LDLIBS) $(LDFLAGS)

$(objects) $(sniffy_main) $(builder_main): helpers.h
Connection.o: Connection.h

clean:
	rm -f $(objects) sniffy

build: $(builder_main)
	g++ -o builder.out $(objects) $(builder_main) -lmysqlcppconn
	./builder.out
