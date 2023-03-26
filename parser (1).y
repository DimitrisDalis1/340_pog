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
%token AND "and"
%token OR "or"
%token BREAK "break"
%token CONTINUE "continue"
%type <stringValue> stmt
%type <stringValue> ifstmt
%%

program: program stmt
        {

        }
        |stmt
        {

        }
        | %empty 
        {
            printf("mphka");
        }
        ;

stmt:	expr SEMICOLON
        {

        }
        |   ifstmt
		{
			printf("IF\n");
		}
        |   whilestmt
        {

        }
        |   forstmt
        {

        }
        |   returnstmt
        {

        }
        |   BREAK SEMICOLON
        {

        }
        |  CONTINUE SEMICOLON
        {

        }
        |   block
        {

        }
        |   funcdef
        {

        }
        |   SEMICOLON
        {

        }
	;

expr:    assignexpr
        {

        }
        |   expr PLUS expr
        {

        }
        |   expr MINUS expr
        {

        }
        |   expr MULT expr
        {

        }
        |   expr DIV expr
        {

        }
        |   expr PERC expr
        {

        }
        |   expr EQUAL expr
        {

        }
        |   expr NOT_EQUAL expr
        {

        }
        |   expr BIGGER expr
        {

        }
        |   expr BIGGER_EQUAL expr
        {

        }
        |   expr SMALLER expr
        {

        }
        |   expr SMALLER_EQUAL expr
        {

        }
        |   expr AND expr
        {

        }
        |   expr OR expr
        {

        }
        |   term
        {

        }
        ;

term: LEFTPAR expr RIGHTPAR 
        {

        }
        | MINUS expr %prec UMINUS;
        {

        }
        | NOT expr
        {

        }
        |   PLUS2 lvalue
        {

        }
        |   lvalue PLUS2
        {

        }
        |   MINUS2 lvalue
        {

        }
        |   lvalue MINUS2
        {

        }
        |   primary
        {

        }
    ;

assignexpr: lvalue ASSIGN expr
            {

            }
            ;

primary: lvalue
            {

            }
            | call
            {

            }
            | objectdef
            {

            }
            | LEFTPAR funcdef RIGHTPAR
            {

            }
            |const
            {

            }
        ;

lvalue: ID
        {

        }
        | LOCAL ID
        {

        }
        | COLON2 ID
        {

        }
        | member
        {

        }
        ;

member: lvalue PERIOD ID
        {

        }
        | lvalue LEFTBRACE expr RIGHTBRACE
        {

        }
        | call PERIOD ID
        {

        }
        | call LEFTBRACE expr RIGHTBRACE
        {

        }
        ;
call:   call LEFTPAR elist RIGHTPAR
        {

        }
        | lvalue callsuffix
        {

        }
        | LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR
        {

        }
        ;
callsuffix: normcall
            {

            }
            | methodcall
            {

            }
            ;
normcall: LEFTPAR elist RIGHTPAR
            {

            }
            ;
methodcall : PERIOD2 ID LEFTPAR elist RIGHTPAR
            {

            }
            ;

elist :    elist COMMA expr
            {

            }
            | expr
            {

            }
            |%empty
            {

            }
            ;

objectdef: LEFTBRACE elist RIGHTBRACE
            {

            }
            | LEFTBRACE indexed RIGHTBRACE
            {

            }
            ;
indexed: indexed COMMA indexedelem
            {

            }
            |indexedelem
            {

            }
            ; 
indexedelem: LEFTCURLY expr COLON2 expr RIGHTCURLY
            {

            }
            ;
block: LEFTCURLY stmt_inf RIGHTCURLY 
        {

        }
        | %empty
        {

        }
        ;
stmt_inf: stmt_inf stmt
        {

        }
        |stmt
        {

        }
        ;
funcdef: FUNCTION  ID  LEFTPAR idlist RIGHTPAR block
        {

        }
        | FUNCTION LEFTPAR idlist RIGHTPAR block
        {

        }
        ;
const : INT | REAL |STRING | NIL |TRUE | FALSE
        {

        }
        ;
idlist: idlist COMMA ID
        {

        }
        |ID
        {

        }
        |%empty
        {

        }
        ;
ifstmt: IF LEFTPAR expr RIGHTPAR stmt ELSE stmt
        {

        }
        | IF LEFTPAR expr RIGHTPAR stmt
        {

        }
        ;
whilestmt: WHILE LEFTPAR expr RIGHTPAR stmt
            {

            }
            ;
forstmt: FOR LEFTPAR elist SEMICOLON expr SEMICOLON elist RIGHTPAR stmt
        {

        };
returnstmt: RETURN expr SEMICOLON
            {

            }   
            |RETURN SEMICOLON
            {

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
