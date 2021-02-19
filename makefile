# General Variables
CC := g++
OBJDIR=build
OBJS := $(addprefix $(OBJDIR)/,canvas.o image.o font.o drawing.o shape.o shader.o shaders.o errorchecking.o)
LIBDIR=lib
LIB_NAME=libmdcl
STD=c++11
SRC_DIRS := src src/canvas src/support_classes src/canvas/renderer
INCLUDE_DIRS := $(addprefix -I,$(SRC_DIRS))
CFLAGS := -Wall -Wextra $(INCLUDE_DIRS)
LINKFLAGS := -lglad -lglfw3
ARCHIVER := ar
ARCHIVER_FLAGS := ru

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
  image.h font.h exceptions.h color.h

drawing.o.dep=drawing.cpp shaders.h color.h

image.o.dep=image.cpp image.h
font.o.dep=font.cpp font.h
shape.o.dep=shape.cpp shape.h
shader.o.dep=shader.cpp shader.h GLCommon.h
shaders.o.dep=shaders.cpp shader.h
errorchecking.o.dep=errorchecking.cpp errorchecking.h


#Paths to search for dependencies
vpath %.cpp $(SRC_DIRS)
vpath %.h $(SRC_DIRS)


static:
	make $(LIBDIR)/$(LIB_NAME).a

dynamic:
	make -f makefile.dynamic


$(LIBDIR)/$(LIB_NAME).a: $(OBJDIR) $(LIBDIR) $(OBJS)
	$(ARCHIVER) $(ARCHIVER_FLAGS) $@ $(OBJDIR)/*.o

#Archiving the objects into lib[name].a
# $(LIBDIR)/$(LIB_NAME).dylib: $(OBJDIR) $(LIBDIR) $(OBJS)
# 	g++ -dynamiclib -o $@ $(OBJDIR)/*.o $(CFLAGS) $(LINKFLAGS)

compile-test: $(OBJDIR) $(LIBDIR) $(OBJS) $(LIBDIR)/$(LIB_NAME).a
	g++ Examples/example.cpp -o Examples/example -lmdcl -Llib $(INCLUDE_DIRS) -std=c++11 -lglad -lglfw3 -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo

run-test:
	make compile-test && Examples/example

compile-test-force: $(OBJDIR) $(LIBDIR) $(OBJS) $(LIBDIR)/$(LIB_NAME).a
	g++ Examples/example.cpp -o Examples/example -lmdcl -Llib $(INCLUDE_DIRS) -std=c++11 -lglad -lglfw3 -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo

#Force a compilation and linkage with certain flags
force: clean $(OBJS) $(LIBDIR)/$(LIB_NAME).a

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
	$(CC) -c $< -o $@ -std=$(STD) $(CFLAGS)

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