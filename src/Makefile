# --- Variables ---
# CC = C Compilador
CC = gcc
# CFLAGS = Flags do compilador
CFLAGS = -Wall -g -Wno-unused-function

# objectos que vamos criar
OBJECTS = main.o ast.o parser.o lex.yy.o symtab.o ir.o gen_code.o mips_code.o

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

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

parser.o: parser.c ast.h parser.h symtab.h
	$(CC) $(CFLAGS) -c parser.c

ir.o: ir.c ir.h
	$(CC) $(CFLAGS) -c ir.c

gen_code.o: gen_code.c gen_code.h ir.h ast.h
	$(CC) $(CFLAGS) -c gen_code.c
	
mips_code.o: mips_code.c mips_code.h ir.h symtab.h
	$(CC) $(CFLAGS) -c mips_code.c
	
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
