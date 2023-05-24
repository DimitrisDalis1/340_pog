%{
    #include <stdio.h>
    #include "quadhandler.h"
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
    extern bool isError;
    int function_scope_count = 0;
    int unnamed_counter=0;
    int global_check = 0;
    int sim_loops = 0; //simultaneous loops active
    int sim_funcs = 0;
    int currQuad=0;
    int offset=-1;
    int loopcounter=0;
	int offset_= -1; 
    //extern unsigned functionLocalOffset;
    //extern unsigned formalArgOffset;
    extern int tempcounter;
    extern unsigned formalArgOffset;  
    
    int b_n_b = 0; //before loop
    int b_a = 0;   //after loop
    int b_n_bf = 0; //before func
    int b_af = 0; //after func
    bool c_f = false;
    bool isFunct1=false;
    bool call_flag = false;

    int isFunc_loop = 0; //exoume function call mesa se active loopa

    bool called_from_func = false;
	stack* stack_;
	stack* func_stack;
	stack* loopcounterstack;

%}


%start program

%union{
    unsigned int uns;
    char* stringValue;
    int intValue;
    double doubleValue;
    struct SymbolTableEntry* symbol;
    struct expr* exprNode;
    struct call_t* callNode;
    struct id_list* listId;
    struct stmt_t* stmtNode;
    struct for_t* forNode;

}

%expect 1

%token<stringValue> ID
%token<stringValue> STRING
%token<intValue> INT
%token<doubleValue> REAL
%token<stmtNode> BREAK CONTINUE
%token<stringValue> IF ELSE WHILE FOR FUNCTION RETURN AND NOT OR LOCAL TRUE FALSE NIL
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

%type<exprNode> arithop
%type<exprNode> relop
%type<exprNode> boolexpr
%type<stmtNode> stmt
%type<stmtNode> stmts
%type<exprNode> lvalue
%type<listId> idlist
%type<exprNode> call
%type<exprNode> elist
%type<callNode> callsuffix
%type<callNode> normcall
%type<exprNode> expr
%type<exprNode> objectdef
%type<exprNode> member
%type<stmtNode> block
%type<exprNode> assignexpr
%type<callNode> methodcall
%type<exprNode> term
%type<exprNode> primary
%type<exprNode> const
%type<uns> ifprefix
%type<uns> elseprefix
%type<uns> whilestart
%type<uns> whilecond
%type<uns> M
%type<uns> N
%type<stmtNode> forstmt 
%type<stmtNode>returnstmt
%type<stmtNode>program
%type<forNode>forprefix
%type<stmtNode> if
%type loopstart loopend
%type <stmtNode> loopstmt

%type<exprNode>indexed
%type<exprNode>indexedelem
%type<stmtNode>whilestmt
%type<stringValue>funcname
%type<exprNode>funcprefix
%type<exprNode>funcdef
%type<exprNode> funcargs
%type<intValue> funcblockstart 
%type<intValue> funcblockend
%%

program : stmts
{
	$$ = $1;
	fprintf(yyout_y, "program -> stmt(asteraki)\n");
}
| {};

stmt : expr SEMICOLON
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);

	fprintf(yyout_y, "stmt -> expr;\n");
	emitBoolean($1);
	tempcounter = 0;
}
| if
{
	$$ = $1;
	fprintf(yyout_y, "stmt -> ifstmt\n");
	tempcounter = 0;
}
| whilestmt
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	fprintf(yyout_y, "stmt -> whilestmt\n");
	tempcounter = 0;
}
| forstmt
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	fprintf(yyout_y, "stmt -> forstmt\n");
	tempcounter = 0;
}
| returnstmt
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	fprintf(yyout_y, "stmt -> returnstmt\n");
	tempcounter = 0;
}
| BREAK SEMICOLON
{
	if (check_for_valid_loop_stop(loopcounter) == 0){
		fprintf(stderr,"Break not inside of loop in line %d\n", yylineno);
		isError = true;
	}
	$1 = malloc(sizeof(stmt_t));
	make_stmt($1);
	$1->breaklist = newlist(nextquad());
	emit(jump, NULL, NULL, NULL, 0, currQuad);

	$$ = $1;
	fprintf(yyout_y, "stmt -> break;\n");
	tempcounter = 0;
}
| CONTINUE SEMICOLON
{
	if (check_for_valid_loop_stop(loopcounter) == 0){
		fprintf(stderr,"Continue not inside of loop in line %d\n", yylineno);
		isError = true;
	}
	$1 = malloc(sizeof(stmt_t));
	make_stmt($1);
	$1->contlist = newlist(nextquad());
	emit(jump, NULL, NULL, NULL, 0, yylineno);

	$$ = $1;
	fprintf(yyout_y, "stmt -> continue;\n");
	tempcounter = 0;
}
| block
{
	$$ = $1;
	fprintf(yyout_y, "stmt -> block\n");
	tempcounter = 0;
}
| funcdef
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	fprintf(yyout_y, "stmt -> funcdef\n");
	tempcounter = 0;
}
| SEMICOLON
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	fprintf(yyout_y, "stmt -> ;\n");
	tempcounter = 0;
};
stmts : stmt
{
	$$ = $1;
	fprintf(yyout_y, "stmts -> stmt\n");
}
| stmts stmt
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	$$->breaklist = mergelist($1->breaklist, $2->breaklist);
	$$->contlist = mergelist($1->contlist, $2->contlist);
	$$->returnlist = mergelist($1->returnlist, $2->returnlist);
	fprintf(yyout_y, "stmts -> stmts stmt\n");
};

arithop : expr PLUS expr
{
	check_arith($1, "");
	check_arith($3, "");
	$$ = result_finder($1, $3);
	emit(add, $$, $1, $3, -1, yylineno);
	fprintf(yyout_y, "arithop -> expr + expr\n");
}
| expr MINUS expr
{
	check_arith($1, "");
	check_arith($3, "");
	$$ = result_finder($1, $3);
	emit(sub, $$, $1, $3, -1, yylineno);
}
| expr MULT expr
{
	check_arith($1, "");
	check_arith($3, "");
	$$ = result_finder($1, $3);
	emit(mul, $$, $1, $3, -1, yylineno);
	fprintf(yyout_y, "arithop -> expr - expr\n");
}
| expr DIV expr
{
	check_arith($1, "");
	check_arith($3, "");
	$$ = result_finder($1, $3);
	emit(divi, $$, $1, $3, -1, yylineno);
	fprintf(yyout_y, "arithop -> expr / expr\n");
}
| expr PERC expr
{
	check_arith($1, "");
	check_arith($3, "");
	$$ = result_finder($1, $3);
	emit(mod, $$, $1, $3, -1, yylineno);
	fprintf(yyout_y, "arithop -> expr % expr\n");
};

relop : expr BIGGER expr
{
	fprintf(yyout_y, "expr -> expr > expr\n");
	$$ = newexpr(boolexpr_e);
	$1 = emitBoolean($1);
	$3 = emitBoolean($3);

	$$->truelist = newlist(nextquad());
	$$->falselist = newlist(nextquad() + 1);

	emit(if_greater, NULL, $1, $3, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}

| expr SMALLER expr
{
	fprintf(yyout_y, "expr -> expr < expr\n");
	$$ = newexpr(boolexpr_e);

	$1 = emitBoolean($1);
	$3 = emitBoolean($3);

	$$->truelist = newlist(nextquad());
	$$->falselist = newlist(nextquad() + 1);

	emit(if_less, NULL, $1, $3, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
| expr SMALLER_EQUAL expr
{
	fprintf(yyout_y, "expr -> expr <= expr\n");
	$$ = newexpr(boolexpr_e);
	check_arith($1, "");
	check_arith($3, "");
	$1 = emitBoolean($1);
	$3 = emitBoolean($3);

	$$->truelist = newlist(nextquad());
	$$->falselist = newlist(nextquad() + 1);

	emit(if_lesseq, NULL, $1, $3, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
| expr BIGGER_EQUAL expr
{
	fprintf(yyout_y, "expr -> expr <= expr\n");
	$$ = newexpr(boolexpr_e);

	$1 = emitBoolean($1);
	$3 = emitBoolean($3);

	$$->truelist = newlist(nextquad());
	$$->falselist = newlist(nextquad() + 1);

	emit(if_greatereq, NULL, $1, $3, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}

| expr EQUAL {}
expr
{
	fprintf(yyout_y, "expr -> expr == expr\n");
	$$ = newexpr(boolexpr_e);

	$1 = emitBoolean($1);
	$4 = emitBoolean($4);

	$$->truelist = newlist(nextquad());
	$$->falselist = newlist(nextquad() + 1);

	emit(if_eq, NULL, $1, $4, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
| expr NOT_EQUAL {}
expr
{
	fprintf(yyout_y, "expr -> expr != expr\n");
	$$ = newexpr(boolexpr_e);

	$1 = emitBoolean($1);
	$4 = emitBoolean($4);

	$$->truelist = newlist(nextquad());
	$$->falselist = newlist(nextquad() + 1);

	emit(if_noteq, NULL, $1, $4, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
};

boolexpr : expr AND
{
	if ($1 == NULL)
	{
	}
	if ($1->type == boolexpr_e)
	{
	}
	else
	{

		$1->truelist = newlist(nextquad());
		$1->falselist = newlist(nextquad() + 1);

		emit(if_eq, NULL, $1, newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
}
M expr
{
	fprintf(yyout_y, "expr -> expr and expr\n");

	if ($5 == NULL)
	{
	}
	if ($5->type == boolexpr_e)
	{
	}
	else
	{
		$5->truelist = newlist(nextquad());
		$5->falselist = newlist(nextquad() + 1);
		emit(if_eq, NULL, $5, newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
	$$ = newexpr(boolexpr_e);
	patchlist($1->truelist, $4);
	$$->truelist = $5->truelist;
	$$->falselist = mergelist($1->falselist, $5->falselist);
}
| expr OR
{
	if ($1 == NULL)
	{
	}
	if ($1->type == boolexpr_e)
	{
	}
	else
	{
		$1->truelist = newlist(nextquad());
		$1->falselist = newlist(nextquad() + 1);
		emit(if_eq, NULL, $1, newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
}
M expr
{
	fprintf(yyout_y, "expr -> expr or expr\n");

	if ($5 == NULL)
	{
	}
	if ($5->type == boolexpr_e)
	{
	}
	else
	{
		$5->truelist = newlist(nextquad());
		$5->falselist = newlist(nextquad() + 1);
		emit(if_eq, NULL, $5, newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
	$$ = newexpr(boolexpr_e);
	patchlist($1->falselist, $4);
	$$->truelist = mergelist($1->truelist, $5->truelist);
	$$->falselist = $5->falselist;
	fprintf(yyout_y, "boolexpr -> expr OR\n");
};

expr : assignexpr
{
	fprintf(yyout_y, "expr -> assignexpr\n");
}
| term
{
	fprintf(yyout_y, "expr -> term\n");
}
| arithop
{
	$$ = $1;
	fprintf(yyout_y, "expr -> arithop\n");
}
| relop
{
	$$ = $1;
	fprintf(yyout_y, "expr -> relop\n");
}
| boolexpr
{
	$$ = $1;
	fprintf(yyout_y, "expr -> boolexpr\n");
};

term : LEFTPAR expr RIGHTPAR
{
	$$ = $2;
	fprintf(yyout_y, "term -> ( expr )\n");
}
| MINUS expr
{
	if ($2 == NULL)
	{
		$$ = NULL;
	}
	else if ($2->type == programfunc_e || $2->type == libraryfunc_e)
	{
		fprintf(stderr, "Term cannot be a library or a program function \n"); // check if we should terminate it here
		isError = true;
	}

	check_arith($2, "");

	if (istempexpr($2))
	{
		emit(uminus, $2, $2, NULL, -1, yylineno);
		$$ = $2;
	}
	else
	{
		$$ = newexpr(arithexpr_e);
		$$->sym = newtemp();
		emit(uminus, $$, $2, NULL, -1, yylineno); // _t0 = -x;
	}
	fprintf(yyout_y, "term -> -expr\n");
}
| NOT expr
{
	if ($2 == NULL)
	{
		$$ = NULL;
	}
	else
	{
		if ($2->type == boolexpr_e)
		{
		}
		else
		{
			$2->truelist = newlist(nextquad());
			$2->falselist = newlist(nextquad() + 1);
			emit(if_eq, NULL, $2, newexpr_constbool('t'), 0, yylineno);
			emit(jump, NULL, NULL, NULL, 0, yylineno);

			$$ = newexpr(boolexpr_e);
			$$->sym = $2->sym;
			$$->truelist = $2->falselist;
			$$->falselist = $2->truelist;
		}
	}
	fprintf(yyout_y, "term -> not expr\n");
}
| PLUS2 lvalue
{
	check_arith($2, "");
	if (($2) != NULL && (($2)->sym->type == USERFUNC || ($2)->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	if ($2->type == tableitem_e)
	{
		$$ = emit_iftableitem($2);
		emit(add, $$, $$, newexpr_constint(1), -1, yylineno);
		emit(tablesetelem, $2, $2->index, $$, -1, yylineno);
	}
	else
	{
		expr *num = newexpr_constint(1);
		emit(add, $2, $2, num, -1, yylineno);
		$$ = newexpr(arithexpr_e);
		$$->sym = newtemp();
		emit(assign, $$, $2, NULL, -1, yylineno);
	}

	fprintf(yyout_y, "term -> ++lvalue\n");
}
| lvalue PLUS2
{
	check_arith($1, "");
	if (($1) != NULL && (($1)->sym->type == USERFUNC || ($1)->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	$$ = newexpr(var_e);
	$$->sym = newtemp();
	if ($1->type == tableitem_e)
	{
		expr *val = emit_iftableitem($1);
		emit(assign, $$, val, NULL, -1, yylineno);
		emit(add, val, val, newexpr_constint(1), -1, yylineno);
		emit(tablesetelem, $1, $1->index, val, -1, yylineno);
	}
	else
	{
		emit(assign, $$, $1, NULL, -1, yylineno);
		emit(add, $1, $1, newexpr_constint(1), -1, yylineno);
	}

	fprintf(yyout_y, "term -> lvalue++\n");
}
| MINUS2 lvalue
{
	check_arith($2, "");
	if (($2) != NULL && (($2)->sym->type == USERFUNC || ($2)->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	if ($2->type == tableitem_e)
	{
		$$ = emit_iftableitem($2);
		expr *num = newexpr_constint(1);
		emit(sub, $$, $$, num, -1, yylineno); // ENDEXETAI NA EXXEI TYPO TO DEUTERO ORISMA
		emit(tablesetelem, $2, $2->index, $$, -1, yylineno);
	}
	else
	{
		emit(sub, $2, $2, newexpr_constint(1), -1, yylineno);
		$$ = newexpr(arithexpr_e);
		$$->sym = newtemp();
		emit(assign, $$, $2, NULL, -1, yylineno);
	}
	fprintf(yyout_y, "term -> --lvalue\n");
}
| lvalue MINUS2
{
	check_arith($1, "");
	if (($1) != NULL && (($1)->sym->type == USERFUNC || ($1)->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	$$ = newexpr(var_e);
	$$->sym = newtemp();
	if ($1->type == tableitem_e)
	{
		expr *val = emit_iftableitem($1);
		emit(assign, $$, val, NULL, -1, yylineno);
		emit(sub, val, val, newexpr_constint(1), -1, yylineno);
		emit(tablesetelem, $1, $1->index, val, -1, yylineno);
	}
	else
	{
		emit(assign, $$, $1, NULL, -1, yylineno);
		emit(sub, $1, $1, newexpr_constint(1), -1, yylineno);
	}

	fprintf(yyout_y, "term -> lvalue--\n");
}
| primary
{
	$$ = $1;
	fprintf(yyout_y, "term -> primary\n");
};

assignexpr : lvalue ASSIGN expr
{
	fprintf(yyout_y, "assignexpr -> lvalue = expr\n");
	if (call_flag == false)
	{

		if ($1 != NULL && ($1)->type == USERFUNC || ($1)->type == LIBFUNC)
		{
			fprintf(stderr, "Error,value cannot be assigned to a function in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
	}
	if (($1)->type == tableitem_e)
	{
		emit(tablesetelem, $1, $1->index, $3, -1, yylineno);
		$$ = emit_iftableitem($1);
		($$)->type = assignexpr_e;
	}   
	else
	{
		expr *tmp = $3;
		assert($3);
		if ($3->type == boolexpr_e)
			tmp = emitBoolean($3);
		emit(assign, $1, tmp, NULL, -1, yylineno);
		SymbolTableEntry *temp = newtemp();
		$$ = newexpr(assignexpr_e);
		$$->sym = temp;
		emit(assign, $$, $1, NULL, -1, yylineno);
	}
};

primary : lvalue
{
	fprintf(yyout_y, "primary -> lvalue\n");
	$$ = emit_iftableitem($1);
}
| call
{
	fprintf(yyout_y, "primary -> call\n");
	$$ = $1;
}
| objectdef
{
	fprintf(yyout_y, "primary -> objectdef\n");
	}
| LEFTPAR funcdef RIGHTPAR
{
	$$ = newexpr(programfunc_e);
	$$->sym = $2->sym;
	fprintf(yyout_y, "primary -> ( funcdef )\n");
}
| const
{
	fprintf(yyout_y, "primary -> const\n");
	$$ = $1;
};

lvalue : ID
{
	fprintf(yyout_y, "lvalue -> id\n");
	int temp_scope = current_scope;
	SymbolTableEntry *entry = lookup_inScope(hash, (char *)$1, current_scope);
	while (entry == NULL && temp_scope != 0)
	{
		entry = lookup_inScope(hash, (char *)$1, temp_scope);
		temp_scope--;
	}

	if (entry == NULL)
	{
		temp_scope = current_scope;
		while (entry == NULL && temp_scope != 0)
		{
			entry = lookup_inScope(hash, (char *)$1, temp_scope);
			temp_scope--;
		}
		if (entry == NULL)
		{
			entry = lookup_inScope(hash, (char *)$1, 0);
			if (entry == NULL)
			{

				if (current_scope == 0)
				{
					offset_++;
					entry = SymTable_insert(hash, (char *)$1, yylineno, NULL, current_scope, GLOBAL);
					entry->space = programvar;
					entry->offset = offset_;
				}
				else
				{
					offset_++;
					entry = SymTable_insert(hash, (char *)$1, yylineno, NULL, current_scope, LOCALV);
					entry->space = functionlocal;
					entry->offset = offset_;
				}
				// incurrscopeoffset();
				$$ = lvalue_expr(entry);
			}
			else
			{

				$$ = lvalue_expr(entry);
			}
		}
		else
		{
			if (entry->type == USERFUNC || entry->type == LIBFUNC)
			{
				fprintf(stderr, "Cannot access local function in line %d and scope %d \n", yylineno, current_scope);
				isError = true;
			}
			else
				fprintf(stderr, "Cannot access local variable in line %d and scope %d \n", yylineno, current_scope);
				isError = true;
		}
	}
	else if (entry->type != USERFUNC && entry->type != FORMAL)
	{
		if (lookup_inScope(hash, (char *)$1, current_scope) == NULL && (b_af <= sim_funcs) && b_af > 0)
		{
			fprintf(stderr, "Cannot access %s in line %d\n", $1, yylineno);
			isError = true;
		}

		$$ = lvalue_expr(entry);
	}
	else if (entry->type == FORMAL) // an einai formal h dothesa
	{
		if (lookup_inScope(hash, (char *)$1, current_scope) == NULL && b_af <= sim_funcs && b_af > 0) // psakse ena panw
		{
			fprintf(stderr, "Cannot access formal %s in line %d\n", $1, yylineno);
			isError = true;
		}

		$$ = lvalue_expr(entry);
	}
	else 
	$$ = lvalue_expr(entry);
}
| LOCAL ID
{
	// edw tha psaksw an exoume sto IDIO SCOPE ksanaorismenh me to idio onoma mia metablhth
	fprintf(yyout_y, "lvalue -> local id\n");
	SymbolTableEntry *entry = lookup_inScope(hash, (char *)$2, current_scope);

	if (entry == NULL)
	{
		entry = lookup_inBucket(hash, (char *)$2, current_scope); // htan 1

		if (entry != NULL && entry->type == LIBFUNC && current_scope != 0) /*htan sketo scope*/
		{
			fprintf(stderr, "Shadow libfunc in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
			$$ = NULL;
		}
		else
		{
			if (current_scope == 0)
			{
				offset_++;
				entry = SymTable_insert(hash, (char *)$2, yylineno, NULL, current_scope, GLOBAL);
				entry->space = programvar;
				entry->offset = offset_++;
				//incurrscopeoffset();
			}
			else
			{
				offset_++;
				entry = SymTable_insert(hash, (char *)$2, yylineno, NULL, current_scope, LOCALV);
				entry->space = functionlocal;
				entry->offset = offset_++;
				//incurrscopeoffset();
			}

			$$ = lvalue_expr(entry);
		}
	}
	else
	{
		fprintf(stderr,"Error: In line %d variable %s is already defined\n", yylineno, $2);
		isError = true;
		$$ = lvalue_expr(entry);
	}
	
	$$ = lvalue_expr(entry);
}
| COLON2 ID
{
	SymbolTableEntry *entry = lookup_inScope(hash, (char *)$2, 0);
	if (entry == NULL)
	{
		fprintf(stderr, "Global not found in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	else

		$$ = lvalue_expr(entry);
	fprintf(yyout_y, "lvalue -> ::id\n");
}
| member
{
	$$ = $1;
	fprintf(yyout_y, "lvalue -> member\n");
};

member : lvalue PERIOD ID
{
	if ($1 == NULL)
	{
		fprintf(stderr, "lvalue not declared in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	else if (($1)->sym->type == USERFUNC || ($1)->sym->type == LIBFUNC)
	{
		fprintf(stderr, "Cant use function name as an lvalue.id in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	fprintf(yyout_y, "member -> lvalue.id\n");
	$$ = member_item($1, $3);
	
}
| lvalue LEFTBRACE expr RIGHTBRACE
{
	if ($1 == NULL)
	{
		fprintf(stderr, "lvalue not declared in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	else if (($1)->sym->type == USERFUNC || ($1)->sym->type == LIBFUNC)
	{
		fprintf(stderr, "Cannot use function name as an lvalue[] in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	fprintf(yyout_y, "member -> lvalue [ expr ]\n");
	$1 = emit_iftableitem($1);
	$$ = newexpr(tableitem_e);
	$$->sym = $1->sym;
	$$->index = $3;
	
}
| call PERIOD ID
{
	$$ = member_item($1, $3);
	call_flag = true;
	fprintf(yyout_y, "member -> call.id\n");
}
| call LEFTBRACE expr RIGHTBRACE
{
	$1 = emit_iftableitem($1);
	$$ = newexpr(tableitem_e);
	$$->sym = $1->sym;
	$$->index = $3;

	fprintf(yyout_y, "member -> call [ expr ]\n");
};

call : call LEFTPAR elist RIGHTPAR
{
	$$ = make_call($1, $3);
	call_flag = false;
	fprintf(yyout_y, "call -> call ( elist )\n");
}
| lvalue callsuffix
{

	$1 = emit_iftableitem($1);
	if ($2->method)
	{
		expr *t = $1;
		$1 = emit_iftableitem(member_item(t, $2->name));
		expr *tmp;
		if ($2->elist != NULL)
		{
			tmp = $2->elist;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = NULL;
			t->next = $2->elist;
			$2->elist = t;
		}
	}
	call_flag = false;
	if (!$1)
	{
		fprintf(stderr, "Function not declared in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	$$ = make_call($1, $2->elist);
	fprintf(yyout_y, "call -> lvalue callsuffix\n");
}
| LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR
{
	expr *func = newexpr(programfunc_e);
	func->sym = $2->sym;
	$$ = make_call(func, $5);
	call_flag = false;
	fprintf(yyout_y, "call -> ( funcdef ) ( elist )\n");
};

callsuffix : normcall
{
	$$ = $1;
	fprintf(yyout_y, "callsuffix -> normcall\n");
}
| methodcall
{
	$$ = $1;

	fprintf(yyout_y, "callsuffix -> methodcall\n");
};

normcall : LEFTPAR elist RIGHTPAR
{
	$$ = malloc(sizeof(call_t));
	$$->elist = $2;
	$$->method = 0;
	$$->name = NULL;
	fprintf(yyout_y, "normcall -> ( elist )\n");
};

methodcall :

	PERIOD2 ID LEFTPAR elist RIGHTPAR
{

	$$ = malloc(sizeof(call_t));
	$$->elist = $4;
	$$->method = 1;
	$$->name = $2;
	fprintf(yyout_y, "methodcall -> ..id ( elist )\n");
};

	
elist:
	expr  {
		if($1->type==boolexpr_e){
				$1=emitBoolean($1);
			}

		$$=$1;
		$$->next=NULL;
		fprintf(yyout_y,"elist -> expr\n");}
	|elist COMMA expr  { 
		expr* tmp;
			if($1!=NULL && $3!=NULL){
			tmp=$1;
			while(tmp->next){
				tmp=tmp->next;
				}
			if($3->type==boolexpr_e){
				$3=emitBoolean($3);
			}
			tmp->next=$3;
			$3->next=NULL;
			$$=$1;
			
			}
            fprintf(yyout_y,"elist -> elist , expr\n");
		}
	| {$$=NULL;fprintf(yyout_y,"elist -> NULL\n");};
const : INT
{
	fprintf(yyout_y, "const -> INT\n");
	$$ = newexpr_constint($1);
}
| REAL
{
	fprintf(yyout_y, "const -> REAL\n");
	$$ = newexpr_constdouble($1);
}
| STRING
{
	fprintf(yyout_y, "const -> STRING\n");
	$$ = newexpr_conststring($1);
}
| NIL
{
	fprintf(yyout_y, "const -> nil\n");
	$$ = newexpr_constnil();
}
| TRUE
{
	fprintf(yyout_y, "const -> TRUE\n");
	$$ = newexpr_constbool('t');
}
| FALSE
{
	fprintf(yyout_y, "const -> FALSE\n");
	$$ = newexpr_constbool('f');
};

objectdef : LEFTBRACE elist RIGHTBRACE
{
	expr *t = newexpr(newtable_e);
	t->sym = newtemp();
	emit(tablecreate, t, NULL, NULL, -1, yylineno);
	for (int i = 0; $2; $2 = $2->next)
		emit(tablesetelem, t, newexpr_constint(i++), $2, -1, yylineno);
	$$ = t;
	fprintf(yyout_y, "objectdef -> { elist }\n");
}
| LEFTBRACE indexed RIGHTBRACE
{
	expr *t = newexpr(newtable_e);
	t->sym = newtemp();
	emit(tablecreate, t, NULL, NULL, -1, yylineno);
	while ($2 != NULL)
	{
		emit(tablesetelem, t, $2->index, $2, -1, yylineno);
		$2 = $2->next;
	}
	$$ = t;

	fprintf(yyout_y, "objectdef -> { indexed }\n");
};

indexed:
	indexedelem {
		$$=$1;
		fprintf(yyout_y,"indexed -> indexedelem\n");} 
	|indexed COMMA indexedelem {
		 while($1->next){  
                   $1 = $1->next;
                 }        
                 $1->next = $3;
		fprintf(yyout_y,"indexed -> indexed , indexedelem\n");}  
	;

indexedelem : LEFTCURLY expr COLON expr RIGHTCURLY
{
	$2=emitBoolean($2);
	$4=emitBoolean($4);
	$4->index = $2;
	fprintf(yyout_y, "indexedelem -> { expr : expr }\n");
	$$ = $4;
};

funcname : ID
{
	$$ = $1;
	fprintf(yyout_y, "funcname -> ID\n");
}
|
{
	char *my_name = malloc(50 * (sizeof(char)));
	sprintf(my_name, "_myfync%d", unnamed_counter++);
	$$ = my_name;
	fprintf(yyout_y, "funcname -> NULL\n");
};

funcprefix : FUNCTION funcname
{
	SymbolTableEntry *search = lookup_inScope(hash, (char *)$2, 0);
	if (search != NULL)
	{
		if (search->type == LIBFUNC)
		{
			fprintf(stderr, "Userfunc shadows libraryfunn in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
		else if (current_scope == 0)
		{
			fprintf(stderr, "Found symbol with same name in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
		$$ = malloc(sizeof(expr));
		$$->sym = search;
	}
	int temp = current_scope;
	search = lookup_inScope(hash, (char *)$2, current_scope);
	if (search == NULL)
	{
		$$ = newexpr(programfunc_e);
		$$->sym = SymTable_insert(hash, (char *)$2, yylineno, NULL, temp, USERFUNC);
	}
	else
	{
		if (search->type == USERFUNC || search->type == LIBFUNC)
		{
			fprintf(stderr, "Function redefinition in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
		else
		{
			fprintf(stderr, "Function %s declared with same name as variable in line %d and scope %d \n", $2, yylineno, current_scope);
			isError = true;
		}
		$$ = malloc(sizeof(expr));
		$$->sym = search;
	}
	$$->iaddress = nextquadlabel(); 
	int *tmp = malloc(sizeof(int));
	*tmp = nextquadlabel();
	push(func_stack, *tmp);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
	emit(funcstart, newexpr_conststring($2), NULL, NULL, -1, yylineno);

	push(stack_, offset_); 
	offset_ = -1;
	enterscopespace();
	resetformalargoffset(); // Start formals from zero
	fprintf(yyout_y, "funcprefix -> FUNCTION funcname\n");};

funcargs :
	funcprefix M LEFTPAR{
	increase_scope();
	isFunct1 = true;
	isFunct = true;
}
idlist RIGHTPAR
{
	$1->sym->value.funcVal->args = $5;
	enterscopespace();		
	resetfunctionlocaloffset(); 
	fprintf(yyout_y, "funcargs -> funcprefix M ( idlist )\n");};

funcblockstart:
{
	offset_ = -1;
	sim_funcs++;
	push(loopcounterstack, loopcounter); 
	loopcounter = 0;
	fprintf(yyout_y, "funcblockstart -> NULL\n");};

funcblockend:
{
	fprintf(yyout_y, "funcblockend -> NULL\n");};


funcdef : funcargs funcblockstart block
{
	int curr_scope_offset = currscopeoffset();
	fprintf(yyout_y, "funcdef -> funcargs funcblockstart block\n");
}
funcblockend
{
	int fof = pop(func_stack);
	assert($$);
	$1->sym->value.funcVal->totalLocals = offset_ + 1;
	emit(funcend, $$, NULL, NULL, -1, yylineno);
	$$ = $1;
	loopcounter = pop(loopcounterstack);
	sim_funcs--;
	exitscopespace();
	offset_ = pop(stack_);
	patchlabel(fof,nextquad());
	// printf("%d",fof);
	fprintf(yyout_y, "funcdef -> funcblockend\n");
};

idlist:
	ID
	{
		//check if it's a gloabal lib function
		if (lookup_inScope(hash,$1,0) != NULL && lookup_inScope(hash,$1,0)->type == LIBFUNC)
		{
			fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
			isError = true;
			
		}else if(lookup_inScope(hash,$1,current_scope)!=NULL)
		{
			fprintf(stderr,"This formal argument already exists in given scope %s  in line %d", $1,current_scope,yylineno); 
			isError = true;       
			
		}else{
		
		//insertion in the idlist and saving the idlist

		$$ = create_id_list();
		insert($$,$1);

		//insertion in the symtable/scopelist
		SymbolTableEntry* temp;
		temp = SymTable_insert(hash, $1, yylineno , NULL , current_scope, FORMAL);
		temp->offset = formalArgOffset++;
		temp->space = formalarg;
		fprintf(yyout_y,"idlist -> id\n");}
	}
	|idlist COMMA ID 
	{
	//check if it's a gloabal lib function
    if (lookup_inScope(hash,(const char *)$3,0) != NULL && lookup_inScope(hash,(const char*)$3,0)->type == LIBFUNC)
    {
        fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
	isError = true;
        
    }else if(lookup_inScope(hash,(const char*)$3,current_scope)!=NULL)
    {
        fprintf(stderr,"This formal argument already exists in given scope in line %d and scope %d \n",yylineno,current_scope); 
	isError = true;      
        
    }else{
    
    //insertion in the idlist and saving the idlist
    insert($1,$3);
    $$ = $1; //not sure if this is functional 

	//insertion in the symtable/scopelist
	SymbolTableEntry* temp;
		temp =SymTable_insert(hash, (const char*)$3, yylineno , NULL , current_scope, FORMAL); /*to 2o orisma htan $1 kai de douleue to print (obviously, afou to ena einai idlist kai to allo string)*/
		//temp->offset = formalArgOffset++;
		//temp->space = formalarg;
		temp->space = currscopespace();
		temp->offset = currscopeoffset();

	fprintf(yyout_y,"idlist -> idlist , ID\n");
}
    fprintf(yyout_y,"idlist -> idlist , id\n");
	
	}
	| 
	{    
		$$ = create_id_list();
 		fprintf(yyout_y,"idlist ->  \n"); 
	};
block : LEFTCURLY
{
	if (sim_funcs != 0)
	{
		b_af++;
	}
	if (isFunct == true)
	{
		isFunct = false;
	}
	else
	{
		increase_scope();
	}
}
RIGHTCURLY
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	fprintf(yyout_y, "block -> { }\n");
	decrease_scope();
	if (sim_funcs == b_af && sim_funcs > 0)
	{
		b_af--;
	}
	else if (sim_funcs < b_af)
	{
		b_af--;
	}
}
| LEFTCURLY
{
	if (sim_funcs != 0)
	{
		b_af++;
	}
	if (isFunct == true)
	{
		isFunct = false;
	}
	else
	{
		increase_scope();
	}
}
stmts RIGHTCURLY
{
	$$ = $3;
	fprintf(yyout_y, "block -> { stmts }\n");
	decrease_scope();
	if (sim_funcs == b_af && sim_funcs > 0)
	{
		b_af--;
	}
	else if (sim_funcs < b_af)
	{
		b_af--;
	}
};

ifprefix : IF LEFTPAR expr RIGHTPAR
{
	$3 = emitBoolean($3);
	emit(if_eq, NULL, $3, newexpr_constbool('t'), nextquad() + 2, yylineno);
	$$ = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
	fprintf(yyout_y, "ifprefix -> IF ( expr )\n");
};

if:	
	ifprefix stmt
	{
		$$ = $2;
		patchlabel($1, nextquad());
		fprintf(yyout_y, "if -> ifprefix stmt\n");
	}
| ifprefix stmt elseprefix stmt
{
	patchlabel($1, $3 + 1);
	patchlabel($3, nextquad());
	$$ = malloc(sizeof(stmt_t));

	$$->breaklist = mergelist($2->breaklist, $4->breaklist);
	$$->contlist = mergelist($2->contlist, $4->contlist);
	$$->returnlist = mergelist($2->returnlist, $4->returnlist);
	fprintf(yyout_y, "if -> ifprefix stmt elseprefix stmt\n");
};

elseprefix : ELSE
{
	$$ = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
};

loopstart:
{
	++loopcounter;
};

loopend:
{
	--loopcounter;
};

loopstmt : loopstart stmt loopend
{
	$$ = $2;
};

whilestart : WHILE
{
	$$ = nextquad();
};

whilecond : LEFTPAR { sim_loops++; }
expr RIGHTPAR
{
	$3 = emitBoolean($3);
	emit(if_eq, NULL, $3, newexpr_constbool('t'), nextquad() + 2, yylineno);
	$$ = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
};

whilestmt : whilestart whilecond loopstmt
{
	fprintf(yyout_y, "whilestmt -> while ( expr ) stmt\n");
	emit(jump, NULL, NULL, NULL, $1, yylineno);
	int bl = 0, cl = 0;
	patchlabel($2, nextquad());
	patchlist($3->breaklist, nextquad());
	patchlist($3->contlist, $1);
	$$ = $3;
};

N:
{
	$$ = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
	fprintf(yyout_y, "N -> NULL \n");
};
M:
{
	$$ = nextquad();
	fprintf(yyout_y, "M -> NULL\n");
};

forprefix : FOR LEFTPAR { sim_loops++; }
elist SEMICOLON M expr SEMICOLON
{
	$$ = malloc(sizeof(for_t));
	$$->test = $6;
	$7 = emitBoolean($7);

	$$->enter = nextquad();
	emit(if_eq, NULL, $7, newexpr_constbool('t'), 0, yylineno);
	fprintf(yyout_y, "forprefix -> for ( \n");
};

forstmt : forprefix N elist RIGHTPAR N loopstmt N
{
	patchlabel($1->enter, $5 + 1); // true jump
	patchlabel($2, nextquad());	   // false jump
	patchlabel($5, $1->test);	   // loop jump
	patchlabel($7, $2 + 1);		   // closure jump

	patchlist($6->breaklist, nextquad());
	patchlist($6->contlist, $2 + 1);
	$$ = $6;
	fprintf(yyout_y, "forstmt -> forprefix N elist ( N loopstmt N \n");
};
returnstmt : RETURN
{
	if (sim_funcs == 0)
	{
		fprintf(stderr, "Error: Return statement not inside of a function in line %d\n", yylineno);
		isError = true;
	}
}
SEMICOLON
{
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	$$->returnlist = newlist(nextquad());
	emit(returnn, NULL, NULL, NULL, -1, nextquad());
	fprintf(yyout_y, "returnstmt -> return ;\n");
}
| RETURN
{
	if (sim_funcs == 0)
	{
		fprintf(stderr, "Return statement not inside of a function in line %d\n", yylineno);
		isError = true;
	}
}
expr SEMICOLON
{
	$3 = emitBoolean($3);
	$$ = malloc(sizeof(stmt_t));
	make_stmt($$);
	$$->returnlist = newlist(nextquad());
	emit(returnn, $3, NULL, NULL, -1, nextquad());
	fprintf(yyout_y, "returnstmt -> return expr ;\n");
};



%%
int main(int argc, char** argv)
{
	stack_ = create_stack();
	loopcounterstack = create_stack();
	func_stack=create_stack();
    //programVarStack = create_stack();
    yyout_y = fopen("yacc_output.txt", "w");
    hash = SymTable_new();
    //symtable_print(head_scope_node,hash);
    if(argc > 1 ){
        if(!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "cannot read file: %s\n", argv[1]);
            isError = true;
            return 1;
        }
    }
    else
        yyin= stdin;
	
	expand();

	emit(jump,NULL,NULL,NULL,0,0);
	
    yyparse();
    symtable_print(head_scope_node,hash);
    if(!isError){
    	printMedianCode();
    }else{
	printf("Could not produce Median Code because of compile time errors \n");
	}
    return 0;
}
