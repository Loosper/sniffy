objects = main.o Connection.o helpers.o

.PHONY: clean

sniffy: $(objects)
	g++ -o sniffy $(objects)

$(objects): helpers.h
Connection.o: Connection.h

clean:
	rm -f $(objects) sniffy

build:
	g++ -o builder builder.cpp -lmysqlcppconn
	./builder
