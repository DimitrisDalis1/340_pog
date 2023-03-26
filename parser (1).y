%{
    #include <stdio.h>
    int yyerror (char* yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

%}



%union{
    char* stringValue;
    int intValue;
    double doubleValue;
}

%start program
%token<stringValue> ID
%token<stringValue> STRING
%token<intValue> INT
%token<doubleValue> REAL
%token<stringValue> IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL

%token<stringValue> ASSIGN PLUS MINUS MULT DIV PERC EQUAL NOT_EQUAL PLUS2 MINUS2 BIGGER SMALLER BIGGER_EQUAL SMALLER_EQUAL 

%token<stringValue> LEFTCURLY RIGHTCURLY LEFTBRACE RIGHTBRACE LEFTPAR RIGHTPAR SEMICOLON COMMA COLON COLON2 PERIOD PERIOD2

%type <stringValue> stmt
%type <stringValue> ifstmt
%%

program: stmt

        |   {
            printf("mphka");
        }
        ;

stmt:	ifstmt
		{
			printf("IF\n");
		}
	;

ifstmt:		IF
		{
			printf("IF\n");
		}
		;
%%

int yyerror(char* yaccProvideMessage){
    fprintf(stderr,"%s: at ;ine %d, before token: %s\n",yaccProvideMessage,yylineno,yytext);
    fprintf(stderr,"INPUT NOT VALID\n");
    return 0;
}

int main(int argc,char** argv)
{
    if(argc >1){
        if(!(yyin = fopen(argv[1],"r"))){
            fprintf(stderr,"Cannot red file: %s\n",argv[1]);
            return 1;
        }
    }else yyin = stdin;

    yyparse();
    return 0;
}
