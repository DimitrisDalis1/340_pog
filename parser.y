%{
    #include <stdio.h> 
    #include "symtable.h"
    int yyerror (char* yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern SymTable_T *hash;
    extern FILE* yyin;
    FILE* yyout_y; 
    int unnamed_counter=0;
    int current_scope=0;

%}


%start program

%union{
    char* stringValue;
    int intValue;
    double doubleValue;
    struct SymbolTableEntry* exprNode;
    struct id_list* listNode;   
}

%expect 1

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
%type<listNode> idlist
%type<exprNode> lvalue
%type<exprNode> call
%type elist
%type callsuffix
%type normcall
%type funcdef
%type<exprNode> expr
%type indexedelem
%type indexed
%type objectdef
%type<exprNode> member
%type block
%type assignexpr
%type temp
%type methodcall
%type term
%type  primary
%type const
%type ifstmt whilestmt forstmt returnstmt


%%

program: stmt program  
	| ;

stmt:	expr SEMICOLON  { fprintf(yyout_y,"stmt -> expr;\n"); }
	|ifstmt		{ fprintf(yyout_y,"stmt -> ifstmt\n"); }
	|whilestmt	{ fprintf(yyout_y,"stmt -> whilestmt\n"); }
	|forstmt	{ fprintf(yyout_y,"stmt -> forstmt\n"); }
	|returnstmt	{ fprintf(yyout_y,"stmt -> returnstmt\n"); }
	|BREAK SEMICOLON  { fprintf(yyout_y,"stmt -> break;\n"); }
	|CONTINUE SEMICOLON { fprintf(yyout_y,"stmt -> continue;\n"); }
	|block		{ fprintf(yyout_y,"stmt -> block\n"); }
	|funcdef	{ fprintf(yyout_y,"stmt -> funcdef\n"); }
	|SEMICOLON 	{ fprintf(yyout_y,"stmt -> ;\n"); }
	;

expr:	assignexpr  { fprintf(yyout_y,"expr -> assignexpr\n"); }
	|expr PLUS expr { fprintf(yyout_y,"expr -> expr + expr\n"); }
	|expr MINUS expr { fprintf(yyout_y,"expr -> expr - expr\n"); }
	| expr MULT expr { fprintf(yyout_y,"expr -> expr * expr\n"); }
	| expr DIV expr	 { fprintf(yyout_y,"expr -> expr / expr\n"); }
	| expr PERC expr { fprintf(yyout_y,"expr -> expr % expr\n"); }
	| expr BIGGER expr { fprintf(yyout_y,"expr -> expr > expr\n"); }
	| expr BIGGER_EQUAL expr { fprintf(yyout_y,"expr -> expr >= expr\n"); }
	| expr SMALLER expr  { fprintf(yyout_y,"expr -> expr < expr\n"); }
	| expr SMALLER_EQUAL expr { fprintf(yyout_y,"expr -> expr <= expr\n"); }
	| expr EQUAL expr { fprintf(yyout_y,"expr -> expr == expr\n"); }
	| expr NOT_EQUAL expr { fprintf(yyout_y,"expr -> expr != expr\n"); }
	| expr AND expr  { fprintf(yyout_y,"expr -> expr and expr\n"); }		
	| expr OR expr	 { fprintf(yyout_y,"expr -> expr or expr\n"); }
	|term  { fprintf(yyout_y,"expr -> term\n"); } 
	;

term: LEFTPAR expr RIGHTPAR   { fprintf(yyout_y,"term -> ( expr )\n"); }
	|MINUS expr { fprintf(yyout_y,"term -> -expr\n"); }
	|NOT expr { fprintf(yyout_y,"term -> not expr\n"); }
	|PLUS2 lvalue { 
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"error"); // fix this error
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
	fprintf(yyout_y,"term -> ++lvalue\n");
	}
	|lvalue PLUS2 { 
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"error"); // fix this error
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
	fprintf(yyout_y,"term -> lvalue++\n"); }
	|MINUS2 lvalue {
		 if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"error"); // fix this error
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
		fprintf(yyout_y,"term -> --lvalue\n"); 
	}
	|lvalue MINUS2 {
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC)){
         fprintf(stderr,"error"); // fix this error
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
		fprintf(yyout_y,"term -> lvalue--\n");
	}

assignexpr: lvalue ASSIGN expr   { 
	if($1 != NULL && ((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC){
        fprintf(stderr,"error"); // fix this error
    }else if($1 == NULL){
		return 0;
	}

	fprintf(yyout_y,"assignexpr -> lvalue = expr\n");

	 }


primary: lvalue { fprintf(yyout_y,"primary -> lvalue\n"); }
	|call  { fprintf(yyout_y,"primary -> call\n"); }
	|objectdef  { fprintf(yyout_y,"primary -> objectdef\n"); }
	|LEFTPAR funcdef RIGHTPAR  { fprintf(yyout_y,"primary -> ( funcdef )\n"); }
	|const  { fprintf(yyout_y,"primary -> const\n"); } 
	;
	
lvalue:	ID  { 
		fprintf(yyout_y,"lvalue -> id\n");  
		}/****/
	|LOCAL ID  { 
		fprintf(yyout_y,"lvalue -> local id\n"); 
	}/****/
	|COLON2 ID  { 
		fprintf(yyout_y,"lvalue -> ::id\n");
		SymbolTableEntry* symEntry = lookup_inScope(hash,(char*) $2, 0);
		if(!symEntry){
			printf("Global var not found");
			$$=NULL;
		}else $$=symEntry;
	}
	|member  { fprintf(yyout_y,"lvalue -> member\n"); } 
	;
	
member: lvalue PERIOD ID {
	if(((SymbolTableEntry*)$1)!= NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC))
        {fprintf(stderr,"Cannot use function as a value");}
    else if(((SymbolTableEntry*)$1) == NULL){
		fprintf(stderr,"lvalue not declared");
    }else{
		fprintf(yyout_y,"member -> lvalue.id\n");
	};}
	| lvalue LEFTBRACE expr RIGHTBRACE  { 
	if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)){
        fprintf(stderr,"error cannot use function as a value"); // fix this error
    }else if(((SymbolTableEntry*)$1) == NULL){
		fprintf(stderr,"error cannot use nothing as a value");
	}else{
		fprintf(yyout_y,"member -> lvalue [ expr ]\n");
		}
	}
	| call PERIOD ID  {
	if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)){
        fprintf(stderr,"error cannot use function as a value"); // fix this error
    }else if(((SymbolTableEntry*)$1) == NULL){
		fprintf(stderr,"error cannot use nothing as a value");
	}else{
		fprintf(yyout_y,"member -> call.id\n");
	}
	}
	| call LEFTBRACE expr RIGHTBRACE {
	if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)){
        fprintf(stderr,"error cannot use function as a value"); // fix this error
    }else if(((SymbolTableEntry*)$1)== NULL){
		fprintf(stderr,"error cannot use nothing as a value");
	}else{
		fprintf(yyout_y,"member -> call [ expr ]\n");
	}
	}
	;
	
call: call LEFTPAR elist RIGHTPAR   {fprintf(yyout_y,"call -> call ( elist )\n");}
	|lvalue callsuffix  {
		fprintf(yyout_y,"call -> lvalue callsuffix\n");
		if(!$1){
			printf("Function not declared");
		}
	}
	|LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR  {fprintf(yyout_y,"call -> ( funcdef ) ( elist )\n");}
	 ;

callsuffix: normcall  {fprintf(yyout_y,"callsuffix -> normcall\n");}
	| methodcall  {fprintf(yyout_y,"callsuffix -> methodcall\n");}
	;
	
normcall: LEFTPAR elist RIGHTPAR  {fprintf(yyout_y,"normcall -> ( elist )\n");}
	;
	
methodcall: PERIOD2 ID LEFTPAR elist RIGHTPAR {fprintf(yyout_y,"methodcall -> ..id ( elist )\n");}
	;
	
elist: expr  {fprintf(yyout_y,"elist -> expr\n");}
	|elist COMMA expr  
	| {};
 
const: INT {fprintf(yyout_y,"const -> number\n");}
	| REAL {fprintf(yyout_y,"const -> number\n");}
	| STRING  {fprintf(yyout_y,"const -> string\n");}
	|NIL  {fprintf(yyout_y,"const -> nil\n");}
	|TRUE  {fprintf(yyout_y,"const -> true\n");}
	|FALSE  {fprintf(yyout_y,"const -> false\n");}
	;


objectdef: LEFTBRACE elist RIGHTBRACE {fprintf(yyout_y,"objectdef -> { elist }\n");} 
	|LEFTBRACE indexed RIGHTBRACE {fprintf(yyout_y,"objectdef -> { indexed }\n");} ;

indexed: indexedelem {fprintf(yyout_y,"indexed -> indexedelem\n");} 
	|indexed COMMA indexedelem {fprintf(yyout_y,"indexed -> indexed , indexedelem\n");}  ;

indexedelem: LEFTCURLY expr COLON expr RIGHTCURLY {fprintf(yyout_y,"indexedelem -> { expr : expr }\n");} ;

temp_id: ID {
	$$=strdup($1);
	fprintf(yyout_y,"temp_id -> id\n");
	} /*****/
	| { $$=NULL;}
	;
funcdef: FUNCTION {increase_scope();} temp_id LEFTPAR idlist RIGHTPAR {
				if((char *)$3!=NULL){
						fprintf(yyout_y,"funcdef -> function id ( idlist ) block\n");
						SymbolTableEntry* func_name = lookup_inScope(&hash,(char *)$3,0);
						if (func_name!=NULL){
							if(func_name->type==LIBFUNC){
								printf("Userfunc shadows libfunc");
								return;
							}else if(scope == 1){
								printf("found symbol with same name");
								return;
							}
						}
						if (scope>1){
							//search in previus scope because u are one scope inside 
							//and u need to check the name if given like this 
							//function f(x){
							//     function x(){}
							//}
							func_name = lookup_inScope(&hash,(char *)$3,current_scope-1);
							if (func_name !=NULL)
							{
								printf("Variable already exists");
								return;
						}
		
					//if it already exists in the same scope print error
					if ((func_name = lookup_inScope(&hash,(char *)$3,current_scope)) != NULL)
					{
						if(func_name->type==LIBFUNC||func_name->type==USERFUNC)
							printf("Function redefinition");
						else
							printf("Funtion declared with same name as variable");
						return ;
					}

					SymbolTableEntry* func_name =SymTable_insert(&hash,(char *)$3, (idList *)$5,  current_scope-1, yylineno, USERFUNC);
				}else{
					fprintf(yyout_y,"funcdef -> function ( idlist ) block\n");
					char* my_name= malloc(50*(sizeof(char)));
					sprintf(my_name,"_myfync%d",unnamed_counter++);
					SymbolTableEntry* func_name =SymTable_insert(&hash,my_name, (idList *)$5,  current_scope, yylineno, USERFUNC);
		        }
				}
			block
			   ;
idlist: ID
{
    //check if it's a gloabal lib function
    if (lookup_inScope(hash,(char*)$1,0) != NULL && lookup_inScope(hash,(char*)$1,0)->type == LIBFUNC)
    {
        printf("This formal argument shadows function from libary");
        return 0;
    }
    //check if it is in the same scope we are in
    if (lookup_inScope(hash,(char*)$1,current_scope)!=NULL)
    {
        printf("this formal argument already exists in given scope");        
        return 0;
    }
    
    //insertion in the idlist and saving the idlist

    $$ = create_id_list();
    insert((id_list*)$$,(char*)$1);

    //insertion in the symtable/scopelist
	SymTable_insert(hash, (char*)$1, yylineno , NULL , current_scope, FORMAL);
	fprintf(yyout_y,"idlist -> id\n");

	}
	| idlist COMMA ID {
	//check if it's a gloabal lib function
    if (lookup_inScope(hash,(char*)$1,0) != NULL && lookup_inScope(hash,(char*)$1,0)->type == LIBFUNC)
    {
        printf("This formal argument shadows function from libary");
        return 0;
    }
    //check if it is in the same scope we are in
    if (lookup_inScope(hash,(char*)$1,current_scope)!=NULL)
    {
        printf("this formal argument already exists in given scope");        
        return 0;
    }
    
    //insertion in the idlist and saving the idlist
    insert((id_list*)$1,(char*)$3);
    $$ = $1; //not sure if this is functional 

	//insertion in the symtable/scopelist
	SymTable_insert(hash,(char*)$1, yylineno , NULL , current_scope, FORMAL);

    fprintf(yyout_y,"idlist -> idlist , id\n");
	}
	|  {    $$ = create_id_list();
 			fprintf(yyout_y,"idlist -> ε\n"); 
	};

temp: temp stmt {fprintf(yyout_y,"temp -> temp stmt\n");}
	| {}; 

block: LEFTCURLY{increase_scope();} temp RIGHTCURLY{decrease_scope();} {fprintf(yyout_y,"block -> { temp }\n");}; /*******/

ifstmt: IF LEFTPAR expr RIGHTPAR stmt ELSE stmt {fprintf(yyout_y,"ifstmt -> if ( expr ) stmt else stmt\n");}
	|IF LEFTPAR expr RIGHTPAR stmt {fprintf(yyout_y,"ifstmt -> if ( expr ) stmt\n");}
	;

whilestmt: WHILE LEFTPAR expr RIGHTPAR stmt {fprintf(yyout_y,"whilestmt -> while ( expr ) stmt\n");};

forstmt: FOR LEFTPAR elist SEMICOLON expr SEMICOLON elist RIGHTPAR stmt {fprintf(yyout_y,"forstmt -> for ( elist ; expr ; elist ) stmt\n");};

returnstmt: RETURN SEMICOLON {fprintf(yyout_y,"returnstmt -> return ;\n");}
	| RETURN expr SEMICOLON {fprintf(yyout_y,"returnstmt -> return expr ;\n");}
	;
