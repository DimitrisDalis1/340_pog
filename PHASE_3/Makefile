all: flex bison compile

flex:
	flex --outfile=scanner.c scanner.l
bison:
	bison -v -Wno-yacc --defines --output=parser.c parser.y

compile:
	gcc -o calc scanner.c parser.c symtablehash.c expr.c quadhandler.c stack.c

run:
	calc example.alpha

clean:
	rm -rf calc scanner.c lex.yy.c yacc_output.txt parser.c parser.h


 
