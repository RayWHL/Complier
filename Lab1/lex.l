%{
#include "parser.tab.h"
#include "string.h"
#include "def.h"
#include "stdio.h"
int yycolumn=1;
int modle=1;    //是否显示词法分析结果
#define YY_USER_ACTION yylloc.first_line=yylloc.last_line=yylineno; yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
typedef union{
    int type_int;
    int type_float;
    int type_double;
    int type_char;
    char type_id[32];
    struct node *pr;
}YYLVAL;
#define YYSTYPE YYLVAL

%}
%option yylineno

id [A-Za-z][A-Za-z0-9]*
int [0-9]+
float ([0-9]*\.[0-9]+)|([0-9]+\.)
double ([0-9]*\.[0-9]+)|([0-9]+\.)
char '.'|[0-9]+

%%
{int}       {if(modle){printf("(%s, INT)\n", yytext);}
            yylval.type_int=atoi(yytext);return INT;}
{float}     {if(modle){printf("(%s, FLOAT)\n", yytext);}
            yylval.type_float=atof(yytext);return FLOAT;}
{double}    {if(modle){printf("(%s, DOUBLE)\n", yytext);}
            yylval.type_double=atof(yytext);return DOUBLE;}//{yylval.type_double=atod(yytext);return DOUBLE;}//有atod函数吗？
{char}      {if(modle){printf("(%s, CHAR)\n", yytext);}
            yylval.type_char=yytext[1];return CHAR;}//有问题
"int"       {if(modle){printf("(%s, TYPE)\n", yytext);}
            strcpy(yylval.type_id, yytext);return TYPE;}
"float"     {if(modle){printf("(%s, TYPE)\n", yytext);}
            strcpy(yylval.type_id, yytext);return TYPE;}
"double"    {if(modle){printf("(%s, TYPE)\n", yytext);}
            strcpy(yylval.type_id, yytext);return TYPE;}
"char"      {if(modle){printf("(%s, TYPE)\n", yytext);}
            strcpy(yylval.type_id, yytext);return TYPE;}
"struct"    {if(modle){printf("(%s, TYPE)\n", yytext);}
            strcpy(yylval.type_id, yytext);return STRUCT;}

"return"    {if(modle){printf("(%s, RETURN)\n", yytext);}
            return RETURN;}
"if"        {if(modle){printf("(%s, IF)\n", yytext);}
            return IF;}
"else"      {if(modle){printf("(%s, ELSE)\n", yytext);}
            return ELSE;}
"while"     {if(modle){printf("(%s, WHILE)\n", yytext);}
            return WHILE;}

{id}        {if(modle){printf("(%s, ID)\n", yytext);}
            strcpy(yylval.type_id, yytext);return ID;}//由于关键词的形式也符合标识符的规则，所以把关键字的处理全部放在标识符的前面
";"         {if(modle){printf("(%s, SEMI)\n", yytext);}
            return SEMI;}
","         {if(modle){printf("(%s, COMMA)\n", yytext);}
            return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" {if(modle){printf("(%s, RELOP)\n", yytext);}
            strcpy(yylval.type_id, yytext);return RELOP;}
"="         {if(modle){printf("(%s, ASSIGNOP)\n", yytext);}
            return ASSIGNOP;}
"++"        {if(modle){printf("(%s, INC)\n", yytext);}
            return INC;}
"+="        {if(modle){printf("(%s, COMP_PLUS)\n", yytext);}
            return COMP_PLUS;}
"+"         {if(modle){printf("(%s, PLUS)\n", yytext);}
            return PLUS;}
"--"        {if(modle){printf("(%s, DEC)\n", yytext);}
            return DEC;}
"-="        {if(modle){printf("(%s, COMP_MINUS)\n", yytext);}
            return COMP_MINUS;}
"-"         {if(modle){printf("(%s, MINUS)\n", yytext);}
            return MINUS;}
"*"         {if(modle){printf("(%s, STAR)\n", yytext);}
            return STAR;}
"/"         {if(modle){printf("(%s, DIV)\n", yytext);}
            return DIV;}//为什么这个可以放在"//"前面？？ "//"还可以放在.后面？？？
"&&"        {if(modle){printf("(%s, AND)\n", yytext);}
            return AND;}
"||"        {if(modle){printf("(%s, OR)\n", yytext);}
            return OR;}
"!"         {if(modle){printf("(%s, NOT)\n", yytext);}
            return NOT;}
"("         {if(modle){printf("(%s, LP)\n", yytext);}
            return LP;}
")"         {if(modle){printf("(%s, RP)\n", yytext);}
            return RP;}
"["         {if(modle){printf("(%s, LB)\n", yytext);}
            return LB;}
"]"         {if(modle){printf("(%s, RB)\n", yytext);}
            return RB;}
"{"         {if(modle){printf("(%s, LC)\n", yytext);}
            return LC;}
"}"         {if(modle){printf("(%s, RC)\n", yytext);}
            return RC;}
[\n]        {yycolumn=1;}
[ \r\t]     {} 
(\/\/).*    {}//匹配注释//
\/\*.*?(\n)*\*\/ {}//匹配注释/**/ 有问题，贪婪方法无效？？？
.           {printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}
%%

/* 和bison联用时，不需要这部分
void main()
{
yylex();
return 0;
}
*/

int yywrap()
{
return 1;
}

