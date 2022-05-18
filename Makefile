P=woo
CFLAGS = -g -Wall -O3 -std=c++17 `pkg-config --cflags sfml-all`
LDLIBS= `pkg-config --libs sfml-all`
OBJS=main.o game.o ui.o

$(P): $(OBJS)
	$(CXX) $(CFLAGS) -o $(P) $(OBJS) $(LDLIBS)

clean:
	rm $(P) $(OBJS)

install:
	mkdir -p $(DESTDIR)/usr/share/woo
	cp *.png *.otf $(DESTDIR)/usr/share/woo
	chmod 644 $(DESTDIR)/usr/share/woo/*
	mkdir -p $(DESTDIR)/usr/bin
	install -m 0755 $(P) $(DESTDIR)/usr/bin/$(P)

archive: main.cc game.h game.cc ui.h ui.cc
	zip woo *.cc *.h *.png Makefile

main.o: main.cc ui.cc game.cc game.h ui.h
	$(CXX) $(CFLAGS) -c main.cc -o main.o

game.o: game.cc game.h
	$(CXX) $(CFLAGS) -c game.cc -o game.o

ui.o: ui.cc game.h ui.h
	$(CXX) $(CFLAGS) -c ui.cc -o ui.o
