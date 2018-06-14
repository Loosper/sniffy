objects = SQLConnection.o Connection.o helpers.o
CXXFLAGS = -std=c++11

MYSQL_INCLUDE_PATH = /usr/include/cppconn
MYSQL_LINK_PATH = /usr/lib
CPPFLAGS = -I $(MYSQL_INCLUDE_PATH) -L $(MYSQL_LINK_PATH)

.PHONY: clean

sniffy: $(objects)
	g++ -o sniffy $(objects) main.cpp -lmysqlcppconn

$(objects): helpers.h
Connection.o: Connection.h

clean:
	rm -f $(objects) sniffy

build:
	g++ -o builder.out $(objects) builder.cpp -lmysqlcppconn
	./builder.out
