# General Variables
CC=g++
ARCHIVER=ar
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,canvas.o image.o font.o drawing.o)
LIBDIR=lib
LINKED=mdcl.o
LIB_NAME=libmdcl.a
STD=c++11
SRC_DIRS=src src/canvas src/support_classes
INCLUDE_DIRS := $(addprefix -I,$(SRC_DIRS))
CFLAGS=-Wall -Wextra -Wno-unused-command-line-argument $(INCLUDE_DIRS)
ARCHIVER_FLAGS=rcs
LINKFLAGS=-lglfw3 -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo

#Test user input variables
ifeq ($(BUILDTYPE), debug)
CFLAGS += -O0 -g
else 
ifeq ($(BUILDTYPE), release)
CFLAGS += -O3 -DNDEBUG
endif
endif

ifdef VERBOSE
CFLAGS+=-v
endif

ifdef FATALWARN
CFLAGS+=-Werror
endif

#Source file dependencies
canvas.o.dep=canvas.cpp canvas.h\
  image.h font.h exceptions.h

drawing.o.dep=drawing.cpp shaders.h

image.o.dep=image.cpp image.h
font.o.dep=font.cpp font.h


#Paths to search for dependencies
vpath %.cpp $(SRC_DIRS)
vpath %.h $(SRC_DIRS)

#Archiving the objects into lib[name].a
$(LIBDIR)/$(LIB_NAME): $(OBJDIR) $(LIBDIR) $(OBJS)
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $(OBJDIR)/*.o


#Force a compilation and linkage with certain flags
force: clean $(OBJS) $(LIBDIR)/$(LIB_NAME)

release:
	make force "BUILDTYPE=release"

debug:
	make force "BUILDTYPE=debug"

fatalwarn:
	make force "FATALWARN=t"

verbose:
	make force "VERBOSE=t"

#Compiles the source files to objects
#Uses second expansion to determine dependencies from .o.dep variables
.SECONDEXPANSION:
$(OBJDIR)/%.o: $$($$*.o.dep)
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS) $(LINKFLAGS)

*.cpp:
*.h:

#Clean project from objects and binaries
clean-full: $(OBJDIR) $(LIBDIR)
	rm -r $(OBJDIR) $(LIBDIR)

clean: $(OBJDIR) $(LIBDIR)
	rm -f $(OBJDIR)/*
	rm -f $(LIBDIR)/*

#Makes folders if don't exist
$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR)
