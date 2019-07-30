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
void yyerror(const char* fmt, ...);//可变长参数
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

//%type定义非终结符的语义值类型(对应union中成员ptr的类型，本实验中为一个树节点的指针)
%type <ptr> program ExtDefList ExtDef StructSpecifier OptTag Tag Specifier ExtDecList FuncDec CompSt VarList VarDec ParamDec Stmt StmList DefList Def DecList Dec Exp Args 


//%token定义终结符的语义值类型
%token <type_int> INT//指定INT的语义值是type_int,由词法分析得到的数值
%token <type_id> ID RELOP TYPE//指定ID、RELOP、TYPE的语义值是type_id,由词法分析得到的标识符字符
%token <type_float> FLOAT//指定FLOAT的语义是type_id,由词法分析得到的标识符字符串
%token <type_double> DOUBLE
%token <type_char> CHAR

%token LP RP LB RB LC RC SEMI COMMA//用bison对该文件编译时，带参数-d,生成的exp.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码
//LP: (
//RP: )
//LB: [
//RB: ]
//LC: {
//RC: }
//SEMI: 分号
//COMMA: 逗号
%token PLUS MINUS STAR DIV ASSIGNOP AND OR NOT IF ELSE WHILE RETURN INC DEC STRUCT COMP_PLUS COMP_MINUS

%left ASSIGNOP//赋值号=
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
//program: 初始语法单元
 //program: ExtDefList {display($1,0);semantic_Analysis0($1);prnIR($1->code);}//显示语法树，语义分析。display在ast.c中定义，semantic_Analysis0在def.h中定义

 //program: ExtDefList {display($1,0);semantic_Analysis0($1);}//显示语法树，语义分析。display在ast.c中定义，semantic_Analysis0在def.h中定义
 program: ExtDefList {display($1,0);semantic_Analysis0($1);prnIR($1->code);genCode($1->code);}//显示语法树，语义分析。display在ast.c中定义，semantic_Analysis0在def.h中定义
//program: ExtDefList {display($1,0);}//显示语法树，语义分析。display在ast.c中定义，semantic_Analysis0在def.h中定义
        ;
//ExtDefList: 零个或多个ExtDef
ExtDefList: {$$=NULL;}
        |ExtDef ExtDefList {$$=mknode(EXT_DEF_LIST,$1,$2,NULL,yylineno);}//每一个EXT_DEF_LIST的节点，其第一棵子树对应一个外部变量声明或函数
        ;
//ExtDef: 一个全局变量、结构体或函数的定义
ExtDef: Specifier ExtDecList SEMI {$$=mknode(EXT_VAR_DEF,$1,$2,NULL,yylineno);}//该节点对应一个外部变量声明
        |Specifier FuncDec CompSt {$$=mknode(FUNC_DEF,$1,$2,$3,yylineno);}//该节点对应一个函数定义
        |Specifier SEMI           {$$=mknode(STRUCT_TYPE_DEF,$1,NULL,NULL,yylineno);}//该节点对应一个结构体定义
        |error SEMI               {$$=NULL;}
        ;
//StructSpecifier: 结构体描述符
StructSpecifier: STRUCT OptTag LC DefList RC {$$=mknode(STRUCT_DEF,$2,$4,NULL,yylineno);}
        | STRUCT Tag {$$=mknode(STRUCT_VAR_DEF,$2,NULL,NULL,yylineno);}
        ;
//Specifier: 类型描述符
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
//ExtDecList: 零个或多个VarDec
ExtDecList: VarDec {$$=$1;}//每一个EXT_DEFLIST的结点，其第一棵子树对应一个变量名(ID类型的结点)，第二棵子树对应剩下的外部变量名
        | VarDec COMMA ExtDecList {$$=mknode(EXT_DEC_LIST,$1,$3,NULL,yylineno);}
        ;
//VarDec: 一个变量的ID
VarDec: ID {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//ID结点，标识符符号串存放节点的type_id
        | VarDec LB INT RB {$$=mknode(ARRAY_DEF,$1,NULL,NULL,yylineno);$$->array_size[0]=$3;}//一维数组
        //| VarDec LB INT RB LB INT RB {$$=mknode(TWO_ARRAY_DEF,$1,NULL,NULL,yylineno);$$->array_size[0]=$3;$$->array_size[1]=$6;}//二维数组
        ;
//FuncDec: 函数头
FuncDec: ID LP VarList RP {$$=mknode(FUNC_DEC,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
        |ID LP RP         {$$=mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
        ;  
//VarList: 形参列表
VarList: ParamDec                {$$=mknode(PARAM_LIST,$1,NULL,NULL,yylineno);}
        | ParamDec COMMA VarList {$$=mknode(PARAM_LIST,$1,$3,NULL,yylineno);}
        ;
//ParamDec: 一个形参的定义
ParamDec: Specifier VarDec {$$=mknode(PARAM_DEC,$1,$2,NULL,yylineno);}
        ;
//CompSt: 函数体、由花括号括起来的语句块, 即复合语句
/*
CompSt: LC StmList RC {$$=mknode(COMP_STM,$2,NULL,NULL,yylineno);}
        ;*/
CompSt: LC DefList StmList RC {$$=mknode(COMP_STM,$2,$3,NULL,yylineno);}
        ;        
//StmList: 语句列表
StmList: {$$=NULL;}  
        | Stmt StmList {$$=mknode(STM_LIST,$1,$2,NULL,yylineno);}
        ;
//Stmt: 一条语句
/*
Stmt: Def   {$$=$1;}
        | Exp SEMI                        {$$=mknode(EXP_STMT,$1,NULL,NULL,yylineno);}
        | CompSt                      {$$=$1;}//复合语句结点直接最为语句结点，不再生成新的结点
        | RETURN Exp SEMI             {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}
        | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);}
        | IF LP Exp RP Stmt ELSE Stmt {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
        | WHILE LP Exp RP Stmt        {$$=mknode(WHILE,$3,$5,NULL,yylineno);}
        |error SEMI               {$$=NULL;}
        ;
*/
Stmt: Exp SEMI                        {$$=mknode(EXP_STMT,$1,NULL,NULL,yylineno);}
        | CompSt                      {$$=$1;}//复合语句结点直接最为语句结点，不再生成新的结点
        | RETURN Exp SEMI             {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}
        | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);}
        | IF LP Exp RP Stmt ELSE Stmt {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
        | WHILE LP Exp RP Stmt        {$$=mknode(WHILE,$3,$5,NULL,yylineno);}
        ;
//DefList: 变量定义列表，删除了；又加了回来
DefList: {$$=NULL;}
        | Def DefList {$$=mknode(DEF_LIST,$1,$2,NULL,yylineno);}
        ;
//Def: 一条变量定义
Def: Specifier DecList SEMI {$$=mknode(VAR_DEF,$1,$2,NULL,yylineno);}
        |error SEMI               {$$=NULL;}
        ;

DecList: Dec                {$$=mknode(DEC_LIST,$1,NULL,NULL,yylineno);}
        | Dec COMMA DecList {$$=mknode(DEC_LIST,$1,$3,NULL,yylineno);}
        ;
Dec: VarDec {$$=$1;}
        | VarDec ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
        ;
//Exp: 一个表达式
Exp: Exp ASSIGNOP Exp   {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}//$$结点type_id空置未用，正好存放运算符
        | Exp AND Exp   {$$=mknode(AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"AND");}
        | Exp OR Exp    {$$=mknode(OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"OR");}
        | Exp RELOP Exp {$$=mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}  //词法分析关系运算符号自身值保存在$2中
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
//Args: 实参列表
Args: Exp COMMA Args    {$$=mknode(ARGS,$1,$3,NULL,yylineno);}
        | Exp           {$$=mknode(ARGS,$1,NULL,NULL,yylineno);}
        ;
       
%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (!yyin) return 1;//return后填了1
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
