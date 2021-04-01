# Adapted from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
BUILDDIRS := build build/game build/optimal build/players build/utils
DATADIRS := data/optimal
TARGET := bin/quixo

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g3 -Wall -std=c++11 -O3 -march=native -Xpreprocessor -fopenmp -DOPT_COMPUTE_ERROR_CHECKING=$$OPT_COMPUTE_ERROR_CHECKING -DOPT_COMPUTE_SPEED_CHECKING=$$OPT_COMPUTE_SPEED_CHECKING
INC := -I include -I /usr/local/include -I /usr/local/opt/libomp/include

ifeq ($(USER), jc3395)
LDFLAGS := -Xpreprocessor -fopenmp -lomp -march=native -g -O3 -xHost
else
LDFLAGS := -Xpreprocessor -fopenmp -lomp -march=native -g -O3 -framework OpenGL -L /usr/local/lib -lglfw
endif

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

purge:
	@echo "Purging data..."
	@echo "$(RM) -r $(DATADIRS)"; $(RM) -r $(DATADIRS)
	@mkdir -p $(DATADIRS)
