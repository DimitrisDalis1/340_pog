%{
    #include <stdio.h>
    #include "symtable.h"
    int yyerror (char* yaccProvidedMessage);
    int yylex(void);
    extern Scope_node *head_scope_node;
    extern SymTable_T* hash ;
    extern int current_scope;
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    FILE* yyout_y; 
    int scope=0;
    int block_count = 0;
    int function_scope_count = 0;
    

%}


%start program

%union{
    char* stringValue;
    int intValue;
    double doubleValue;
    struct SymbolTableEntry* exprNode;
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
%type<exprNode> lvalue
%type call
%type elist
%type callsuffix
%type normcall
%type funcdef
%type expr
%type objectdef
%type member
%type block
%type assignexpr
%type temp
%type methodcall
%type term
%type  primary
%type const
%type ifstmt whilestmt forstmt returnstmt


%%

program: stmt program{fprintf(yyout_y,"program -> stmt(asteraki)\n");}  
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
	|expr PLUS expr { fprintf(yyout_y,"expr -> expr + expr\n");}
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
	| MINUS expr { fprintf(yyout_y,"term -> -expr\n"); }
	|NOT expr { fprintf(yyout_y,"term -> not expr\n"); }
		|PLUS2 lvalue { 
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
	fprintf(yyout_y,"term -> ++lvalue\n");
	}
	|lvalue PLUS2 { 
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
	fprintf(yyout_y,"term -> lvalue++\n"); }
	|MINUS2 lvalue {
		 if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
		fprintf(yyout_y,"term -> --lvalue\n"); 
	}
	|lvalue MINUS2 {
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC)){
        fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$2) == NULL){
		return 0;
	}
		fprintf(yyout_y,"term -> lvalue--\n");
	}

assignexpr: lvalue ASSIGN expr   { 
	if($1 != NULL && ((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC){
       fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    }else if($1 == NULL){
		return 0;
	}

	fprintf(yyout_y,"assignexpr -> lvalue = expr\n");
}


primary: lvalue { fprintf(yyout_y,"primary -> lvalue\n"); 
                }
	|call  { fprintf(yyout_y,"primary -> call\n"); }
	|objectdef  { fprintf(yyout_y,"primary -> objectdef\n"); }
	|LEFTPAR funcdef RIGHTPAR  { fprintf(yyout_y,"primary -> ( funcdef )\n"); }
	|const  { fprintf(yyout_y,"primary -> const\n"); } 
	;
	
lvalue:	ID  { 
                fprintf(yyout_y,"lvalue -> id\n");
		int temp_scope=current_scope;
		SymbolTableEntry* entry = lookup_inScope(hash,(char *)$1,current_scope);
		while(entry==NULL&&temp_scope!=0){
			entry=lookup_inScope(hash,(char *)$1,temp_scope);
			temp_scope--;
	
		}
		
		if(entry==NULL){
			temp_scope=current_scope;
			
			while(entry==NULL&&temp_scope!=0){
				entry=lookup_inScope_wA(hash,(char *)$1,temp_scope);
				temp_scope--;
			}
			if(entry==NULL){
				entry=lookup_inScope_wA(hash,(char *)$1,0);
				if(entry==NULL){
				if(current_scope==0){
					entry=SymTable_insert(hash,(char *)$1,yylineno,NULL,current_scope,GLOBAL);
				}else{
					entry=SymTable_insert(hash,(char *)$1,yylineno,NULL,current_scope,LOCALV);
				}
				$$=entry;
					
				}
			else{
				$$=entry;
				
			}

		}else{
			if(entry->type== USERFUNC ||	entry->type== LIBFUNC){
				printf("cant access local function ");
			}else printf("cant access local variable ");
			$$=NULL;


		}
		}else {	
			$$=entry;

		}

                 }
	|LOCAL ID  {

		fprintf(yyout_y,"lvalue -> local id\n");
		SymbolTableEntry* entry = lookup_inScope(hash,(char *)$1,current_scope);
    		if(entry == NULL){
  		      entry = lookup_inBucket(hash,(char *)$1,current_scope);

      		  if(entry != NULL && entry->type == LIBFUNC && scope != 0) {
      		      printf("shadow libfunc");
          		  $$= NULL;
          		  
       		 }
        		else {
         	   if(current_scope == 0)

			entry=SymTable_insert(hash,(char *)$2,yylineno,NULL,current_scope,GLOBAL);
           		 else
			entry=SymTable_insert(hash,(char *)$2,yylineno,NULL,current_scope,LOCALV);
            		$$= entry;
           
      			  }
  		  }
   		 else {
      		  $$ = entry;
       
  		  }
	 }
	|COLON2 ID  { 

		SymbolTableEntry* entry = lookup_inScope(hash,(char *)$1,0);
		if(entry==NULL){
			printf("Global not found");
			$$=NULL;
		}else
			$$=entry;
		
		fprintf(yyout_y,"lvalue -> ::id\n"); 

		} /*******************/
	|member  { fprintf(yyout_y,"lvalue -> member\n"); } ;
	
member: lvalue PERIOD ID {
	if(((SymbolTableEntry*)$1)!= NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC))
        {
            fprintf(stderr,"Cannot use function as a value in line %d and scope %d \n",yylineno,current_scope);
    }
    else if(((SymbolTableEntry*)$1) == NULL){
		fprintf(stderr,"value not declared in line %d and scope %d \n",yylineno,current_scope);
    }
		fprintf(yyout_y,"member -> lvalue.id\n");
	}
	| lvalue LEFTBRACE expr RIGHTBRACE  { 
	if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)){
        fprintf(stderr,"Cannot use function as a value in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$1) == NULL){
		fprintf(stderr,"value not declared in line %d and scope %d \n",yylineno,current_scope);
	}
		fprintf(yyout_y,"member -> lvalue [ expr ]\n");
	}
	| call PERIOD ID  {
	if(((SymbolTableEntry*)$1)!= NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)){
        fprintf(stderr,"Cannot use function as a value in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$1) == NULL){
		fprintf(stderr,"value not declared in line %d and scope %d \n",yylineno,current_scope);
	}
		fprintf(yyout_y,"member -> call.id\n");
	}
	| call LEFTBRACE expr RIGHTBRACE {
	if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)){
        fprintf(stderr,"Cannot use function as a value in line %d and scope %d \n",yylineno,current_scope);
    }else if(((SymbolTableEntry*)$1)== NULL){
		fprintf(stderr,"lvalue not declared in line %d and scope %d \n",yylineno,current_scope);
	}
		fprintf(yyout_y,"member -> call [ expr ]\n");
		}
	;
	
call: call LEFTPAR elist RIGHTPAR   {fprintf(yyout_y,"call -> call ( elist )\n");}
	|lvalue callsuffix  {
		fprintf(yyout_y,"call -> lvalue callsuffix\n");
		if(!$1){
			fprintf(stderr,"Function not declared in line %d and scope %d \n",yylineno,cureent_scope);
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
	$$=$1;
	fprintf(yyout_y,"temp_id -> id\n");
	} 
	| { $$=NULL;}
	;

funcdef: FUNCTION temp_id LEFTPAR {} idlist RIGHTPAR {} block {function_scope_count++; fprintf(yyout_y,"funcdef -> function_return temp_id ( idlist ) {}\n");}   ; /*****/

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

block: LEFTCURLY {increase_scope(); }
        temp RIGHTCURLY {
			fprintf(yyout_y,"block -> { temp }\n");
			decrease_scope();
			 }; /*******/

ifstmt: IF LEFTPAR expr RIGHTPAR stmt ELSE stmt {fprintf(yyout_y,"ifstmt -> if ( expr ) stmt else stmt\n");}
	|IF LEFTPAR expr RIGHTPAR stmt {fprintf(yyout_y,"ifstmt -> if ( expr ) stmt\n");}
	;

whilestmt: WHILE LEFTPAR expr RIGHTPAR stmt {fprintf(yyout_y,"whilestmt -> while ( expr ) stmt\n");};

forstmt: FOR LEFTPAR elist SEMICOLON expr SEMICOLON elist RIGHTPAR stmt {fprintf(yyout_y,"forstmt -> for ( elist ; expr ; elist ) stmt\n");};

returnstmt: RETURN SEMICOLON {fprintf(yyout_y,"returnstmt -> return ;\n");}
	| RETURN expr SEMICOLON {fprintf(yyout_y,"returnstmt -> return expr ;\n");}
	;

%%
int main(int argc, char** argv)
{

    yyout_y = fopen("yacc_output.txt", "w");
    hash = SymTable_new();
    //symtable_print(head_scope_node,hash);
    if(argc > 1 ){
        if(!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "cannot read file: %s\n", argv[1]);
            return 1;
        }
    }
    else
        yyin= stdin;
    yyparse();
    symtable_print(head_scope_node,hash);
    return 0;

    
    
    return 0;
}
