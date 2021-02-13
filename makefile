# Adapted from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
BUILDDIRS := build build/game build/optimal build/players
TARGET := bin/quixo

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11
LDFLAGS := -framework OpenGL -L /usr/local/lib -lglfw
INC := -I include -I /usr/local/include

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo "Linking..."
	@echo "$(CC) $^ $(LDFLAGS) -o $(TARGET)"; $(CC) $^ $(LDFLAGS) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIRS)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."
	@echo "$(RM) -r $(BINDIR) $(BUILDDIRS) $(TARGET)"; $(RM) -r $(BINDIR) $(BUILDDIRS) $(TARGET)
