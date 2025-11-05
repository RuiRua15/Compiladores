# --- Variables ---
# CC = C Compilador
CC = gcc
# CFLAGS = Flags do compilador
CFLAGS = -Wall -g

# objectos que vamos criar
OBJECTS = main.o ast.o parser.o lex.yy.o

# nome do projeto
TARGET = compiler

# --- Rules ---

all: $(TARGET)

# para fazer o target é preciso os objectos
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)


# main <- main, ast e parser
main.o: main.c ast.h parser.h
	$(CC) $(CFLAGS) -c main.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

parser.o: parser.c ast.h
	$(CC) $(CFLAGS) -c parser.c

lex.yy.o: lex.yy.c ast.h parser.h
	$(CC) $(CFLAGS) -c lex.yy.c

# --- How to Generate .c files ---
#parser files <- parser.bison
parser.c parser.h: parser.bison
	bison -d -o parser.c parser.bison

# lex.yy.c <- flex e parser.h
lex.yy.c: scanner.flex parser.h
	flex scanner.flex

# --- The "Clean Up" Rule ---

clean:
	rm -f $(TARGET) $(OBJECTS) parser.c parser.h lex.yy.c
