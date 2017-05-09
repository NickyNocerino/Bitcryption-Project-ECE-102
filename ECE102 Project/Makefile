CC = gcc

WARN_FLAGS = -Wall 
SRC_DIR = ./

EXEC = bitcrypt.exe
EXES = bitdecrypt.exe

DEBUG_FLAGS = -g

INC_FLAGS =  
LIB =  

FLAGS = $(DEBUG_FLAGS) $(WARN_FLAGS) $(INC_FLAGS)

OBJSC = \
bitcrypt.o

SRCC = \
bitcrypt.c

OBJSS = \
bitdecrypt.o

SRCS = \
bitdecrypt.c

HEADERS = 

all: $(EXEC) $(EXES) 

$(EXEC): CTAGS $(OBJSC) $(HEADERS)
	$(CC) $(FLAGS) $(OBJSC) -o $(EXEC) $(LIB)

$(EXES): CTAGS $(OBJSS) $(HEADERS)
	$(CC) $(FLAGS) $(OBJSS) -o $(EXES) $(LIB)

CTAGS :
	ctags *.[ch]

%.o:$(SRC_DIR)/%.c 
	$(CC) $(FLAGS) -c $< -o $@ 

clean :
	rm -f tags
	rm -f *.exe
	rm -f *.o
	rm -f *~

