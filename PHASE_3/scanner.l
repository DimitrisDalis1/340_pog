/*%option header-file="./al.h"*/
%option noyywrap
%option pointer
%option yylineno
%x COMMENT2
%x STRINGG


/*declarations*/
%{
#include <math.h>
#include <stdlib.h>
#include "parser.h"
#include <string.h>
//#define YY_DECL int alpha_yylex(void* yylval)
#define MAX_STRING_SIZE 100000
int yytokenno = 0;
char string_buf[MAX_STRING_SIZE];
char *strbuffer;
char comment_block[5]; 
int nested_flag = 0;

/*token node for storing the token information in a list*/
struct alpha_token_t {
  unsigned int     numline;
  unsigned int     numToken;
  char             *content;
  char             *specifier;
  char             *type;
  char             *category;
  struct alpha_token_t *alpha_yylex;
};

/*comment node for storing comment info in a list*/
struct comment_node{
  unsigned int     start;
  unsigned int     end;
  struct comment_node *next;
};

/*initialize*/
struct alpha_token_t *head=NULL;
struct alpha_token_t *temp=NULL;
struct comment_node *head_comment=NULL;

%}

%{


/*insert node to the list*/
void insert_node(char *type,char* ytext,char* x){
            struct alpha_token_t* yylval= (struct alpha_token_t * )malloc(sizeof(struct alpha_token_t));     /*malloc the size of a token-node*/  
            yylval->content=malloc((1 + strlen(ytext))*sizeof(char));   /*malloc the size of the content*/
            yylval->numToken = yytokenno; /*put the information inside the node*/
            strcpy(yylval->content, ytext);
            yylval->type = type;  
            yylval->specifier=x;
            if(!strcmp(yylval->type,"INTCONST")){
                yylval->category = "integer";
            } else if(!strcmp(yylval->type,"REALCONST")){
                yylval->category = "real";
            } else if(!strcmp(yylval->type,"STRING")||!strcmp(yylval->type,"IDENT")){
                yylval->category = "char*";
            }else{
                yylval->category = "enumerated";
            }
            if(!strcmp(yylval->type,"COMMENT")){
                yylval->numline = atoi(&comment_block[0]);
            }else{
                yylval->numline = yylineno;
            }
            yylval->alpha_yylex=NULL; /*connect it with the list*/
            if(head == NULL){  
                head=yylval; 
                temp = head;
            }  
            else{  
                temp->alpha_yylex=yylval;
                temp=yylval;
            }
           
}


/*function that inserts a comment_node in a list*/
struct comment_node  * insert_comment(int start, struct comment_node  *h) {
    struct comment_node  *node;
    node = (struct comment_node  *) malloc(sizeof(struct comment_node ));
    node->start = start;
    node->next= h;
    node->end=0;
    h = node;
    return h;
}

/*function that returns the last node of the list*/
struct comment_node  *get_last_element(int num, struct comment_node  *h) {
    struct comment_node  *tmp;
    tmp=h;
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }
    tmp->end=num;
    return tmp;
}

/*function that prints all the information stored in the list*/
void print_list(int option, char* file){
  FILE *ptr;
  if(head){
    temp=head; /*determine where the output will be based on how many arguments are given*/
    if(option==0){
        printf("-------------------      Lexical Analysis     -------------------\n");
        while(temp!= NULL)
            {   
                if(!strcmp(temp->type,"INTCONST")){
                    printf("%d: #%d  \"%s\"   %s  %d  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,atoi(temp->content),temp->category);
                }else if(!strcmp(temp->type,"REALCONST")){
                    printf("%d: #%d  \"%s\"   %s  %.2f  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,atof(temp->content),temp->category);
                }else if(!strcmp(temp->type,"STRING")||!strcmp(temp->type,"IDENT")){
                    printf("%d: #%d  \"%s\"   %s  \"%s\"  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,temp->content,temp->category);
                }else{
                    printf("%d: #%d  \"%s\"   %s  %s  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,temp->specifier,temp->category);
                    
                }   
                temp = temp->alpha_yylex;
            }
        
  }else{
    ptr = fopen(file, "w");
    fprintf(ptr,"-------------------      Lexical Analysis     -------------------\n");
    while(temp!= NULL)
            {   
                if(!strcmp(temp->type,"INTCONST")){
                    fprintf(ptr,"%d: #%d  \"%s\"   %s  %d  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,atoi(temp->content),temp->category);
                }else if(!strcmp(temp->type,"REALCONST")){
                    fprintf(ptr,"%d: #%d  \"%s\"   %s  %.2f  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,atof(temp->content),temp->category);
                }else if(!strcmp(temp->type,"STRING")||!strcmp(temp->type,"IDENT")){
                    fprintf(ptr,"%d: #%d  \"%s\"   %s  \"%s\"  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,temp->content,temp->category);
                }else{
                    fprintf(ptr,"%d: #%d  \"%s\"   %s  %s  <-%s\n",temp->numline, temp->numToken , temp->content ,temp->type,temp->specifier,temp->category);
                    
                }   
                temp = temp->alpha_yylex;
            }
        
        fclose(ptr);

  }
}
}

/*do what its name says,deletes all the list*/
void free_list(){
    struct alpha_token_t* temp = head;
    if(head==NULL){
        printf("already empty list");
    }else{
        while(temp!=NULL){
            head=temp->alpha_yylex;
            free(temp->content);
            free(temp);
            temp=head;
        }
    }
}



%}

INTCONST                [0-9]+
REALCONST	            [0-9]+"."[0-9]*	
KEYWORD_IF              "if"
KEYWORD_ELSE            "else"    
KEYWORD_WHILE           "while"
KEYWORD_FOR             "for"
KEYWORD_FUNCTION        "function"
KEYWORD_RETURN          "return"
KEYWORD_BREAK           "break"
KEYWORD_CONTINUE        "continue"
KEYWORD_AND             "and"
KEYWORD_NOT             "not"
KEYWORD_OR              "or"
KEYWORD_LOCAL           "local"
KEYWORD_TRUE            "true"
KEYWORD_FALSE           "false"
KEYWORD_NIL             "nil"
IDENT                   [a-zA-Z][a-zA-Z_0-9]*
NEWLINE	                \n
OPERATOR_ASSIGN         "="
OPERATOR_PLUS           "+"
OPERATOR_MINUS          "-"
OPERATOR_MULT           "*"
OPERATOR_DIV            "/"
OPERATOR_PERC           "%"
OPERATOR_EQUAL          "=="
OPERATOR_NOT_EQUAL      "!="
OPERATOR_PLUS2          "++"
OPERATOR_MINUS2         "--"
OPERATOR_BIGGER         ">"
OPERATOR_SMALLER        "<"
OPERATOR_BIGGER_EQUAL   ">="
OPERATOR_SMALLER_EQUAL  "<="
PUNCTUATION_LEFTCURLY   "{"
PUNCTUATION_RIGHTCURLY  "}"
PUNCTUATION_LEFTBRACE   "["
PUNCTUATION_RIGHTBRACE  "]"
PUNCTUATION_LEFTPAR     "("
PUNCTUATION_RIGHTPAR    ")"
PUNCTUATION_SEMICOLON   ";"
PUNCTUATION_COMMA       ","
PUNCTUATION_COLON       ":"
PUNCTUATION_COLON2      "::"
PUNCTUATION_PERIOD      "."
PUNCTUATION_PERIOD2     ".."
COMMENT1                "//".*

%%

\n                        {}  

{KEYWORD_IF}                {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"IF");
                                yylval.stringValue = strdup(yytext);
                                return IF;
                            }
{KEYWORD_ELSE}              {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"ELSE"); 
                                yylval.stringValue = strdup(yytext); 
                                return ELSE;
                            }    
{KEYWORD_WHILE}             {   
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"WHILE"); 
                                yylval.stringValue = strdup(yytext); 
                                return WHILE;
                            }
{KEYWORD_FOR}               {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"FOR"); 
                                yylval.stringValue = strdup(yytext); 
                                return FOR;
                            }
{KEYWORD_FUNCTION}          {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"FUNCTION");
                                yylval.stringValue = strdup(yytext);
                                return FUNCTION;
                            }
{KEYWORD_RETURN}            {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"RETURN");
                                yylval.stringValue = strdup(yytext);
                                return RETURN;
                            }
{KEYWORD_BREAK}             {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"BREAK");
                                yylval.stringValue = strdup(yytext);
                                return BREAK;
                            }
{KEYWORD_CONTINUE}          {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"CONTINUE");
                                yylval.stringValue = strdup(yytext);
                                return CONTINUE;
                            }
{KEYWORD_AND}               {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"AND");
                                yylval.stringValue = strdup(yytext);
                                return AND;
                            }
{KEYWORD_NOT}               {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"NOT");
                                yylval.stringValue = strdup(yytext);
                                return NOT;
                            }
{KEYWORD_OR}                {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"OR");
                                yylval.stringValue = strdup(yytext);
                                return OR;
                            }
{KEYWORD_LOCAL}             {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"LOCAL");
                                yylval.stringValue = strdup(yytext);
                                return LOCAL;
                            }
{KEYWORD_TRUE}              {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"TRUE");
                                yylval.stringValue = strdup(yytext);
                                return TRUE;
                            }
{KEYWORD_FALSE}             {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"FALSE");
                                yylval.stringValue = strdup(yytext);
                                return FALSE;
                            }
{KEYWORD_NIL}               {
                                yytokenno++; 
                                insert_node("KEYWORD",yytext,"NIL");
                                yylval.stringValue = strdup(yytext);
                                return NIL;
                            }
{OPERATOR_ASSIGN}           {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"ASSIGN");
                                yylval.stringValue = strdup(yytext);
                                return ASSIGN;
                            }
{OPERATOR_PLUS}             {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"PLUS");
                                yylval.stringValue = strdup(yytext);
                                return PLUS;
                            }
{OPERATOR_MINUS}            {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"MINUS");
                                yylval.stringValue = strdup(yytext);
                                return MINUS;
                            }
{OPERATOR_MULT}             {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"MULT");
                                yylval.stringValue = strdup(yytext);
                                return MULT;
                            }
{OPERATOR_DIV}              {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"DIV");
                                yylval.stringValue = strdup(yytext);
                                return DIV;
                            }
{OPERATOR_PERC}             {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"PERC");
                                yylval.stringValue = strdup(yytext);
                                return PERC;
                            }
{OPERATOR_EQUAL}            {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"EQUAL");
                                yylval.stringValue = strdup(yytext);
                                return EQUAL;
                            }
{OPERATOR_NOT_EQUAL}        {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"NOT_EQUAL");
                                yylval.stringValue = strdup(yytext);
                                return NOT_EQUAL;
                            }
{OPERATOR_PLUS2}            {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"PLUS2");
                                yylval.stringValue = strdup(yytext);
                                return PLUS2;
                            }
{OPERATOR_MINUS2}           {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"MINUS2");
                                yylval.stringValue = strdup(yytext);
                                return MINUS2;
                            }
{OPERATOR_BIGGER}           {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"BIGGER");
                                yylval.stringValue = strdup(yytext);
                                return BIGGER;
                            }
{OPERATOR_SMALLER}          {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"SMALLER");
                                yylval.stringValue = strdup(yytext);
                                return SMALLER;
                            }
{OPERATOR_BIGGER_EQUAL}     {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"BIGGER_EQUAL");
                                yylval.stringValue = strdup(yytext);
                                return BIGGER_EQUAL;
                            }
{OPERATOR_SMALLER_EQUAL}    {
                                yytokenno++; 
                                insert_node("OPERATOR",yytext,"SMALLER_EQUAL");
                                yylval.stringValue = strdup(yytext);
                                return SMALLER_EQUAL;
                            }
{PUNCTUATION_LEFTCURLY}     {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"LEFTCURLY");
                                yylval.stringValue = strdup(yytext);
                                return LEFTCURLY;
                            }
{PUNCTUATION_RIGHTCURLY}    {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"RIGHTCURLY");
                                yylval.stringValue = strdup(yytext);
                                return RIGHTCURLY;
                            }
{PUNCTUATION_LEFTBRACE}     {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"LEFTBRACE");
                                yylval.stringValue = strdup(yytext);
                                return LEFTBRACE;
                            }
{PUNCTUATION_RIGHTBRACE}    {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"RIGHTBRACE");
                                yylval.stringValue = strdup(yytext);
                                return RIGHTBRACE;
                            }
{PUNCTUATION_LEFTPAR}       {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"LEFTPAR");
                                yylval.stringValue = strdup(yytext);
                                return LEFTPAR;
                            }
{PUNCTUATION_RIGHTPAR}      {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"RIGHTPAR");
                                yylval.stringValue = strdup(yytext);
                                return RIGHTPAR;
                            }
{PUNCTUATION_SEMICOLON}     {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"SEMICOLON");
                                yylval.stringValue = strdup(yytext);
                                return SEMICOLON;
                            }
{PUNCTUATION_COMMA}         {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"COMMA");
                                yylval.stringValue = strdup(yytext);
                                return COMMA;
                            }
{PUNCTUATION_COLON}         {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"COLON");
                                yylval.stringValue = strdup(yytext);
                                return COLON;
                            }
{PUNCTUATION_COLON2}        {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"COLON2");
                                yylval.stringValue = strdup(yytext);
                                return COLON2;
                            }
{PUNCTUATION_PERIOD}        {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"PERIOD");
                                yylval.stringValue = strdup(yytext);
                                return PERIOD;
                            }
{PUNCTUATION_PERIOD2}       {
                                yytokenno++; 
                                insert_node("PUNCTUATION",yytext,"PERIOD2");
                                yylval.stringValue = strdup(yytext);
                                return PERIOD2;
                            }            

"/*"             {  
                    BEGIN(COMMENT2); 
                    head_comment=insert_comment(yylineno,head_comment);
                  
                }

<COMMENT2>{
  "/*"           {
                    head_comment=insert_comment(yylineno,head_comment);
                    ++nested_flag;                                      //keep count for nested comments
                 }
  "*/"           {
                    struct comment_node *t=NULL;
                    if (nested_flag>0) {
                        yytokenno++;
                        head_comment->end=yylineno;
                        sprintf(comment_block,"%d-%d",head_comment->start,head_comment->end);
                        insert_node("COMMENT",comment_block,"NESTED_comment");
                        yylval.stringValue = strdup(yytext);
                        --nested_flag;
                    }
                   else{ 
                    BEGIN(INITIAL);
                    head_comment->end=yylineno;
                    yytokenno++; 
                    sprintf(comment_block,"%d-%d",head_comment->start,head_comment->end);
                    insert_node("COMMENT",comment_block,"BLOCK_comment");
                    yylval.stringValue = strdup(yytext);
                    }
                    //remove head from list and free allocated memory
                    t=head_comment;
                    head_comment=head_comment->next;
                    free(t);

                }
  "*"+           ; 
  [^/*\n]+       ; 
  [/]            ; 
  \n             ; 
  <<EOF>> 	    {
                    printf("Error! Comment did not terminate correctly.\n"); 
                    return -1;             
                }
}

\"              strbuffer = string_buf; BEGIN(STRINGG);

<STRINGG>\"         {
                        yytokenno++;
                        BEGIN(INITIAL);
                        *strbuffer = '\"';
			++strbuffer;
			*strbuffer = '\0';
			
                        insert_node("STRING",string_buf,NULL);
                        yylval.stringValue = strdup(string_buf);
                        return STRING;
                }


<STRINGG>\\[0-7]{1,3}       {
                                /* octal escape sequence */
                                int result;
                                (void) sscanf( yytext + 1, "%o", &result );
                                *strbuffer++ = result;
                        }

<STRINGG>\\[0-9]+           {
                                /*BAD_SEQUENCE*/
                                printf("Warning! Bad Sequence\n");
                        }

<STRINGG>\\n  *strbuffer++ = '\n';
<STRINGG>\\t  *strbuffer++ = '\t';
<STRINGG>\\r  *strbuffer++ = '\r';
<STRINGG>\\b  *strbuffer++ = '\b';
<STRINGG>\\f  *strbuffer++ = '\f';

<STRINGG>\\(.|\n)  *strbuffer++ = yytext[1];

<STRINGG>[^\\\n\"]+         {
                               
                                char *yptr = yytext;
                                while ( *yptr ){
                                    *strbuffer++ = *yptr++;
                                }
                        }   
                           

<STRINGG><<EOF>> {
                    printf("Error! String did not terminate correctly.\n");
                    return -1;
                }

{INTCONST}    {
    yytokenno++;
    insert_node("INTCONST",yytext,NULL);
    yylval.intValue = atoi(yytext);
    return INT;
            }

{REALCONST}   {
    yytokenno++;
    insert_node("REALCONST",yytext,NULL);
    yylval.doubleValue = atof(yytext);
    return REAL;
            }

{IDENT}        {
    yytokenno++;
    insert_node("IDENT",yytext,NULL);
    yylval.stringValue = strdup(yytext);
    return ID;
}

{COMMENT1}     {
    yytokenno++;
    sprintf(comment_block,"%d",yylineno);    
    insert_node("COMMENT","","LINE_comment");
    yylval.stringValue = strdup(yytext);
            } 

[ \t\n]+          /* eat up whitespace */

.           printf( "#%d Unrecognized character: %s\n",yylineno, yytext );

%%
/*run lex*/
/*int main( int argc,  char **argv ){
    struct alpha_token_t * yylval=NULL;
    if ( argc > 1 ){
        if(!(yyin = fopen(argv[1],"r"))){//read file
                fprintf(stderr,"Cannot read file: %s\n",argv[1]);
                return 1;
            }
	}else{
        //get input from terminal
            yyin = stdin;
    }
    //call yylex
    alpha_yylex(yylval);
    //print in file
    if(argc==3){
        print_list(1,argv[2]);
    }else{
        //print in terminal
        print_list(0,NULL);        
    }
    //free allocated memory
    free_list();
    return 0;
}*/
