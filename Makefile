
CXXFLAGS = -Wall -g

entropy: main.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm *.o entropy core
