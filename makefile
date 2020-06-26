# C++ Compiler
CXX := g++
# Compiler|Linker|Library|Include Flags
CXXFLAGS += -c -O3 -DNDEBUG -Wall -std=c++11
LDFLAGS +=
LIBFLAGS :=
INCLUDEFLAGS :=

# Program name
EXE := czbwt

# Directories for: Headers|Sources|Objects|Binary
HEADERDIR := src
SRCDIR := src
OBJDIR := build
BINDIR := bin
#
#
################################################################################

HEADERS := $(wildcard $(HEADERDIR)/*.h)
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.cpp=.o))

CXXFLAGS += $(INCLUDEFLAGS) -I$(HEADERDIR)
LDFLAGS += $(LIBFLAGS)

.PHONY: all clean

all: $(BINDIR)/$(EXE)

$(BINDIR)/$(EXE): $(OBJS)
	mkdir -p $(BINDIR)
	$(CXX) $(LDFLAGS) $? -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(EXE)
