%{
    #include <stdio.h>
    #include "symtable.h"
    #define for_each_time(item, list) \
	for(T * item = list->head; item != NULL; item = item->next)
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
    bool isFunct1=false;
    bool call_flag = false;

    int isFunc_loop = 0; //exoume function call mesa se active loopa

    bool called_from_func = false;

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
%type ifprefix whilestmt forstmt returnstmt elseprefix
%type N M forprefix for
%type while whilestart whilecond
%type if
%typeof<funcname>:	char*
%typeof<funcbody>:	unsigned
%typeof<funcprefix,funcdef>:	symbol*
%type funcblockend funcblockstart
%type loopstart loopend loopstmt
%type  stmts


%%

program: 
	stmt program{fprintf(yyout_y,"program -> stmt(asteraki)\n");}  
	| ;

stmt:	
	expr SEMICOLON  { fprintf(yyout_y,"stmt -> expr;\n"); }
	|if		{ fprintf(yyout_y,"stmt -> ifstmt\n"); }
	|whilestmt	{ fprintf(yyout_y,"stmt -> whilestmt\n"); }
	|forstmt	{ fprintf(yyout_y,"stmt -> forstmt\n"); }
	|returnstmt	{ fprintf(yyout_y,"stmt -> returnstmt\n"); }
	|BREAK SEMICOLON  {
				if(isFunc_loop == 1){fprintf(stderr, "Error: Break used inside of function with no active loop inside of it in line %d\n", yylineno);}
				if(sim_loops == 0){fprintf(stderr, "Error: Break used but not inside of a loop in line %d\n", yylineno);}
				make_stmt(&$1);
				$1.breaklist = newlist(nextquad()); emit(jump,NULL,NULL,0,-1,currQuad); //not sure orisma 5
				fprintf(yyout_y,"stmt -> break;\n"); }
	|CONTINUE SEMICOLON {
				if(isFunc_loop == 1){fprintf(stderr, "Error: Continue used inside of function with no active loop inside of it in line %d\n", yylineno);}
				if(sim_loops == 0){fprintf(stderr, "Error: Continue used but not inside of a loop in line %d\n", yylineno);}
				make_stmt(&$1);
				$1.contlist = newlist(nextquad()); emit(jump, NULL, NULL,0, -1, currQuad); //not sure orisma 5
	 			fprintf(yyout_y,"stmt -> continue;\n");
			    }
	|block		{ fprintf(yyout_y,"stmt -> block\n"); }
	|funcdef	{ fprintf(yyout_y,"stmt -> funcdef\n"); }
	|SEMICOLON 	{ fprintf(yyout_y,"stmt -> ;\n"); }
	;

stmts:
	stmt{$$= $1;}
	|stmts stmt
	{
		$$.breaklist = mergelist($1.breaklist, $2.breaklist);
		$$.contlist = mergelist($1.contlist, $2.contlist);

	}

arithop:
	PLUS{$$=add;}
	|MINUS{$$=sub;}
	|MULT{$$=mul;}
	|DIV{$$=div;}
	|PERC{$$=mod;}
	;

//Still stuff to do according to DIale3h 11, diafaneia 5
//Gia ta upolipa leei gia olikh apotimhsh opote to suzhtame (11,6)
expr:	
	assignexpr  { fprintf(yyout_y,"expr -> assignexpr\n"); }
	| expr arithop expr{
		$$=newexpr(arithexpr_e);
		$$->sym=newtemp();
		emit($2, $1, $3, $$, -1,currQuad);
	}
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
	LEFTPAR expr RIGHTPAR   {$$ = $2; fprintf(yyout_y,"term -> ( expr )\n"); }
	| MINUS expr { 
		check_arith($2);
		$$ = newexpr(arithexpr_e);
		$$->sym = istempexpr($2) ? $2->sym : newtemp();
		emit(uminus, $2, NULL, $$, -1, currQuad);
		fprintf(yyout_y,"term -> -expr\n"); 
	}
	|NOT expr { 
		fprintf(yyout_y,"term -> not expr\n"); 
	}
	|PLUS2 lvalue
	{ 
		check_arith($2);
		if($2->type == tableitem_e){
			$$ = emit_iftableitem($2);
			emit(add, $$, newexpr_constnum(1), $$, -1, currQuad); //ENDEXETAI NA EXXEI TYPO TO DEUTERO ORISMA
			emit(tablesetelem, $2, $2->index, $$, -1,currQuad);
		}else{
			emit(add, $2, newexpr_constnum(1), $2,-1,currQuad)
			$$ = newexpr(arithexpr_e);
			$$->sym = newtemp();
			emit(assign, $2, NULL, $$,-1,currQuad);
		}
		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC || ((SymbolTableEntry*)$2)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}
		fprintf(yyout_y,"term -> ++lvalue\n");
	}
	|lvalue PLUS2
	{ 
		check_arith($1);
		$$ = newexpr(var_e);
		$$->sym = newtemp();
		if ($1->type == tableitem_e){
			expr* val = emit_iftableitem($1);
			emit(assign, val, NULL, $$,-1,currQuad);
			emit(add,val,newexpr_constnum(1), val,-1,currQuad);
			emit(tablesetelem, $1, $1->index, val-1,currQuad);
		}
		else {
			emit(assign, $1, NULL, $$,-1,currQuad);
			emit(add, $1, newexpr_constnum(1), $1,-1,currQuad);
		}
		if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}
		fprintf(yyout_y,"term -> lvalue++\n");
	}
	|MINUS2 lvalue
	{
		check_arith($2);
		if($2->type == tableitem_e){
			$$ = emit_iftableitem($2);
			emit(sub, $$, newexpr_constnum(1), $$,-1,currQuad); //ENDEXETAI NA EXXEI TYPO TO DEUTERO ORISMA
			emit(tablesetelem, $2, $2->index, $$,-1,currQuad);
		}else{
			emit(sub, $2, newexpr_constnum(1), $2,-1,currQuad)
			$$ = newexpr(arithexpr_e);
			$$->sym = newtemp();
			emit(assign, $2, NULL, $$,-1,currQuad);
		}

		if(((SymbolTableEntry*)$2) != NULL && (((SymbolTableEntry*)$2)->type == USERFUNC ||((SymbolTableEntry*)$2)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}
		fprintf(yyout_y,"term -> --lvalue\n"); 
	}
	|lvalue MINUS2
	{
		check_arith($1);
		$$ = newexpr(var_e);
		$$->sym = newtemp();
		if ($1->type == tableitem_e){
			expr* val = emit_iftableitem($1);
			emit(assign, val, NULL, $$,-1,currQuad);
			emit(sub,val,newexpr_constnum(1), val,-1,currQuad);
			emit(tablesetelem, $1, $1->index, val,-1,currQuad);
		}
		else {
			emit(assign, $1, NULL, $$,-1,currQuad);
			emit(sub, $1, newexpr_constnum(1), $1,-1,currQuad);
		}

		if(((SymbolTableEntry*)$1) != NULL && (((SymbolTableEntry*)$1)->type == USERFUNC ||((SymbolTableEntry*)$1)->type == LIBFUNC))
		{
        		fprintf(stderr,"Error,value cannnot be a function in line %d and scope %d \n",yylineno,current_scope);
    		}		
		fprintf(yyout_y,"term -> lvalue--\n");
	}
	|primary {$$ = $1;fprintf(yyout_y,"term -> primary\n"); };
;

assignexpr:
	lvalue ASSIGN expr   
	{ 
		if(((SymbolTableEntry*)$1)->type == tableitem_e)
		{
			emit(tablesetelem, $1, $1->index, $3,-1,currQuad);
			$$ = emit_iftableitem($1);
			((SymbolTableEntry*)$$)->type = assignexxpr_e;

		}
		else
		{
			emit(assign, $3, NULL, $1,-1,currQuad);
			$$ = newexpr(assignexpr_e);
			$$->sym=newtemp();
			emit(assign, $1, NULL, $$,-1,currQuad);
		}
		if(call_flag == false){ 
		if( $1 != NULL && ((SymbolTableEntry*)$1)->type == USERFUNC || ((SymbolTableEntry*)$1)->type == LIBFUNC)
		{
			//printf("call flag %d userfunc %d\n", call_flag, ((SymbolTableEntry*)$1)->type );
       			fprintf(stderr,"Error,value cannot be assigned to a function in line %d and scope %d \n",yylineno,current_scope);
    		}}
		fprintf(yyout_y,"assignexpr -> lvalue = expr\n");
	}

primary:
	lvalue {
			fprintf(yyout_y,"primary -> lvalue\n");
			$$ = emit_iftableitem($1);
		}
	|call  {fprintf(yyout_y,"primary -> call\n");}
	|objectdef  {fprintf(yyout_y,"primary -> objectdef\n");}
	|LEFTPAR funcdef RIGHTPAR  {
					$$ = newexpr(programfunc_e);
					$$->sym = $2;
					fprintf(yyout_y,"primary -> ( funcdef )\n");}
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
			while(entry==NULL&&temp_scope!=0){
				entry=lookup_inScope(hash,(char *)$1,temp_scope);
				temp_scope--;
			}
			if(entry==NULL)
			{
				entry=lookup_inScope(hash,(char *)$1,0);
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
				if(entry->type== USERFUNC ||	entry->type== LIBFUNC)
				{
					fprintf(stderr, "Cannot access local function in line %d and scope %d \n",yylineno,current_scope);
				}else
					fprintf(stderr, "Cannot access local variable in line %d and scope %d \n",yylineno,current_scope);
				//$$=NULL;
			}
		}
		else if(entry->type!= USERFUNC && entry->type != FORMAL) //
		{				
			if(lookup_inScope(hash, (char *)$1, current_scope) == NULL && (b_af <= sim_funcs) && b_af > 0){
				fprintf(stderr, "Cannot access %s in line %d\n", $1, yylineno);	
				
			}
			$$=entry;
		}
		else if(entry->type == FORMAL) 	//an einai formal h dothesa
		{
			if(lookup_inScope(hash, (char *)$1, current_scope) == NULL && b_af <= sim_funcs && b_af > 0) //psakse ena panw
			{
				fprintf(stderr, "Cannot access formal %s in line %d\n",$1, yylineno);
			}
			$$=entry;
		}else $$=entry;


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
	|member {
			$$ = $1;
		 	fprintf(yyout_y,"lvalue -> member\n"); }
	;
	
member:
	lvalue PERIOD ID
	{
		fprintf(yyout_y,"member -> lvalue.id\n");
		$$ = member_item($1, $3);
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
		$1 = emit_iftableitem($1);
		$$ = newexpr(tableitem_e);
		$$->sym = $1->sym;
		$$->index = $3;
		if($1 == NULL)
        	{
			fprintf(stderr,"lvalue not declared in line %d and scope %d \n",yylineno,current_scope);
		}
    		else if(((SymbolTableEntry*)$1)->type == USERFUNC ||((SymbolTableEntry*)$1)->type== LIBFUNC)
		{
        	fprintf(stderr,"Cannot use function name as an lvalue[] in line %d and scope %d \n",yylineno,current_scope);
    		}
	}
	| call PERIOD ID  {call_flag = true; fprintf(yyout_y,"member -> call.id\n");}
	| call LEFTBRACE expr RIGHTBRACE {fprintf(yyout_y,"member -> call [ expr ]\n");}
	;
	
call: call LEFTPAR elist RIGHTPAR
   	{
		$$ = make_call($1, $3);
		call_flag = false;
		fprintf(yyout_y,"call -> call ( elist )\n");
	}
	|lvalue callsuffix
	{
		$1 = emit_iftableitem($1); //in case it was a table item too
		if($2.method){
			expr* t = $1;
			$1 = emit_iftableitem(member_item(t, $2.name));
			$2.elist->next = t; //insert as first argument (recersed, so last)
		}
		call_flag = false;
		fprintf(yyout_y,"call -> lvalue callsuffix\n");
		if(!$1)
		{
			fprintf(stderr,"Function not declared in line %d and scope %d \n",yylineno,current_scope);
		}
		fprintf(yyout_y,"call -> lvalue callsuffix\n");
	} 
	|LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR  {
		expr* func = newexpr(programfunc_e);
		func->sym = $2;
		$$ = make_call(func, $5);
		call_flag = false;fprintf(yyout_y,"call -> ( funcdef ) ( elist )\n");}
	;

callsuffix:
	normcall  {
			$$ = $1;
			fprintf(yyout_y,"callsuffix -> normcall\n");}
	|methodcall  {
			$$ = $1;
			fprintf(yyout_y,"callsuffix -> methodcall\n");}
	;
	
normcall:
	LEFTPAR elist RIGHTPAR  {
		$$.elist = $2;
		$$.method = 0;
		$$.name = NULL;
		fprintf(yyout_y,"normcall -> ( elist )\n");}
	;
	
methodcall:
	//exei thema obv
	PERIOD2 ID LEFTPAR elist RIGHTPAR {
		$$.elist = $2;
		$$.method = 1;
		$$.name = $2.val;
		fprintf(yyout_y,"methodcall -> ..id ( elist )\n");}
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
 	LEFTBRACE elist RIGHTBRACE {
					expr* t= newexpr(newtable_e);
					t->sym = newtemp();
					emit(tablecreate, t, NULL, NULL,-1,currQuad);
					for(int i = 0; $2; $2 = $2->next)
						emit(tablesetelem, t, newexpr_constnum(i++), $2,-1,currQuad);
					$$ = t;
					fprintf(yyout_y,"objectdef -> { elist }\n");} 
	|LEFTBRACE indexed RIGHTBRACE {
					expr* t = newexpr(newtable_e);
					t->sym = newtemp();
					emit(tablecreate,t , NULL, NULL,-1,currQuad;
					//Fix this
					while($2!=NULL)
					{
						emit(tablesetelem, t, $2->index, $2,-1,currQuad);
					}
					$$ = t;
						
					fprintf(yyout_y,"objectdef -> { indexed }\n");} ;

indexed:
	indexedelem {fprintf(yyout_y,"indexed -> indexedelem\n");} 
	|indexed COMMA indexedelem {fprintf(yyout_y,"indexed -> indexed , indexedelem\n");}  ;

indexedelem:
 	LEFTCURLY expr COLON expr RIGHTCURLY {fprintf(yyout_y,"indexedelem -> { expr : expr }\n");} ;

funcname:
	ID{
		$$ = $1;
	}
	|
	{
		char* my_name= malloc(50*(sizeof(char)));
		sprintf(my_name,"_myfync%d",unnamed_counter++);
		SymbolTableEntry* entry = SymTable_insert(hash,my_name,yylineno,(id_list*)$4,current_scope-1,USERFUNC);
		$$ = newtempfuncname();
	};

funcprefix:
	FUNCTION funcname
	{
		$$ = SymTable_insert(hash, $2, yylineno, NULL, current_scope, USERFUNC); //Mesa anaferetai sto deutero orisma ws function_s	
		//$funcprefix.iaddress = nextquadlabel(); Ti einai to iaddress, to nextquadlabel einai sth diafaneia 10, diale3h 10
		emit(funcstart, $$, NULL, NULL,nextquadlabel(),currQuad);
		//push(scopeoffserstack, currscopeoffset()); Mia push na ftia3oume gia na kanei save to curr offset
		enterscopespace(); //Entering function argument scope space
		resetformalargsoffset(); //Start formals from zero tbf(10,10)
			
	};

funcargs:
	LEFTPAR {increase_scope();isFunct1 = true; isFunct = true; sim_funcs++;} idlist RIGHTPAR
		{
			enterscopespace(); //entering function locals space
			resetfunctionlocaloffset(); //tbf(10,10), Start counting locals from zero kai prepei na ftiaxtei
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
			}
		};

funcbody:
	funcblockstart block
	{
		fprintf(yyout_y,"funcdef -> function temp_id ( idlist ) {}\n");
		$$ = currscopeoffset(); //Extract total locals
		exitscopespace();	//Exiting function locals space
	}funcblockend;
//gia ta 2 apo epanw
//Epishs for further notice leei mesa oti: scopespacecounter == currentscope mono at 
//formal arguments einai se upshlotero scope kata ena apo oti ta function locals
//Ean de sumbainei auto tote theloume 3exwristh metablhth gia scope

funcdef:
	funcprefix funcargs funcbody
	{
		existscopespace(); //Exiting function definition space
		//$funcprefix.totalLocals = $3; Store locals in symbol entry
		//int offset = pop_and_top(scopeoffsetStack); pop and get pre scope offset
		restorecurrscopeoffset(offset); //restore previous scope offset, tbf(10,10)
		$$ = $1;	//The function definition returns the symbol
		emit(funcend, $$, NULL, NULL,nextquadlabel(),currQuad);	
	};

idlist:
	ID
	{
		//check if it's a gloabal lib function
		if (lookup_inScope(hash,$1,0) != NULL && lookup_inScope(hash,$1,0)->type == LIBFUNC)
		{
			fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
			
		}else if(lookup_inScope(hash,$1,current_scope)!=NULL)
		{
			fprintf(stderr,"This formal argument already exists in given scope %s  in line %d", $1,current_scope,yylineno);        
			
		}else{
		
		//insertion in the idlist and saving the idlist

		$$ = create_id_list();
		insert($$,$1);

		//insertion in the symtable/scopelist
		SymTable_insert(hash, $1, yylineno , NULL , current_scope, FORMAL);
		fprintf(yyout_y,"idlist -> id\n");}
	}
	|idlist COMMA ID 
	{
	//check if it's a gloabal lib function
    if (lookup_inScope(hash,(const char *)$3,0) != NULL && lookup_inScope(hash,(const char*)$3,0)->type == LIBFUNC)
    {
        fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
        
    }else if(lookup_inScope(hash,(const char*)$3,current_scope)!=NULL)
    {
        fprintf(stderr,"This formal argument already exists in given scope in line %d and scope %d \n",yylineno,current_scope);       
        
    }else{
    
    //insertion in the idlist and saving the idlist
    insert($1,$3);
    $$ = $1; //not sure if this is functional 

	//insertion in the symtable/scopelist
	SymTable_insert(hash, (const char*)$3, yylineno , NULL , current_scope, FORMAL); /*to 2o orisma htan $1 kai de douleue to print (obviously, afou to ena einai idlist kai to allo string)*/
	}
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

		if(sim_loops > 0 && isFunct == true) //An exoume active loopa ektos tou function
		{
			isFunc_loop = 1; //Tote exoume funct mesa se loop
				
		}
		else
		{
			//printf("De mphka\n");
			isFunc_loop = 0;
		}
	
		if(isFunct == true) //Auto elegxei an kaleitai apo func to block opote na mh megalwnoume peraiterw to scope
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
		//isFunc_loop
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
			sim_funcs--; b_af--; if(sim_funcs == 0){ isFunct1 = false;}
		}
		else if(sim_funcs < b_af) //An loopes ligoteres apo active blocks meta apo autes tote meiwnw ena block
		{
			b_af--;
		}
		else if(sim_funcs == 0) //An den exw energes loopes meiwnw apo tis before loopes
		{
			b_n_bf--; isFunct1 = false;	
		}
		//Tsekare an exoume allo active func afou kleisei to block
		if(isFunc_loop == 1 && sim_funcs == 0)
		{
			isFunc_loop = 0;
		}


		

		
		
	}; 

ifprefix:
	IF LEFTPAR expr RIGHTPAR{
		emit(if_eq, $3, newexpr_constbool(1),nextquad()+2, currQuad+2, currQuad);
		$$ = nextquad();
		emit(jump, NULL, NULL, 0,-1,currQuad);
	};

if:	
	ifprefix stmt{
		patchlabel($1, nextquad());
	}
	|ifprefix stmt elseprefix stmt{
		patchlabel($1,$3+1);
		patchlabel($3,nextquad());
	};

elseprefix:
	ELSE{
		$$=nextquad();
		emit(jump, NULL,NULL,0,-1,currQuad);
	};

loopstart:
	{++loopcounter;};

loopend:
	{--loopcounter;};

loopstmt:
	loopstart stmt loopend {$$=$2;};


whilestmt:
	while LEFTPAR expr RIGHTPAR loopstmt;

forstmt:
	for LEFTPAR elist SEMICOLON expr SEMICOLON elist RIGHTPAR loopstmt;

funcblockstart:
	{
		push(loopcounterstack,loopcounter); loopcounter=0;
	};

funcblockend:
	{
		loopcounter=pop(loopcounterstack);
	};

whilestart:
	while{
		$$=nextquad();
	};

whilecond:	
	LEFTPAR{sim_loops++;} expr RIGHTPAR
	{
		emit(if_eq, $3,newexpr_constbool(1),nextquad()+2,currQuad+2,currQuad);
		$$ = nextquad();
		emit(jump, NULL,NULL,0,-1,currQuad);
	};

while:
	whilestart whilecond stmt 
	{
		fprintf(yyout_y,"whilestmt -> while ( expr ) stmt\n");
		emit(jump,NULL,NULL,$1,-1,currQuad);
		patchlabel($2, nextquad());
		patchlist($3.breaklist, nextquad());
		pathclist($3.contlist, $1);
	};

N:
	{$$ = nextquad(); emit(jump, NULL,NULL,0,-1,currQuad);};
M:
	{$$=nextquad();};

forprefix:
	for LEFTPAR{sim_loops++;} elist SEMICOLON M expr SEMICOLON
	{
		$$.test = $6;
		$$.enter = nextquad();
		emit(if_eq, $7, newexpr_constbool(1), 0,-1,currQuad);
	}

for:
	forprefix N elist RIGHTPAR N stmt N
	{
		patchlabel($1.enter, $5+1); //true jump
		patchlabel($2, nextquad()); //false jump
		patchlabel($5, $1.test);    //loop jump
		patchlabel($7, $2+1);       //closure jump

		patchlist($6.breaklist, nextquad());
		patchlist(%6.contlist, $2+1);
	}
forstmt:
 	FOR LEFTPAR{sim_loops++;} elist SEMICOLON expr SEMICOLON elist RIGHTPAR stmt {fprintf(yyout_y,"forstmt -> for ( elist ; expr ; elist ) stmt\n");};

returnstmt:
 	RETURN
	{		
		if(sim_funcs == 0){fprintf(stderr, "Error: Return statement not inside of a function in line %d\n", yylineno);}	
	}
	SEMICOLON {emit(RETURN,NULL,NULL,NULL,-1,currQuad);fprintf(yyout_y,"returnstmt -> return ;\n");}
	|RETURN
	{
		if(sim_funcs == 0){fprintf(stderr, "Error: Return statement not inside of a function in line %d\n", yylineno);}
	}
	expr SEMICOLON {emit(RETURN,$3,NULL,NULL,-1,currQuad);fprintf(yyout_y,"returnstmt -> return expr ;\n");}
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
