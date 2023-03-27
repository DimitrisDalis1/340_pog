%{
    #include <stdio.h>
    int yyerror (char* yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

%}


%start program

%union{
    char* stringValue;
    int intValue;
    double doubleValue;
}


%token<stringValue> ID
%token<stringValue> STRING
%token<intValue> INT
%token<doubleValue> REAL
%token<stringValue> IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL TRUE FALSE NIL
%token<stringValue> ASSIGN PLUS MINUS MULT DIV PERC EQUAL NOT_EQUAL PLUS2 MINUS2 BIGGER SMALLER BIGGER_EQUAL SMALLER_EQUAL 
%token<stringValue> LEFTCURLY RIGHTCURLY LEFTBRACE RIGHTBRACE LEFTPAR RIGHTPAR SEMICOLON COMMA COLON COLON2 PERIOD PERIOD2

%left ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc BIGGER SMALLER BIGGER_EQUAL SMALLER_EQUAL
%left PLUS MINUS
%left MULT DIV PERC
%right NOT PLUS2 MINUS2 
%nonassoc UMINUS
%left PERIOD PERIOD2
%left LEFTBRACE RIGHTBRACE
%left LEFTPAR RIGHTPAR

%type stmt
%type ifstmt
%type whilestmt
%type forstmt
%type returnstmt
%type block
%type funcdef
%type expr
%type assignexpr
%type term
%type lvalue
%type primary
%type objectdef
%type const
%type member
%type call
%type callsuffix
%type normcall
%type methodcall
%type elist
%type indexed
%type indexedelem
%type idlist




%%

program: stmt program
        {

            printf("mphka");
        }
	|  
        {
            printf("mphka");
        }
        ;

stmt:	expr SEMICOLON
        {

            printf("mphka");
        }
        |   ifstmt
		{
			printf("IF\n");
		}
        |   whilestmt
        {

            printf("mphka");
        }
        |   forstmt
        {

            printf("mphka");
        }
        |   returnstmt
        {

            printf("mphka");
        }
        |   BREAK SEMICOLON
        {

            printf("mphka");
        }
        |  CONTINUE SEMICOLON
        {

            printf("mphka");
        }
        |   block
        {

            printf("mphka");
        }
        |   funcdef
        {

            printf("mphka");
        }
        |   SEMICOLON
        {

            printf("mphka");
        }
	;

expr:    assignexpr
        {

            printf("mphka");
        }
        |   expr PLUS expr
        {

            printf("mphka");
        }
        |   expr MINUS expr
        {

            printf("mphka");
        }
        |   expr MULT expr
        {

            printf("mphka");
        }
        |   expr DIV expr
        {

            printf("mphka");
        }
        |   expr PERC expr
        {

            printf("mphka");
        }
        |   expr EQUAL expr
        {

            printf("mphka");
        }
        |   expr NOT_EQUAL expr
        {

            printf("mphka");
        }
        |   expr BIGGER expr
        {

            printf("mphka");
        }
        |   expr BIGGER_EQUAL expr
        {

            printf("mphka");
        }
        |   expr SMALLER expr
        {

            printf("mphka");
        }
        |   expr SMALLER_EQUAL expr
        {

            printf("mphka");
        }
        |   expr AND expr
        {

            printf("mphka");
        }
        |   expr OR expr
        {

            printf("mphka");
        }
        |   term
        {

            printf("mphka");
        }
        ;

term: LEFTPAR expr RIGHTPAR 
        {

            printf("mphka");
        }
        | MINUS expr %prec UMINUS
        {

            printf("mphka");
        }
        | NOT expr
        {

            printf("mphka");
        }
        |   PLUS2 lvalue
        {

            printf("mphka");
        }
        |   lvalue PLUS2
        {

            printf("mphka");
        }
        |   MINUS2 lvalue
        {

            printf("mphka");
        }
        |   lvalue MINUS2
        {

            printf("mphka");
        }
        |   primary
        {
            printf("mphka");

        }
    ;

assignexpr: lvalue ASSIGN expr
            {

            printf("mphka");
            }
            ;

primary: lvalue
            {

            printf("mphka");
            }
            | call
            {

            printf("mphka");
            }
            | objectdef
            {

            printf("mphka");
            }
            | LEFTPAR funcdef RIGHTPAR
            {

            printf("mphka");
            }
            |const
            {

            printf("mphka");
            }
        ;

lvalue: ID
        {

            printf("mphka");
        }
        | LOCAL ID
        {

            printf("mphka");
        }
        | COLON2 ID
        {

            printf("mphka");
        }
        | member
        {

            printf("mphka");
        }
        ;

member: lvalue PERIOD ID
        {

            printf("mphka");
        }
        | lvalue LEFTBRACE expr RIGHTBRACE
        {

            printf("mphka");
        }
        | call PERIOD ID
        {

            printf("mphka");
        }
        | call LEFTBRACE expr RIGHTBRACE
        {

            printf("mphka");
        }
        ;
call:   call LEFTPAR elist RIGHTPAR
        {

            printf("mphka");
        }
        | lvalue callsuffix
        {

            printf("mphka");
        }
        | LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR
        {

            printf("mphka");
        }
        ;
callsuffix: normcall
            {

            printf("mphka");
            }
            | methodcall
            {

            printf("mphka");
            }
            ;
normcall: LEFTPAR elist RIGHTPAR
            {

            printf("mphka");
            }
            ;
methodcall : PERIOD2 ID LEFTPAR elist RIGHTPAR
            {

            }
            
            ;

elist :    expr
            {

            }
            | elist COMMA expr
            {

            }
            |
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
indexed:  indexedelem
            {

            }
            |indexed COMMA indexedelem
            {

            }
            ; 
indexedelem: LEFTCURLY expr COLON expr RIGHTCURLY
            {

            }
            ;
stmt_inf: stmt_inf stmt
        {

        }
        |
        {

        }
        ;
block: LEFTCURLY stmt_inf RIGHTCURLY 
        {

        }
        |
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
idlist: ID
        {

        }
        |idlist COMMA ID
        {

        }
        |
        {
		;
        }
        ;
ifstmt: IF LEFTPAR expr RIGHTPAR stmt ELSE stmt
        {

        }
        | IF LEFTPAR expr RIGHTPAR stmt
        {

        }
        ;
whilestmt: WHILE {} LEFTPAR expr RIGHTPAR {} stmt
            {

            }
            ;
forstmt: FOR
      	{
	}
	 LEFTPAR elist SEMICOLON expr SEMICOLON elist RIGHTPAR 
	{
	}
	stmt
        {

        };
returnstmt: RETURN SEMICOLON
            {

            }   
            |RETURN expr SEMICOLON
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
            fprintf(stderr,"Cannot read file: %s\n",argv[1]);
            return 1;
        }
    }else yyin = stdin;

    yyparse();
    return 0;
}
