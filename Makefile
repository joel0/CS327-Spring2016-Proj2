CC=gcc
CXX=g++
SDIR=src
ODIR=obj
EXEDIR=bin
CFLAGS=-g -Wall -Werror -std=gnu11
CXXFLAGS=-g -Wall -Werror -std=gnu++11
LDFLAGS = 
TARNAME=may_joel.assignment-1.10
OFILES=$(ODIR)/main.o $(ODIR)/data_chunk_t.o $(ODIR)/fmt_chunk_t.o $(ODIR)/riff_type_chunk_t.o $(ODIR)/wav.o

all: environment $(OFILES)
	$(CXX) -o $(EXEDIR)/wav $(OFILES) $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -o $@ $^ -c $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) -o $@ $^ -c $(CXXFLAGS)

environment:
	mkdir -p bin obj

clean:
	rm -f $(EXEDIR)/* $(ODIR)/*.o $(TARNAME).tar.gz

tar: clean
	cp -rf . ../$(TARNAME)/
	mv ../$(TARNAME)/ ./$(TARNAME)/
	tar cvfz $(TARNAME).tar.gz $(TARNAME)/
	rm -rf $(TARNAME)
