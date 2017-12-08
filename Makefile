#compiler
COMPILER = gcc
#linker
LINKER =gcc

#options for linker and compiler
FLAGS =-g -ansi -Wall -Wextra -D_DEBUG_ -std=c99 -lpthread -lm

EXE_NAME= main.exe

#can have several ones separated by spaces, only cpp files
SOURCES = main.cpp util.cpp case.cpp personne.cpp


#PATH to extra header used in the project (when using libs not installed in the OS)
INC_PATHS= -I

#extra libs used in the project (for instance -lpthread)
LIBS	= -L


LINKOBJ	= $(SOURCES:.cpp=.o)
SOURCEHEADERS = $(SOURCES:.cpp=.h)


$(EXE_NAME): $(LINKOBJ)
	$(LINKER) $(LINKOBJ) $(INC_PATHS) $(LIBS) -o $(EXE_NAME) $(FLAGS)

%.o: %.c %.h define.h
	$(COMPILER) -g -c $< -o $@ $(INC_PATHS) $(FLAGS)

%.o: %.c define.h
	$(COMPILER) -g -c $< -o $@ $(INC_PATHS) $(FLAGS)

# cleanup
clean:
	rm -f $(LINKOBJ)
