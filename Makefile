#
# Template Makefile with some nice features, for C / C++ code.
# Allows separate object file directory to keep source directory
# from becoming cluttered, and provides makedepend support.
#
# Note - makedepend sometimes provides strange error messages in
# standard headers like assert.h.  This is because makedepend doesn't 
# properly handle preprocessor directives.  The errors can be ignored.
#
# - sdiverdi 9/27/02
#
# Added install option to work with glvnc projects
#
# command		result
# -------		------
# make clean		remove object files and executable
# make depend		determine file dependencies
# make			make necessary files
# make fresh		make clean; make depend; make
#


# project specifics - executable name, source files, object directory
#
APP      =	template
APPDIR   =	./
OBJDIR   =	./obj/
SOURCES  =	main.cpp


# platform / project specific compiler, flags, etc.
#
SHELL    =	/bin/sh
#CC       =	/usr/bin/gcc295
#CCC      =	/usr/bin/g++295
CC       =     /usr/bin/gcc
CCC      =     /usr/bin/g++
CXX      =	$(CCC)
CXXFLAGS =	-g -Wall -pedantic -D_REENTRANT
CCFLAGS  =	$(CXXFLAGS)
CFLAGS   =	$(CCFLAGS)
INCLUDES =	
LIBES    =	-lm -lglut -lGLU -lGL \
		-L/usr/X11R6/lib/ -lXmu -lXi -lXext -lX11 -lXt

AUX      =	

# special include directories to keep makedepend from complaining
# about non-standard libraries
#
MDINCLUDES =	$(INCLUDES) \
		-I /usr/include/g++-3 -I/usr/include/g++ -I/usr/include/g++/i486-suse-linux


###
# shouldn't need to change anything below here for different platforms
###


# create target names...
#
OBJECTS  =	$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OTARGETS =	$(addprefix $(OBJDIR), $(OBJECTS))


# rules for compiling c / cpp files
#
$(OBJDIR)%.o:	%.c
	@mkdir -p $(OBJDIR)
	@echo "$(CC) $(CFLAGS) -c $*.c"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(OBJDIR)/$*.o -c $*.c

%.o:	%.c
	@mkdir -p $(OBJDIR)
	@echo "$(CC) $(CFLAGS) -c $*.c"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(OBJDIR)/$*.o -c $*.c

$(OBJDIR)%.o:	%.cpp
	@mkdir -p $(OBJDIR)
	@echo "$(CXX) $(CXXFLAGS) -c $*.cpp"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OBJDIR)/$*.o -c $*.cpp

%.o:	%.cpp
	@mkdir -p $(OBJDIR)
	@echo "$(CXX) $(CXXFLAGS) -c $*.cpp"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OBJDIR)/$*.o -c $*.cpp


# make commands - all, clean, depend, fresh
#
all:	$(APP)

$(APP):	$(OTARGETS) $(AUX)
	@mkdir -p $(APPDIR)
	$(CXX) $(CXXFLAGS) -o $(APPDIR)$(APP) $(OTARGETS) $(LIBES)
	@chmod u+x $(APPDIR)$(APP)

clean:	
	rm -f $(OBJDIR)*.o
	rm -f Makefile.bak
	rm -f $(APPDIR)$(APP)

depend:	
	makedepend -p$(OBJDIR) -- $(CXXFLAGS) $(MDINCLUDES) -- $(SOURCES)

fresh:	
	make clean
	make depend
	make all

install:	$(APP)
	$(INST) -m 755 $(APP) $(BINDIR)


