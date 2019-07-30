%error-verbose
%locations
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);//�ɱ䳤����
void display(struct node *,int);
%}

%union{
    int type_int;
    float type_float;
    double type_double;
    char type_char;
    char type_id[32];
    struct node *ptr;
};

//%type������ս��������ֵ����(��Ӧunion�г�Աptr�����ͣ���ʵ����Ϊһ�����ڵ��ָ��)
%type <ptr> program ExtDefList ExtDef StructSpecifier OptTag Tag Specifier ExtDecList FuncDec CompSt VarList VarDec ParamDec Stmt StmList DefList Def DecList Dec Exp Args 


//%token�����ս��������ֵ����
%token <type_int> INT//ָ��INT������ֵ��type_int,�ɴʷ������õ�����ֵ
%token <type_id> ID RELOP TYPE//ָ��ID��RELOP��TYPE������ֵ��type_id,�ɴʷ������õ��ı�ʶ���ַ�
%token <type_float> FLOAT//ָ��FLOAT��������type_id,�ɴʷ������õ��ı�ʶ���ַ���
%token <type_double> DOUBLE
%token <type_char> CHAR

%token LP RP LB RB LC RC SEMI COMMA//��bison�Ը��ļ�����ʱ��������-d,���ɵ�exp.tab.h�и���Щ���ʽ��б��룬����lex.l�а���parser.tab.hʹ����Щ����������
//LP: (
//RP: )
//LB: [
//RB: ]
//LC: {
//RC: }
//SEMI: �ֺ�
//COMMA: ����
%token PLUS MINUS STAR DIV ASSIGNOP AND OR NOT IF ELSE WHILE RETURN INC DEC STRUCT COMP_PLUS COMP_MINUS

%left ASSIGNOP//��ֵ��=
%left OR
%left AND
%left RELOP

%left COMP_PLUS COMP_MINUS
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT INC DEC

%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%
//program: ��ʼ�﷨��Ԫ
 //program: ExtDefList {display($1,0);semantic_Analysis0($1);prnIR($1->code);}//��ʾ�﷨�������������display��ast.c�ж��壬semantic_Analysis0��def.h�ж���

 //program: ExtDefList {display($1,0);semantic_Analysis0($1);}//��ʾ�﷨�������������display��ast.c�ж��壬semantic_Analysis0��def.h�ж���
 program: ExtDefList {display($1,0);semantic_Analysis0($1);prnIR($1->code);genCode($1->code);}//��ʾ�﷨�������������display��ast.c�ж��壬semantic_Analysis0��def.h�ж���
//program: ExtDefList {display($1,0);}//��ʾ�﷨�������������display��ast.c�ж��壬semantic_Analysis0��def.h�ж���
        ;
//ExtDefList: �������ExtDef
ExtDefList: {$$=NULL;}
        |ExtDef ExtDefList {$$=mknode(EXT_DEF_LIST,$1,$2,NULL,yylineno);}//ÿһ��EXT_DEF_LIST�Ľڵ㣬���һ��������Ӧһ���ⲿ������������
        ;
//ExtDef: һ��ȫ�ֱ������ṹ������Ķ���
ExtDef: Specifier ExtDecList SEMI {$$=mknode(EXT_VAR_DEF,$1,$2,NULL,yylineno);}//�ýڵ��Ӧһ���ⲿ��������
        |Specifier FuncDec CompSt {$$=mknode(FUNC_DEF,$1,$2,$3,yylineno);}//�ýڵ��Ӧһ����������
        |Specifier SEMI           {$$=mknode(STRUCT_TYPE_DEF,$1,NULL,NULL,yylineno);}//�ýڵ��Ӧһ���ṹ�嶨��
        |error SEMI               {$$=NULL;}
        ;
//StructSpecifier: �ṹ��������
StructSpecifier: STRUCT OptTag LC DefList RC {$$=mknode(STRUCT_DEF,$2,$4,NULL,yylineno);}
        | STRUCT Tag {$$=mknode(STRUCT_VAR_DEF,$2,NULL,NULL,yylineno);}
        ;
//Specifier: ����������
Specifier: TYPE {$$=mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);
        if(!strcmp($1,"int")) $$->type=INT;
        if(!strcmp($1,"float")) $$->type=FLOAT;
        if(!strcmp($1,"double")) $$->type=DOUBLE;
        if(!strcmp($1,"char")) $$->type=CHAR;}
        | StructSpecifier {$$=mknode(STRUCT_SPECIFIER,$1,NULL,NULL,yylineno);}
        ;
//
OptTag: ID {$$=mknode(TAG,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | {printf("OptTag");$$=NULL;}
        ;
Tag: ID {$$=mknode(TAG,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        ;
//ExtDecList: �������VarDec
ExtDecList: VarDec {$$=$1;}//ÿһ��EXT_DEFLIST�Ľ�㣬���һ��������Ӧһ��������(ID���͵Ľ��)���ڶ���������Ӧʣ�µ��ⲿ������
        | VarDec COMMA ExtDecList {$$=mknode(EXT_DEC_LIST,$1,$3,NULL,yylineno);}
        ;
//VarDec: һ��������ID
VarDec: ID {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//ID��㣬��ʶ�����Ŵ���Žڵ��type_id
        | VarDec LB INT RB {$$=mknode(ARRAY_DEF,$1,NULL,NULL,yylineno);$$->array_size[0]=$3;}//һά����
        //| VarDec LB INT RB LB INT RB {$$=mknode(TWO_ARRAY_DEF,$1,NULL,NULL,yylineno);$$->array_size[0]=$3;$$->array_size[1]=$6;}//��ά����
        ;
//FuncDec: ����ͷ
FuncDec: ID LP VarList RP {$$=mknode(FUNC_DEC,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//�����������$$->type_id
        |ID LP RP         {$$=mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//�����������$$->type_id
        ;  
//VarList: �β��б�
VarList: ParamDec                {$$=mknode(PARAM_LIST,$1,NULL,NULL,yylineno);}
        | ParamDec COMMA VarList {$$=mknode(PARAM_LIST,$1,$3,NULL,yylineno);}
        ;
//ParamDec: һ���βεĶ���
ParamDec: Specifier VarDec {$$=mknode(PARAM_DEC,$1,$2,NULL,yylineno);}
        ;
//CompSt: �����塢�ɻ�����������������, ���������
/*
CompSt: LC StmList RC {$$=mknode(COMP_STM,$2,NULL,NULL,yylineno);}
        ;*/
CompSt: LC DefList StmList RC {$$=mknode(COMP_STM,$2,$3,NULL,yylineno);}
        ;        
//StmList: ����б�
StmList: {$$=NULL;}  
        | Stmt StmList {$$=mknode(STM_LIST,$1,$2,NULL,yylineno);}
        ;
//Stmt: һ�����
/*
Stmt: Def   {$$=$1;}
        | Exp SEMI                        {$$=mknode(EXP_STMT,$1,NULL,NULL,yylineno);}
        | CompSt                      {$$=$1;}//���������ֱ����Ϊ����㣬���������µĽ��
        | RETURN Exp SEMI             {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}
        | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);}
        | IF LP Exp RP Stmt ELSE Stmt {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
        | WHILE LP Exp RP Stmt        {$$=mknode(WHILE,$3,$5,NULL,yylineno);}
        |error SEMI               {$$=NULL;}
        ;
*/
Stmt: Exp SEMI                        {$$=mknode(EXP_STMT,$1,NULL,NULL,yylineno);}
        | CompSt                      {$$=$1;}//���������ֱ����Ϊ����㣬���������µĽ��
        | RETURN Exp SEMI             {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}
        | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);}
        | IF LP Exp RP Stmt ELSE Stmt {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
        | WHILE LP Exp RP Stmt        {$$=mknode(WHILE,$3,$5,NULL,yylineno);}
        ;
//DefList: ���������б�ɾ���ˣ��ּ��˻���
DefList: {$$=NULL;}
        | Def DefList {$$=mknode(DEF_LIST,$1,$2,NULL,yylineno);}
        ;
//Def: һ����������
Def: Specifier DecList SEMI {$$=mknode(VAR_DEF,$1,$2,NULL,yylineno);}
        |error SEMI               {$$=NULL;}
        ;

DecList: Dec                {$$=mknode(DEC_LIST,$1,NULL,NULL,yylineno);}
        | Dec COMMA DecList {$$=mknode(DEC_LIST,$1,$3,NULL,yylineno);}
        ;
Dec: VarDec {$$=$1;}
        | VarDec ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
        ;
//Exp: һ�����ʽ
Exp: Exp ASSIGNOP Exp   {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}//$$���type_id����δ�ã����ô�������
        | Exp AND Exp   {$$=mknode(AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"AND");}
        | Exp OR Exp    {$$=mknode(OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"OR");}
        | Exp RELOP Exp {$$=mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}  //�ʷ�������ϵ�����������ֵ������$2��
        | Exp PLUS Exp  {$$=mknode(PLUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUS");}
        | Exp MINUS Exp {$$=mknode(MINUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"MINUS");}
        | Exp STAR Exp  {$$=mknode(STAR,$1,$3,NULL,yylineno);strcpy($$->type_id,"STAR");}
        | Exp DIV Exp   {$$=mknode(DIV,$1,$3,NULL,yylineno);strcpy($$->type_id,"DIV");}
        | LP Exp RP     {$$=$2;}
        | MINUS Exp %prec UMINUS   {$$=mknode(UMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"UMINUS");}
        | Exp COMP_PLUS Exp        {$$=mknode(COMP_PLUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"COMP_PLUS");}
        | Exp COMP_MINUS Exp        {$$=mknode(COMP_MINUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"COMP_MINUS");}
        | Exp INC       {$$=mknode(INC,$1,NULL,NULL,yylineno);strcpy($$->type_id,"PRE_INC");}
        | Exp DEC       {$$=mknode(DEC,$1,NULL,NULL,yylineno);strcpy($$->type_id,"PRE_DEC");}
        | INC Exp       {$$=mknode(INC,$2,NULL,NULL,yylineno);strcpy($$->type_id,"POST_INC");}
        | DEC Exp       {$$=mknode(DEC,$2,NULL,NULL,yylineno);strcpy($$->type_id,"POST_DEC");}
        | NOT Exp       {$$=mknode(NOT,$2,NULL,NULL,yylineno);strcpy($$->type_id,"NOT");}
        | ID LP Args RP {$$=mknode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | ID LP RP      {$$=mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | VarDec LB INT RB {$$=mknode(ARRAY_DEF,$1,NULL,NULL,yylineno);strcpy($$->type_id,"ARRAY_DEF");$$->array_size[0]=$3;}
        | ID            {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | INT           {$$=mknode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}
        | FLOAT         {$$=mknode(FLOAT,NULL,NULL,NULL,yylineno);$$->type_float=$1;$$->type=FLOAT;}
        | DOUBLE        {$$=mknode(DOUBLE,NULL,NULL,NULL,yylineno);$$->type_double=$1;$$->type=DOUBLE;}
        | CHAR          {$$=mknode(CHAR,NULL,NULL,NULL,yylineno);$$->type_char=$1;$$->type=CHAR;}
        
        ;
//Args: ʵ���б�
Args: Exp COMMA Args    {$$=mknode(ARGS,$1,$3,NULL,yylineno);}
        | Exp           {$$=mknode(ARGS,$1,NULL,NULL,yylineno);}
        ;
       
%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (!yyin) return 1;//return������1
	yylineno=1;
        // yydebug=1;
	yyparse();
	return 0;
}

#include <stdarg.h>
void yyerror(const char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}	
