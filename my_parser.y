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
    bool isFunct = false;
    int function_scope_count = 0;
    int unnamed_counter=0;
    int global_check = 0;
    int sim_loops = 0; //simultaneous loops active
    int sim_funcs = 0;
    
    int b_n_b = 0; //before loop
    int b_a = 0;   //after loop
    int b_n_bf = 0; //before func
    int b_af = 0; //after func
    bool c_f = false;

%}


%start program

%union{
    char* stringValue;
    int intValue;
    double doubleValue;
    struct SymbolTableEntry* exprNode;
    struct id_list* listId;
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
%type<listId> idlist
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

program: 
	stmt program{fprintf(yyout_y,"program -> stmt(asteraki)\n");}  
	| ;

stmt:	
	expr SEMICOLON  { fprintf(yyout_y,"stmt -> expr;\n"); }
	|ifstmt		{ fprintf(yyout_y,"stmt -> ifstmt\n"); }
	|whilestmt	{ fprintf(yyout_y,"stmt -> whilestmt\n"); }
	|forstmt	{ fprintf(yyout_y,"stmt -> forstmt\n"); }
	|returnstmt	{ fprintf(yyout_y,"stmt -> returnstmt\n"); }
	|BREAK SEMICOLON  {
				if(sim_loops == 0){fprintf(stderr, "Error: Break used but not inside of a loop in line %d\n", yylineno);}
				fprintf(yyout_y,"stmt -> break;\n"); }
	|CONTINUE SEMICOLON {
				if(sim_loops == 0){fprintf(stderr, "Error: Continue used but not inside of a loop in line %d\n", yylineno);}
	 			fprintf(yyout_y,"stmt -> continue;\n");
			    }
	|block		{ fprintf(yyout_y,"stmt -> block\n"); }
	|funcdef	{ fprintf(yyout_y,"stmt -> funcdef\n"); }
	|SEMICOLON 	{ fprintf(yyout_y,"stmt -> ;\n"); }
	;

expr:	
	assignexpr  { fprintf(yyout_y,"expr -> assignexpr\n"); }
	| expr PLUS expr { fprintf(yyout_y,"expr -> expr + expr\n");}
	| expr MINUS expr { fprintf(yyout_y,"expr -> expr - expr\n"); }
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
	| term  { fprintf(yyout_y,"expr -> term\n"); } 
	;

term: 
	LEFTPAR expr RIGHTPAR   { fprintf(yyout_y,"term -> ( expr )\n"); }
	| MINUS expr { fprintf(yyout_y,"term -> -expr\n"); }
	|NOT expr { fprintf(yyout_y,"term -> not expr\n"); }
	|PLUS2 lvalue
	{ 
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}else if(((SymbolTableEntry*)$2) == NULL)
		{
			return 0;
		}
		fprintf(yyout_y,"term -> ++lvalue\n");
	}
	|lvalue PLUS2
	{ 
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}else if(((SymbolTableEntry*)$2) == NULL){
			return 0;
		}
		fprintf(yyout_y,"term -> lvalue++\n");
	}
	|MINUS2 lvalue
	{
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}else if(((SymbolTableEntry*)$2) == NULL){
			return 0;
		}
		fprintf(yyout_y,"term -> --lvalue\n"); 
	}
	|lvalue MINUS2
	{
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}else if(((SymbolTableEntry*)$2) == NULL){
			return 0;
		}
		fprintf(yyout_y,"term -> lvalue--\n");
	}
	|primary {fprintf(yyout_y,"term -> primary\n"); };
;

assignexpr:
	lvalue ASSIGN expr   
	{ 
		if($1 != NULL && ((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)
		{
       			fprintf(stderr,"Error,value cannot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}
		fprintf(yyout_y,"assignexpr -> lvalue = expr\n");
	}

primary:
	lvalue {fprintf(yyout_y,"primary -> lvalue\n");}
	|call  {fprintf(yyout_y,"primary -> call\n");}
	|objectdef  {fprintf(yyout_y,"primary -> objectdef\n");}
	|LEFTPAR funcdef RIGHTPAR  {fprintf(yyout_y,"primary -> ( funcdef )\n");}
	|const  {fprintf(yyout_y,"primary -> const\n");} 
	;
	
lvalue:
	ID 
	{ 
        	fprintf(yyout_y,"lvalue -> id\n");
		int temp_scope=current_scope;
		//printf("\nID: %s, temp_scope: %d, curr_scope: %d___I am the first print\n", $1, temp_scope, current_scope);
		SymbolTableEntry* entry = lookup_inScope(hash,(char *)$1,current_scope);
		while(entry==NULL&&temp_scope!=0){
			entry=lookup_inScope(hash,(char *)$1,temp_scope);
			temp_scope--;
	
		}

		if(entry==NULL)
		{
			temp_scope=current_scope;
			//printf("ID: %s, temp_scope: %d, curr_scope: %d\n", $1, temp_scope, current_scope);
			while(entry==NULL&&temp_scope!=0){
				entry=lookup_inScope_wA(hash,(char *)$1,temp_scope);
				temp_scope--;
			}
			if(entry==NULL)
			{
				entry=lookup_inScope_wA(hash,(char *)$1,0);
				if(entry==NULL)
				{
					if(current_scope==0)
					{
						entry=SymTable_insert(hash,(char *)$1,yylineno,NULL,current_scope,GLOBAL);
					}else{
						entry=SymTable_insert(hash,(char *)$1,yylineno,NULL,current_scope,LOCALV);
					}
					$$=entry;	
				}
				else
				{
					$$=entry;	
				}

			}else
			{
				printf("is not null\n");
				if(entry->type== USERFUNC ||	entry->type== LIBFUNC)
				{
					fprintf(stderr, "Cannot access local function in line %d and scope %d \n",yylineno,current_scope);
				}else
					fprintf(stderr, "Cannot access local variable in line %d and scope %d \n",yylineno,current_scope);
				$$=NULL;
			}
		}
		else if(entry->type!= USERFUNC && entry->type != FORMAL) //
		{				
			if(lookup_inScope(hash, (char *)$1, current_scope) == NULL){
				fprintf(stderr, "Cannot access %s in line %d\n", $1, yylineno);	
				$$=entry;
			}
		}
		else if(entry->type == FORMAL) 	//an einai formal h dothesa
		{
			if(lookup_inScope(hash, (char *)$1, current_scope) == NULL) //psakse ena panw
			{
				fprintf(stderr, "Cannot access formal %s in line %d\n",$1, yylineno);
			}
		}


    	}
	|LOCAL ID
	{
//edw tha psaksw an exoume sto IDIO SCOPE ksanaorismenh me to idio onoma mia metablhth
		fprintf(yyout_y,"lvalue -> local id\n");
		SymbolTableEntry* entry = lookup_inScope(hash,(char *)$2,current_scope); //htan 1

    		if(entry == NULL)
		{
  		    	entry = lookup_inBucket(hash,(char *)$2,current_scope); //htan 1
			
      			if(entry != NULL && entry->type == LIBFUNC && current_scope != 0) /*htan sketo scope*/
			{
      		    		fprintf(stderr,"Shadow libfunc in line %d and scope %d \n",yylineno,current_scope);
          			$$= NULL;  
       			}else
			{
         			if(current_scope == 0)
				{
					entry=SymTable_insert(hash,(char *)$2,yylineno,NULL,current_scope,GLOBAL);
				}
           			else
				{
					entry=SymTable_insert(hash,(char *)$2,yylineno,NULL,current_scope,LOCALV);

				}
            			$$= entry;
      			}
  		}else
		{
		printf("Error: In line %d variable %s is already defined\n", yylineno, $2);
      		$$ = entry;
		}
	}
	|COLON2 ID
	{ 
		SymbolTableEntry* entry = lookup_inScope(hash,(char *)$2,0);
		if(entry==NULL)
		{
			fprintf(stderr,"Global not found in line %d and scope %d \n",yylineno,current_scope);
			//$$=NULL; //petaei seg
		}else
			$$=entry;
		fprintf(yyout_y,"lvalue -> ::id\n"); 
	}
	|member { fprintf(yyout_y,"lvalue -> member\n"); }
	;
	
member:
	lvalue PERIOD ID
	{
		fprintf(yyout_y,"member -> lvalue.id\n");
		if($1 == NULL)
        	{
			fprintf(stderr,"lvalue not declared in line %d and scope %d \n",yylineno,current_scope);
		}
    		else if(((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)
		{
        		fprintf(stderr,"Cant use function name as an lvalue.id in line %d and scope %d \n",yylineno,current_scope);
    		}
	}
	|lvalue LEFTBRACE expr RIGHTBRACE
	{
		fprintf(yyout_y,"member -> lvalue [ expr ]\n");
		if($1 == NULL)
        	{
			fprintf(stderr,"lvalue not declared in line %d and scope %d \n",yylineno,current_scope);
		}
    		else if(((SymbolTableEntry*)$1)->type == USERFUNC ||((SymbolTableEntry*)$1)->type== LIBFUNC)
		{
        	fprintf(stderr,"Cannot use function name as an lvalue[] in line %d and scope %d \n",yylineno,current_scope);
    		}
	}
	| call PERIOD ID  {fprintf(yyout_y,"member -> call.id\n");}
	| call LEFTBRACE expr RIGHTBRACE {fprintf(yyout_y,"member -> call [ expr ]\n");}
	;
	
call: call LEFTPAR elist RIGHTPAR
   	{
		fprintf(yyout_y,"call -> call ( elist )\n");
	}
	|lvalue callsuffix
	{
		fprintf(yyout_y,"call -> lvalue callsuffix\n");
		if(!$1)
		{
			fprintf(stderr,"Function not declared in line %d and scope %d \n",yylineno,current_scope);
		}
		fprintf(yyout_y,"call -> lvalue callsuffix\n");
	} 
	|LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR  {fprintf(yyout_y,"call -> ( funcdef ) ( elist )\n");}
	;

callsuffix:
	normcall  {fprintf(yyout_y,"callsuffix -> normcall\n");}
	|methodcall  {fprintf(yyout_y,"callsuffix -> methodcall\n");}
	;
	
normcall:
	LEFTPAR elist RIGHTPAR  {fprintf(yyout_y,"normcall -> ( elist )\n");}
	;
	
methodcall:
	PERIOD2 ID LEFTPAR elist RIGHTPAR {fprintf(yyout_y,"methodcall -> ..id ( elist )\n");}
	;
	
elist:
	expr  {fprintf(yyout_y,"elist -> expr\n");}
	|elist COMMA expr  
	| {};
 
const:
 	INT {fprintf(yyout_y,"const -> number\n");}
	| REAL {fprintf(yyout_y,"const -> number\n");}
	| STRING  {fprintf(yyout_y,"const -> string\n");}
	|NIL  {fprintf(yyout_y,"const -> nil\n");}
	|TRUE  {fprintf(yyout_y,"const -> true\n");}
	|FALSE  {fprintf(yyout_y,"const -> false\n");}
	;


objectdef:
 	LEFTBRACE elist RIGHTBRACE {fprintf(yyout_y,"objectdef -> { elist }\n");} 
	|LEFTBRACE indexed RIGHTBRACE {fprintf(yyout_y,"objectdef -> { indexed }\n");} ;

indexed:
	indexedelem {fprintf(yyout_y,"indexed -> indexedelem\n");} 
	|indexed COMMA indexedelem {fprintf(yyout_y,"indexed -> indexed , indexedelem\n");}  ;

indexedelem:
 	LEFTCURLY expr COLON expr RIGHTCURLY {fprintf(yyout_y,"indexedelem -> { expr : expr }\n");} ;

funcdef:
 	FUNCTION  ID LEFTPAR {increase_scope(); isFunct = true; sim_funcs++;} idlist RIGHTPAR /*exei kanei hdh increase to scope se 3 (sto paradeigma mou, ara de douleuei to -1)*/
	{
		//SymbolTableEntry* search;
		SymbolTableEntry* search =lookup_inScope(hash,(char *)$2,0);
		if (search!=NULL)
		{
			if(search->type==LIBFUNC)
			{
				fprintf(stderr,"Userfunc shadows libraryfunn in line %d and scope %d \n",yylineno,current_scope);
			}else if(current_scope-1 == 0)
			{
				fprintf(stderr,"Found symbol with same name in line %d and scope %d \n",yylineno,current_scope);
			}	
		}

		/*
		if(current_scope>1)
		{
			//ena while loop gia na mporesei na dei ola ta prohgoume scopes
			search =lookup_inScope_wA(hash,$2,current_scope-1); 
			if (search !=NULL)
			{
				fprintf(stderr, "Variable %s exists in line %d and scope %d \n", $2,yylineno,current_scope);
			}
			
		}
		*/
		/*check if it doesnt exist on the hash*/
		//printf("1321\n");
		int temp = current_scope - 1;
		search = lookup_inScope(hash,(char *)$2,current_scope-1);
		if (search == NULL)
		{
			//printf("This function %s did not exist so we are free to add it to the hash\n", $2);
			SymTable_insert(hash,(char *)$2,yylineno,(id_list*)$5,temp,USERFUNC);
		}
		else
		{
			if(search->type==USERFUNC || search->type==LIBFUNC){
				fprintf(stderr, "Function redefinition in line %d and scope %d \n",yylineno,current_scope);}
			else{
				fprintf(stderr, "Function %s declared with same name as variable in line %d and scope %d \n",$2,yylineno,current_scope);}
			//printf("RETURNING\n");
			//return 0;
	
		}

		/*if ((search = lookup_inScope(hash,(char *)$2,current_scope-1)) != NULL) 
		{
			if(search->type==USERFUNC || search->type==LIBFUNC)
				fprintf(stderr, "Function redefinition in line %d and scope %d \n",yylineno,current_scope);
			else
				fprintf(stderr, "Function %s declared with same name as variable, in line %d and scope %d \n",$2,yylineno,current_scope);
			return 0;
		}*/
		
		//insertion in the symtable and in the scopelist
		//SymbolTableEntry* entry = SymTable_insert(hash,(char *)$2,yylineno,(id_list*)$5,current_scope-1,USERFUNC);
		

	}
	block{fprintf(yyout_y,"funcdef -> function temp_id ( idlist ) {}\n");}   
	|FUNCTION LEFTPAR {increase_scope();} idlist RIGHTPAR block
	{
		char* my_name= malloc(50*(sizeof(char)));
		sprintf(my_name,"_myfync%d",unnamed_counter++);
		SymbolTableEntry* entry = SymTable_insert(hash,my_name,yylineno,(id_list*)$4,current_scope-1,USERFUNC);
        fprintf(yyout_y,"funcdef -> function ( idlist ) {}\n");
	}   
	;


idlist:
	ID
	{
		//check if it's a gloabal lib function
		if (lookup_inScope(hash,$1,0) != NULL && lookup_inScope(hash,$1,0)->type == LIBFUNC)
		{
			fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
			
		}
		//check if it is in the same scope we are in
		if (lookup_inScope(hash,$1,current_scope)!=NULL)
		{
			fprintf(stderr,"This formal argument already exists in given scope %s  in line %d", $1,current_scope,yylineno);        
			
		}
		
		//insertion in the idlist and saving the idlist

		$$ = create_id_list();
		insert($$,$1);

		//insertion in the symtable/scopelist
		SymTable_insert(hash, $1, yylineno , NULL , current_scope, FORMAL);
		//fprintf(stderr,"Variable inserted %s current scope: %d in line %d \n", $1, current_scope,yylineno);
		fprintf(yyout_y,"idlist -> id\n");
	}
	|idlist COMMA ID 
	{
	//check if it's a gloabal lib function
    if (lookup_inScope(hash,(const char *)$3,0) != NULL && lookup_inScope(hash,(const char*)$3,0)->type == LIBFUNC)
    {
        fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
        
    }
    //check if it is in the same scope we are in
    if (lookup_inScope(hash,(const char*)$3,current_scope)!=NULL)
    {
        fprintf(stderr,"This formal argument already exists in given scope in line %d and scope %d \n",yylineno,current_scope);       
        
    }
    
    //insertion in the idlist and saving the idlist
    insert($1,$3);
    $$ = $1; //not sure if this is functional 

	//insertion in the symtable/scopelist
	SymTable_insert(hash, (const char*)$3, yylineno , NULL , current_scope, FORMAL); /*to 2o orisma htan $1 kai de douleue to print (obviously, afou to ena einai idlist kai to allo string)*/

    fprintf(yyout_y,"idlist -> idlist , id\n");
	}
	| 
	{    
		$$ = create_id_list();
 		fprintf(yyout_y,"idlist ->  \n"); 
	};
temp:
 	temp stmt {fprintf(yyout_y,"temp -> temp stmt\n");}
	| {}; 

block:
	LEFTCURLY
 	{
		//b_a == after loop
		//b_n_b == before loop
		//checkarisma an anoikse block enw den uparxei active loop
		if(sim_loops == 0)
		{
			b_n_b++;
		}
		else
		{
			b_a++;
		}
		
		//same logic for functions
		if(sim_funcs == 0)
		{
			b_n_bf++;
		}
		else
		{
			b_af++;
		}

			
		if(isFunct == true)
		{
			isFunct = false;
		}
		else
		{
			increase_scope();
		}

		//increase_scope();
 	} /*talk about this one, giati ousiastika kanoume increase scope alla ama einai megalo to function kai 3ekinaei kai allo function ta gamaei ola ekei mesa  προσοχή (ειδική περίπτωση): το block της συνάρτησης δεν αυξάνει επιπλέον το scope κατά 
+1 άρα το κεντρικό block της συνάρτησης είναι +1 σε σύγκριση με το scope που περιέχει τη
συνάρτηση*/
    	temp RIGHTCURLY
	{
		fprintf(yyout_y,"block -> { temp }\n");
		decrease_scope(); /*opote care about this one as well*/
		//an loopes ises me blocks tote sigoura apo loopa to block
		if(sim_loops == b_a && sim_loops > 0)
		{
			sim_loops--; b_a--;
		}
		else if(sim_loops < b_a) //An loopes ligoteres apo active blocks meta apo autes tote meiwnw ena block
		{
			b_a--;
		}
		else if(sim_loops == 0) //An den exw energes loopes meiwnw apo tis before loopes
		{
			b_n_b--;	
		}


		if(sim_funcs == b_af && sim_funcs > 0)
		{
			sim_funcs--; b_af--;
		}
		else if(sim_funcs < b_af) //An loopes ligoteres apo active blocks meta apo autes tote meiwnw ena block
		{
			b_af--;
		}
		else if(sim_funcs == 0) //An den exw energes loopes meiwnw apo tis before loopes
		{
			b_n_bf--;	
		}

		
		
	}; 

ifstmt:
 	IF LEFTPAR expr RIGHTPAR stmt ELSE stmt {fprintf(yyout_y,"ifstmt -> if ( expr ) stmt else stmt\n");}
	|IF LEFTPAR expr RIGHTPAR stmt {fprintf(yyout_y,"ifstmt -> if ( expr ) stmt\n");}
	;

whilestmt: 
	WHILE LEFTPAR{sim_loops++;} expr RIGHTPAR stmt {fprintf(yyout_y,"whilestmt -> while ( expr ) stmt\n");};

forstmt:
 	FOR LEFTPAR{sim_loops++;} elist SEMICOLON expr SEMICOLON elist RIGHTPAR stmt {fprintf(yyout_y,"forstmt -> for ( elist ; expr ; elist ) stmt\n");};

returnstmt:
 	RETURN
	{		
		if(sim_funcs == 0){fprintf(stderr, "Error: Return statement not inside of a function in line %d\n", yylineno);}	
	}
	SEMICOLON {fprintf(yyout_y,"returnstmt -> return ;\n");}
	|RETURN
	{
		if(sim_funcs == 0){fprintf(stderr, "Error: Return statement not inside of a function in line %d\n", yylineno);}
	}
	expr SEMICOLON {fprintf(yyout_y,"returnstmt -> return expr ;\n");}
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
}
