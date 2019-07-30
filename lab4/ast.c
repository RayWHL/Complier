#include "def.h"
// #define DISPLAY
struct node * mknode(int kind,struct node *first,struct node *second, struct node *third,int pos) {
    struct node *T=(struct node *)malloc(sizeof(struct node));
    T->kind=kind;
    T->ptr[0]=first;
    T->ptr[1]=second;
    T->ptr[2]=third;
    T->pos=pos;
    return T;
}

int place;

//#ifdef DISPLAY
void display(struct node *T,int indent){//�Գ����﷨�����ȸ�����,indentΪ��ʶ��
    int i=1;
    struct node *T0;
    if (T){
	    switch (T->kind){
            case EXT_DEF_LIST:  
                display(T->ptr[0],indent);//��ʾ���ⲿ�����б��еĵ�һ��
                display(T->ptr[1],indent);//��ʾ���ⲿ�����б��е������ⲿ����
                break;
            case EXT_VAR_DEF:   
                printf("%*c�ⲿ�������壺\n",indent,' ');
                display(T->ptr[0],indent+3);//��ʾ�ⲿ��������
                printf("%*c��������\n",indent+3,' ');
                display(T->ptr[1],indent+6);//��ʾ�����б�
                break;
            case STRUCT_SPECIFIER:
                // printf("%*c�ṹ���������� \n",indent,' ');
                display(T->ptr[0],indent+3);
                break;
            case STRUCT_TYPE_DEF:
                printf("%*c�ṹ�����ͣ� \n",indent,' ');
                display(T->ptr[0],indent+3);
                break;
            case STRUCT_VAR_DEF:
                printf("%*c�ṹ������� \n",indent,' ');
                display(T->ptr[0],indent+3);
                break;
            case STRUCT_DEF:
                printf("%*c�ṹ�壺 \n",indent,' ');
                display(T->ptr[0],indent+3);
                display(T->ptr[1],indent+3);
                break;
            case TYPE:          
                printf("%*c���ͣ� %s\n",indent,' ',T->type_id);
                break;
            case EXT_DEC_LIST:  
                display(T->ptr[0],indent);//������ʾ�ⲿ��������
                display(T->ptr[1],indent);//����������ͬ�ģ�����ʾ�﷨���˴��������Ժ����ƴ���ϲ�
                break;
            case FUNC_DEF:      
                printf("%*c�������壺\n",indent,' ');
                display(T->ptr[0],indent+3);//��ʾ������������
                display(T->ptr[1],indent+3);//��ʾ�������Ͳ���
                display(T->ptr[2],indent+3);//��ʾ������
                break;
            case FUNC_DEC:      
                printf("%*c��������%s\n",indent,' ',T->type_id);
                if (T->ptr[0]) {
                    printf("%*c�����βΣ�\n",indent,' ');
                    display(T->ptr[0],indent+3);//��ʾ���������б�
                }
                else printf("%*c�޲κ���\n",indent+3,' ');
                break;
            case PARAM_LIST:    
                display(T->ptr[0],indent);//������ʾȫ���������ͺ����ƣ�
                display(T->ptr[1],indent);
                break;
            case PARAM_DEC:     
                if(T->ptr[0]->type==INT)
                    printf("%*c���ͣ�%s, ��������%s\n", indent,' ',"int",T->ptr[1]->type_id);
                if(T->ptr[0]->type==FLOAT)
                    printf("%*c���ͣ�%s, ��������%s\n", indent,' ',"float",T->ptr[1]->type_id);
                if(T->ptr[0]->type==CHAR)
                    printf("%*c���ͣ�%s, ��������%s\n", indent,' ',"char",T->ptr[1]->type_id);
                if(T->ptr[0]->type==DOUBLE)
                    printf("%*c���ͣ�%s, ��������%s\n", indent,' ',"double",T->ptr[1]->type_id);
                break;
            case EXP_STMT:      
                printf("%*c���ʽ��䣺\n",indent,' ');
                display(T->ptr[0],indent+3);
                break;
            case RETURN:        
                printf("%*c������䣺\n",indent,' ');
                display(T->ptr[0],indent+3);
                break;
            case COMP_STM:     
                printf("%*c������䣺\n",indent,' ');
                printf("%*c�������ı������壺\n",indent+3,' ');
                display(T->ptr[0],indent+6);//��ʾ���岿��
                printf("%*c����������䲿�֣�\n",indent+3,' ');
                display(T->ptr[1],indent+6);//��ʾ��䲿��
                break;
            case STM_LIST:      
                display(T->ptr[0],indent);//��ʾ��һ�����
                display(T->ptr[1],indent);//��ʾʣ�����
                break;
            case WHILE:         
                printf("%*cѭ����䣺\n",indent,' ');
                printf("%*cѭ��������\n",indent+3,' ');
                display(T->ptr[0],indent+6);//��ʾѭ������
                printf("%*cѭ���壺\n",indent+3,' ');
                display(T->ptr[1],indent+6);//��ʾѭ����
                break;
            case IF_THEN:       
                printf("%*c�������(IF_THEN)��\n",indent,' ');
                printf("%*c������\n",indent+3,' ');
                display(T->ptr[0],indent+6);//��ʾ����
                printf("%*cIF�Ӿ䣺\n",indent+3,' ');
                display(T->ptr[1],indent+6);//��ʾif�Ӿ�
                break;
            case IF_THEN_ELSE:  
                printf("%*c�������(IF_THEN_ELSE)��\n",indent,' ');
                printf("%*c������\n",indent+3,' ');
                display(T->ptr[0],indent+6);//��ʾ����
                printf("%*cIF�Ӿ䣺\n",indent+3,' ');
                display(T->ptr[1],indent+6);//��ʾif�Ӿ�
                printf("%*cELSE�Ӿ䣺\n",indent+3,' ');
                display(T->ptr[2],indent+6);//��ʾelse�Ӿ�
                break;
            case DEF_LIST:      
                display(T->ptr[0],indent);//��ʾ�þֲ����������б��еĵ�һ��
                display(T->ptr[1],indent);//��ʾ�����ֲ���������
                break;
            case VAR_DEF:       
                printf("%*cLOCAL VAR_NAME��\n",indent,' ');
                display(T->ptr[0],indent+3);//��ʾ��������
                display(T->ptr[1],indent+3);//��ʾ�ö����ȫ��������
                break;
            case DEC_LIST:      
                printf("%*cVAR_NAME��\n",indent,' ');
                T0=T;
                while (T0) {
                    if (T0->ptr[0]->kind==ID)
                        printf("%*c %s\n",indent+3,' ',T0->ptr[0]->type_id);
                    //����
                    else if(T0->ptr[0]->kind==ARRAY_DEF)
                         display(T0->ptr[0],indent+3);
                    else if (T0->ptr[0]->kind==ASSIGNOP){
                        printf("%*c %s ASSIGNOP\n ",indent+3,' ',T0->ptr[0]->ptr[0]->type_id);
                        //��ʾ��ʼ�����ʽ
                        display(T0->ptr[0]->ptr[1],indent+strlen(T0->ptr[0]->ptr[0]->type_id)+4);       
                    }
                    T0=T0->ptr[1];
                }
                break;
            case ID:	        
                printf("%*cID�� %s\n",indent,' ',T->type_id);
                break;
            case TAG:
                // printf("%*cTAG�� \n",indent,' ');
                printf("%*c�ṹ��������� %s\n",indent,' ',T->type_id);
                break;
            case ARRAY_DEF:
                printf("%*cһά���鶨�壺 \n",indent,' ');
                display(T->ptr[0],indent+3);
                printf("%*cSIZE: %d\n",indent,' ',T->array_size[0]);
                break;
            case TWO_ARRAY_DEF:
                printf("%*c��ά���鶨�壺 \n",indent,' ');
                display(T->ptr[0],indent+3);
                printf("%*cSIZE: %d %d\n",indent,' ',T->array_size[0],T->array_size[1]);
                break;
            case INT:	        
                printf("%*cINT��%d\n",indent,' ',T->type_int);
                break;
            case FLOAT:	        
                printf("%*cFLAOT��%f\n",indent,' ',T->type_float);
                break;
            case CHAR:
                printf("%*cCHAR��%c\n",indent,' ',T->type_char);
                break;
            case ASSIGNOP:
            case AND:
            case OR:
            case RELOP:
            case COMP_PLUS:
            case COMP_MINUS:
            case PLUS:
            case MINUS:
            case STAR:
            case DIV:
                printf("%*c%s\n",indent,' ',T->type_id);
                display(T->ptr[0],indent+3);
                display(T->ptr[1],indent+3);
                break;
            case INC:
            case DEC:
            case NOT:
            case UMINUS:    
                printf("%*c%s\n",indent,' ',T->type_id);
                display(T->ptr[0],indent+3);
                break;
            case FUNC_CALL: 
                printf("%*c�������ã�\n",indent,' ');
                printf("%*c��������%s\n",indent+3,' ',T->type_id);
                display(T->ptr[0],indent+3);
                break;
            case ARGS:      
                i=1;
                while (T) {//ARGS��ʾʵ�ʲ������ʽ���н�㣬���һ������Ϊ��һ��ʵ�ʲ������ʽ���ڶ�������Ϊʣ�µġ�
                    struct node *T0=T->ptr[0];
                    printf("%*c��%d��ʵ�ʲ������ʽ��\n",indent,' ',i++);
                    display(T0,indent+3);
                    T=T->ptr[1];
                }
                //printf("%*c��%d��ʵ�ʲ������ʽ��\n",indent,' ',i);
                //display(T,indent+3);
                printf("\n");
                break;
        }
    }
}
//#endif

char *strcat0(char *s1,char *s2) {//ƴ��s1��s2
    static char result[10];
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}

char *newAlias() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("v",s);
}

char *newLabel() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("label",s);
}

char *newTemp(){
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("temp",s);
}

//codenode�ṹ����def.h�ж��壬��������ַTAC������
//����һ��TAC����Ľ����ɵ�˫��ѭ����������ͷָ��
struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=op;
    h->opn1=opn1;
    h->opn2=opn2;
    h->result=result;
    h->next=h->prior=h;
    return h;
}

//����һ�������䣬����ͷָ��
struct codenode *genLabel(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=LABEL;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//����GOTO��䣬����ͷָ��
struct codenode *genGoto(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=GOTO;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//�ϲ�����м�����˫��ѭ��������β����
struct codenode *merge(int num,...){
    struct codenode *h1,*h2,*p,*t1,*t2;
    va_list ap;
    va_start(ap,num);
    h1=va_arg(ap,struct codenode *);
    while (--num>0) {
        h2=va_arg(ap,struct codenode *);
        if (h1==NULL) h1=h2;
        else if (h2){
            t1=h1->prior;
            t2=h2->prior;
            t1->next=h2;
            t2->next=h1;
            h1->prior=t2;
            h2->prior=t1;
        }
    }
    va_end(ap);
    return h1;
}

//����м����
void prnIR(struct codenode *head){
    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    do{
        //printf("%d %d %s\n",INT,h->opn1.kind,h->result.id);
        if (h->opn1.kind==INT)
            sprintf(opnstr1,"#%d",h->opn1.const_int);
        if (h->opn1.kind==FLOAT)
            sprintf(opnstr1,"#%f",h->opn1.const_float);
        if (h->opn1.kind==CHAR)
            sprintf(opnstr1,"#%d",h->opn1.const_char);
        if (h->opn1.kind==ID)
            sprintf(opnstr1,"%s",h->opn1.id);
        if (h->opn2.kind==INT)
            sprintf(opnstr2,"#%d",h->opn2.const_int);
        if (h->opn2.kind==FLOAT)
            sprintf(opnstr2,"#%f",h->opn2.const_float);
        if (h->opn2.kind==CHAR)
            sprintf(opnstr2,"#%d",h->opn2.const_char);
        if (h->opn2.kind==ID)
            sprintf(opnstr2,"%s",h->opn2.id);
        sprintf(resultstr,"%s",h->result.id);
        switch (h->op) {
            case ASSIGNOP:  
                printf("  %s := %s\n",resultstr,opnstr1);
                break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV: 
                printf(" %s := %s %c %s\n",resultstr,opnstr1, \
                h->op==PLUS ? '+' : h->op==MINUS ? '-' : h->op==STAR ? '*' : '\\',opnstr2);
                break;
            case FUNCTION: 
                printf("\nFUNCTION %s :\n",h->result.id);
                break;
            case PARAM:   
                printf("  PARAM %s\n",h->result.id);
                break;
            case LABEL:    
                printf("LABEL %s :\n",h->result.id);
                break;
            case GOTO:
                printf("  GOTO %s\n",h->result.id);
                break;
            case JLE:    
                printf("  IF %s <= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                break;
            case JLT:    
                printf("  IF %s < %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                break;
            case JGE:      
                printf("  IF %s >= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                break;
            case JGT:      
                printf("  IF %s > %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                break;
            case EQ:       
                printf("  IF %s == %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                break;
            case NEQ:      
                printf("  IF %s != %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                break;
            case ARG://��ʾʵ��      
                printf("  ARG %s\n",h->result.id);
                break;
            case CALL://���ú���     
                printf("  %s := CALL %s\n",resultstr, opnstr1);
                break;
            case RETURN:   
                if (h->result.kind)
                    printf("  RETURN %s\n",resultstr);
                else
                    printf("  RETURN\n");
                break;
            //case INC:
                    
                //break;
            default:
                printf("there\n");
        }
        h=h->next;
    } while (h!=head);
}

void semantic_error(int line,char *msg1,char *msg2){//�������ֻ�ռ�������Ϣ�������һ����ʾ
    printf("��%d��,%s %s\n",line,msg1,msg2);
}
//THERE
//symbolTable��def.h�ж��壬������ű�
void prn_symbol(){ //��ʾ���ű�
    int i=0;
    printf(" %6s%7s%8s%8s%6s%7s\n","������","�� ��","�� ��","�� ��","���","ƫ����");
    printf("%d\n",symbolTable.index);
    for(i=0;i<symbolTable.index;i++){
        if(symbolTable.symbols[i].type==INT)
            printf("%6s %6s %6d  %6s %4c %6d\n",symbolTable.symbols[i].name,\
                symbolTable.symbols[i].alias,symbolTable.symbols[i].level,\
                "int",\
                symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
        else if(symbolTable.symbols[i].type==FLOAT)
            printf("%6s %6s %6d  %6s %4c %6d\n",symbolTable.symbols[i].name,\
                symbolTable.symbols[i].alias,symbolTable.symbols[i].level,\
                "float",\
                symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
        else if(symbolTable.symbols[i].type==CHAR)
            printf("%6s %6s %6d  %6s %4c %6d\n",symbolTable.symbols[i].name,\
                symbolTable.symbols[i].alias,symbolTable.symbols[i].level,\
                "char",\
                symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
        else if(symbolTable.symbols[i].type==ARRAY_DEF)
            printf("ARRAY_DEF");
        else printf("there %s\n",symbolTable.symbols[i].name); //�ṹ
    }
}

int searchSymbolTable(char *name) {
    int i;
    for(i=symbolTable.index-1;i>=0;i--)
        if (!strcmp(symbolTable.symbols[i].name, name))  return i;
    return -1;
}

//����ű�
int fillSymbolTable(char *name,char *alias,int level,int type,char flag,int offset) {
    //���ȸ���name����ű������ظ����� �ظ����巵��-1
    int i;
    /*���Ų��أ������ⲿ��������ǰ�к������壬
    ���β������ڷ��ű��У���ʱ���ⲿ������ǰ�������β������������*/
    for(i=symbolTable.index-1;symbolTable.symbols[i].level==level||(level==0 && i>=0);i--) {
        if (level==0 && symbolTable.symbols[i].level==1) continue;  //�ⲿ�������ββ��رȽ�����
        if (!strcmp(symbolTable.symbols[i].name, name))  return -1;
    }
    //��д���ű�����
    strcpy(symbolTable.symbols[symbolTable.index].name,name);
    strcpy(symbolTable.symbols[symbolTable.index].alias,alias);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //���ص��Ƿ����ڷ��ű��е�λ����ţ��м��������ʱ�������ȡ�����ű���
}

//��д��ʱ���������ű�������ʱ�����ڷ��ű��е�λ��
int fill_Temp(char *name,int level,int type,char flag,int offset) {
    strcpy(symbolTable.symbols[symbolTable.index].name,"");
    strcpy(symbolTable.symbols[symbolTable.index].alias,name);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //���ص�����ʱ�����ڷ��ű��е�λ�����
}

int LEV=0;      //���
int func_size;  //1�������Ļ��¼��С

void ext_var_list(struct node *T){//��������б�
    int rtn,num=1;
    switch (T->kind){
        case EXT_DEC_LIST:
            T->ptr[0]->type=T->type;              //�������������´��ݱ������
            T->ptr[0]->offset=T->offset;          //�ⲿ������ƫ�������´���
            T->ptr[1]->type=T->type;              //�������������´��ݱ������
            T->ptr[1]->offset=T->offset+T->width; //�ⲿ������ƫ�������´���
            T->ptr[1]->width=T->width;
            ext_var_list(T->ptr[0]);
            ext_var_list(T->ptr[1]);
            T->num=T->ptr[1]->num+1;
            break;
        case ID:
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'V',T->offset);  //���һ��������
            if (rtn==-1)
                semantic_error(T->pos,T->type_id, "�����ظ�����");
            else T->place=rtn;
            T->num=1;
            break;
        case ARRAY_DEF :    //THERE
            //T->ptr[0]->type=T->type;              //�������������´��ݱ������
            //T->ptr[0]->offset=T->offset;          //�ⲿ������ƫ�������´���
            //T->ptr[1]->type=T->type;              //�������������´��ݱ������
            //T->ptr[1]->offset=T->offset+T->width; //�ⲿ������ƫ�������´���
            //T->ptr[1]->width=T->width;
            rtn=fillSymbolTable(T->ptr[0]->type_id,newAlias(),LEV,T->type,'V',T->offset);  //���һ��������
            if (rtn==-1)
                semantic_error(T->pos,T->type_id, "�����ظ�����");
            else T->place=rtn;
            T->num= T->array_size[0];
            break;
    }
}

int match_param(int i,struct node *T){
    int j,num=symbolTable.symbols[i].paramnum;
    int type1,type2;
    struct node *pre=T;
    if (num==0 && T==NULL) return 1;
    //printf("1  %d\n",num);
    for (j=0;j<num;j++) {
        if (!T){
            semantic_error(pre->pos,"", "�������ò���̫��");
            return 0;
        }
        type1=symbolTable.symbols[i+j].type;  //�β�����
        type2=T->ptr[0]->type;
        if (type1!=type2){
            semantic_error(T->pos,"", "�������Ͳ�ƥ��");
            return 0;
        }
        //pre=T;
        T=T->ptr[1];
    }
    if (T){ //num�������Ѿ�ƥ���꣬����ʵ�α��ʽ
        semantic_error(T->pos,"", "�������ò���̫��");
        return 0;
    }
    return 1;
}

void boolExp(struct node *T){//�������ʽ���ο�����[2]p84��˼��
    struct opn opn1,opn2,result;
    int op;
    int rtn;
    if (T){
        switch (T->kind) {
            case INT:   
                if (T->type_int!=0)
                    T->code=genGoto(T->Etrue);
                else T->code=genGoto(T->Efalse);
                T->width=0;
                break;
            case FLOAT: 
                if (T->type_float!=0.0)
                    T->code=genGoto(T->Etrue);
                else T->code=genGoto(T->Efalse);
                T->width=0;
                break;
            case CHAR:
                if (T->type_char!=0)
                    T->code=genGoto(T->Etrue);
                else T->code=genGoto(T->Efalse);
                T->width=0;
                break;
            case ID:    
                //����ű���÷��ű��е�λ�ã�������type
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1)
                    semantic_error(T->pos,T->type_id, "����δ����");
                if (symbolTable.symbols[rtn].flag=='F')
                    semantic_error(T->pos,T->type_id, "�Ǻ����������Ͳ�ƥ��");
                else {
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[rtn].alias);
                    opn1.offset=symbolTable.symbols[rtn].offset;
                    opn2.kind=INT; opn2.const_int=0;
                    result.kind=ID; strcpy(result.id,T->Etrue);
                    T->code=genIR(NEQ,opn1,opn2,result);
                    T->code=merge(2,T->code,genGoto(T->Efalse));
                }
                T->width=0;
                break;
            case RELOP: //�����ϵ������ʽ,2�������������������ʽ����
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                Exp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                Exp(T->ptr[1]);
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[T->ptr[1]->place].alias);
                opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                result.kind=ID; strcpy(result.id,T->Etrue);
                if (strcmp(T->type_id,"<")==0)
                    op=JLT;
                else if (strcmp(T->type_id,"<=")==0)
                    op=JLE;
                else if (strcmp(T->type_id,">")==0)
                    op=JGT;
                else if (strcmp(T->type_id,">=")==0)
                    op=JGE;
                else if (strcmp(T->type_id,"==")==0)
                    op=EQ;
                else if (strcmp(T->type_id,"!=")==0)
                    op=NEQ;
                T->code=genIR(op,opn1,opn2,result);
                T->code=merge(4,T->ptr[0]->code,T->ptr[1]->code,T->code,genGoto(T->Efalse));
                break;
            case AND:
            case OR:
                if (T->kind==AND) {
                    strcpy(T->ptr[0]->Etrue,newLabel());
                    strcpy(T->ptr[0]->Efalse,T->Efalse);
                }
                else {
                    strcpy(T->ptr[0]->Etrue,T->Etrue);
                    strcpy(T->ptr[0]->Efalse,newLabel());
                }
                strcpy(T->ptr[1]->Etrue,T->Etrue);
                strcpy(T->ptr[1]->Efalse,T->Efalse);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                boolExp(T->ptr[1]);
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                if (T->kind==AND)
                    T->code=merge(3,T->ptr[0]->code,genLabel(T->ptr[0]->Etrue),T->ptr[1]->code);
                else
                    T->code=merge(3,T->ptr[0]->code,genLabel(T->ptr[0]->Efalse),T->ptr[1]->code);
                break;
            case NOT:   
                strcpy(T->ptr[0]->Etrue,T->Efalse);
                strcpy(T->ptr[0]->Efalse,T->Etrue);
                boolExp(T->ptr[0]);
                T->code=T->ptr[0]->code;
                break;
        }
    }
}

void Exp(struct node *T)
{//����������ʽ���ο�����[2]p82��˼��
    int rtn,num,width;
    struct node *T0;
    struct opn opn1,opn2,result;
    if (T){
        switch (T->kind) { 
            case ID:    
                //����ű���÷��ű��е�λ�ã�������type
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1)
                    semantic_error(T->pos,T->type_id, "����δ����");
                if (symbolTable.symbols[rtn].flag=='F')
                    semantic_error(T->pos,T->type_id, "�Ǻ����������Ͳ�ƥ��");
                else {
                    T->place=rtn;       //��㱣������ڷ��ű��е�λ��
                    T->code=NULL;       //��ʶ������Ҫ����TAC
                    T->type=symbolTable.symbols[rtn].type;
                    T->offset=symbolTable.symbols[rtn].offset;
                    T->width=0;   //δ��ʹ���µ�Ԫ
                }
                break;
            case INT:   
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);//Ϊ����������һ����ʱ����
                T->type=INT;
                opn1.kind=INT;opn1.const_int=T->type_int;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4;
                break;
            case FLOAT:
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);//Ϊ���㳣������һ����ʱ����
                T->type=FLOAT;
                opn1.kind=FLOAT; opn1.const_float=T->type_float;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4;
                break;
            case CHAR:
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);//Ϊ�ַ���������һ����ʱ����
                T->type=CHAR;
                opn1.kind=CHAR;opn1.const_char=T->type_char;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=1;
                break;
            case ARRAY_DEF://δд���� //THERE
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);//Ϊ����������һ����ʱ����
                T->type=ARRAY_DEF;
                // opn1.kind=INT;opn1.const_int=T->type_int;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                // T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4*T->type_id[0];
                Exp(T->ptr[0]);
                break;
            case ASSIGNOP:
                if (T->ptr[0]->kind!=ID){
                    semantic_error(T->pos,"", "��ֵ�����Ҫ��ֵ");
                }
                else {
                    Exp(T->ptr[0]);   //������ֵ�����н�Ϊ����
                    T->ptr[1]->offset=T->offset;
                    Exp(T->ptr[1]);
                    //if����
                    if(T->ptr[0]->type!=FLOAT && T->ptr[1]->type==FLOAT)
                    {
                        semantic_error(T->pos,"", "��ֵ������Ͳ�ƥ��");
                        break;
                    }
                    if(T->ptr[0]->type==CHAR && T->ptr[1]->type!=CHAR)
                    {
                        semantic_error(T->pos,"", "��ֵ������Ͳ�ƥ��");
                        break;
                    }
                    /*
                    if(strcmp(T->ptr[1]->type_id,"PRE_DEC")==0 )
                    {
                        //�滻Ϊ�����m++ -> m=m+1
                        opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        opn1.type=T->ptr[1]->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[1]->ptr[0]->place].offset;
                        opn2.kind=INT; opn2.const_int=1;
                        //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        result.type=T->ptr[1]->type;result.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        T->code=merge(2,T->code,genIR(PLUS,opn1,opn2,result));
                    }
                    if(strcmp(T->ptr[1]->type_id,"PRE_INC")==0 )
                    {
                        opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        opn1.type=T->ptr[1]->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[1]->ptr[0]->place].offset;
                        opn2.kind=INT; opn2.const_int=1;
                        //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        result.type=T->ptr[1]->type;result.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        T->code=merge(2,T->code,genIR(MINUS,opn1,opn2,result));
                    }
                    */
                    T->type=T->ptr[0]->type;
                    T->width=T->ptr[1]->width;
                    T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code);
                    opn1.kind=ID;   strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->place].alias);//��ֵһ���Ǹ���������ʱ����
                    opn1.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));
                    /*
                    if(strcmp(T->ptr[1]->type_id,"PRE_DEC")==0 )
                    {
                        opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        opn1.type=T->ptr[1]->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[1]->ptr[0]->place].offset;
                        opn2.kind=INT; opn2.const_int=1;
                        //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        result.type=T->ptr[1]->type;result.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        T->code=merge(2,T->code,genIR(MINUS,opn1,opn2,result));
                    }
                    if(strcmp(T->ptr[1]->type_id,"PRE_INC")==0 )
                    {
                        opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        opn1.type=T->ptr[1]->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[1]->ptr[0]->place].offset;
                        opn2.kind=INT; opn2.const_int=1;
                        //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[1]->ptr[0]->place].alias);
                        result.type=T->ptr[1]->type;result.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        T->code=merge(2,T->code,genIR(PLUS,opn1,opn2,result));
                    }*/
                }
                break;
            case AND:   //���������ʽ��ʽ���㲼��ֵ��δд��
            case OR:    //���������ʽ��ʽ���㲼��ֵ��δд��
            case RELOP: //���������ʽ��ʽ���㲼��ֵ��δд��
                T->type=INT;
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                Exp(T->ptr[0]);
                Exp(T->ptr[1]);
                break;
            case COMP_PLUS:
            case COMP_MINUS:
                if (T->ptr[0]->kind != ID) {
				semantic_error(T->pos, "", "��ֵ�����Ҫ��ֵ");
			}
			else {
				T->ptr[0]->offset = T->offset;
				Exp(T->ptr[0]);   //������ֵ�����н�Ϊ����
				T->ptr[1]->offset = T->offset + T->ptr[0]->width;
				Exp(T->ptr[1]);
				if (T->ptr[0]->type == FLOAT || T->ptr[1]->type == FLOAT)
					T->type = FLOAT, T->width = T->ptr[0]->width + T->ptr[1]->width + 4;
				else if (T->ptr[0]->type == INT || T->ptr[1]->type == INT)
					T->type = INT, T->width = T->ptr[0]->width + T->ptr[1]->width + 2;
				else if (T->ptr[0]->type == CHAR || T->ptr[1]->type == CHAR)
					T->type = CHAR, T->width = T->ptr[0]->width + T->ptr[1]->width + 1;
				T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset + T->ptr[0]->width + T->ptr[1]->width);
				opn1.kind = ID; strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
				opn1.type = T->ptr[0]->type; opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;
				opn2.kind = ID; strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
				opn2.type = T->ptr[1]->type; opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;
				result.kind = ID; strcpy(result.id, symbolTable.symbols[T->place].alias);
				result.type = T->ptr[0]->type; result.offset = symbolTable.symbols[T->place].offset;
				if (T->kind == COMP_PLUS)
					T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(PLUS, opn1, opn2, result));
				else if (T->kind == COMP_MINUS)
					T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(MINUS, opn1, opn2, result));
				if (T->type == INT)
					T->width = T->ptr[0]->width + T->ptr[1]->width + 4;
				else if (T->type == FLOAT)
					T->width = T->ptr[0]->width + T->ptr[1]->width + 8;
				else if (T->type == CHAR)
					T->width = T->ptr[0]->width + T->ptr[1]->width + 1;

				opn1.kind = ID;   strcpy(opn1.id, symbolTable.symbols[T->place].alias);//��ֵһ���Ǹ���������ʱ����
				opn1.offset = symbolTable.symbols[T->place].offset;
				result.kind = ID; strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
				result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
				T->code = merge(2, T->code, genIR(ASSIGNOP, opn1, opn2, result));
			}
			break;

            case PLUS:
            case MINUS:
            case STAR:
            case DIV:   
                T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                Exp(T->ptr[1]);
                //�ж�T->ptr[0]��T->ptr[1]�����Ƿ���ȷ�����ܸ�����������ɲ�ͬ��ʽ�Ĵ��룬��T��type��ֵ
                //������������Լ��㣬û�п��Ǵ��������
                if (T->ptr[0]->type==FLOAT || T->ptr[1]->type==FLOAT)
                    T->type=FLOAT,T->width=T->ptr[0]->width+T->ptr[1]->width+4;
                else if(T->ptr[0]->type==INT || T->ptr[1]->type==INT)
                    T->type=INT,T->width=T->ptr[0]->width+T->ptr[1]->width+2;
                else if(T->ptr[0]->type==CHAR || T->ptr[1]->type==CHAR)
                    T->type=CHAR,T->width=T->ptr[0]->width+T->ptr[1]->width+1;

                //��0������
                if(T->ptr[1]->type_int==0 || T->ptr[1]->type_float==0)
                {
                    semantic_error(T->ptr[0]->pos,"", "��������Ϊ0");
                    break;
                }   
               
  
                    
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->ptr[0]->width+T->ptr[1]->width);
                        opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                        opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                        opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[T->ptr[1]->place].alias);
                        opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                        result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                        result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                        T->code=merge(3,T->ptr[0]->code,T->ptr[1]->code,genIR(T->kind,opn1,opn2,result));
                    
                if(T->type==INT)
                    T->width=T->ptr[0]->width+T->ptr[1]->width+4;
                else if(T->type==FLOAT)
                    T->width=T->ptr[0]->width+T->ptr[1]->width+8;
                else if(T->type==CHAR)
                    T->width=T->ptr[0]->width+T->ptr[1]->width+1;
                break;

            case INC:   //a++Ϊ��ֵ if����
                if (strcmp(T->ptr[0]->type_id,"PRE_INC")==0 || strcmp(T->ptr[0]->type_id,"PRE_DEC")==0)
                {
                    semantic_error(T->ptr[0]->pos,T->ptr[0]->type_id, "��ֵ����++");
                    break;
                }
                

            case DEC:  //if����
                if (strcmp(T->ptr[0]->type_id,"PRE_INC")==0 || strcmp(T->ptr[0]->type_id,"PRE_DEC")==0)
                {
                    semantic_error(T->ptr[0]->pos,T->ptr[0]->type_id, "��ֵ����--");
                    break;
                }
            case NOT:   //δд����
            case UMINUS://δд����
                T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                //�ж�T->ptr[0]��T->ptr[1]�����Ƿ���ȷ�����ܸ�����������ɲ�ͬ��ʽ�Ĵ��룬��T��type��ֵ
                //������������Լ��㣬û�п��Ǵ��������
                if (T->ptr[0]->type==FLOAT)
                    T->type=FLOAT,T->width=T->ptr[0]->width+4;
                else if(T->ptr[0]->type==INT)
                    T->type=INT,T->width=T->ptr[0]->width+4;
                else if(T->ptr[0]->type==CHAR)
                    T->type=CHAR,T->width=T->ptr[0]->width+1;
                //place���ڱ���ԭֵ
                place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->ptr[0]->width);
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->ptr[0]->width+4);
                //T->place=T->ptr[0]->offset;
                opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                // T->code=merge(3,T->ptr[0]->code,T->ptr[1]->code,genIR(T->kind,opn1,opn2,result));

                //INC�м��������
                if(T->kind==INC) 
                {
                    //���ȱ����1ǰ��ֵ
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[place].alias);
                    result.type=T->type;result.offset=symbolTable.symbols[place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));

                    //�滻Ϊ�����m++ -> m=m+1
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    opn2.kind=INT; opn2.const_int=1;
                    //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                    result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                    T->code=merge(2,T->code,genIR(PLUS,opn1,opn2,result));
                    
                   /* opn1.kind=ID;   strcpy(opn1.id,symbolTable.symbols[T->place].alias);//��ֵһ���Ǹ���������ʱ����
                    opn1.offset=symbolTable.symbols[T->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));*/
                    
                }
                if(T->kind==DEC)
                {
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[place].alias);
                    result.type=T->type;result.offset=symbolTable.symbols[place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));

                    //�滻Ϊ�����m-- -> m=m-1
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    opn2.kind=INT; opn2.const_int=1;
                    //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                     result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                    result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                    T->code=merge(2,T->code,genIR(MINUS,opn1,opn2,result));
                }
                //Ȼ����ʱ����ֵ��ֵ��ԭ����
                opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->place].alias);
                opn1.type=T->type;opn1.offset=symbolTable.symbols[T->place].offset;
                //opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                result.type=T->ptr[0]->type;result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));

                if(strcmp(T->type_id,"PRE_INC")==0 ||strcmp(T->type_id,"PRE_DEC")==0)
                    T->place=place;
                
                    //T->code=merge(2,T->ptr[0]->code,genIR(T->kind,opn1,opn2,result));
                if(T->type==INT)
                    T->width=T->ptr[0]->width+4;
                else if(T->type==FLOAT)
                    T->width=T->ptr[0]->width+8;
                else if(T->type==CHAR)
                    T->width=T->ptr[0]->width+1;
                break;
            case FUNC_CALL: //����T->type_id��������Ķ��壬���������������ʵ��̲ĵ�read��write��Ҫ��������һ��
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1){
                    semantic_error(T->pos,T->type_id, "����δ����");
                    break;
                }
                if (symbolTable.symbols[rtn].flag!='F'){
                    semantic_error(T->pos,T->type_id, "����һ������");
                    break;
                }
                T->type=symbolTable.symbols[rtn].type;
                if(T->type==INT)//��ź�������ֵ�ĵ����ֽ���
                    width=4;
                if(T->type==FLOAT)
                    width=8;
                if(T->type==CHAR)
                    width=1;    
                if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]);       //��������ʵ�α��ʽ��ֵ��������
                    T->width=T->ptr[0]->width+width; //�ۼ��ϼ���ʵ��ʹ����ʱ�����ĵ�Ԫ��
                    T->code=T->ptr[0]->code;
                }
                else {T->width=width; T->code=NULL;}
                if(strcmp(T->type_id,"write")!=0 && (strcmp(T->type_id,"read")!=0))
                    match_param(rtn,T->ptr[0]);   //�������Բ�����ƥ��
                else
                {
                    //����read write
                    if(strcmp(T->type_id,"write")==0 && T->ptr[0]==NULL)
                        semantic_error(T->pos,"", "��������");
                    if(strcmp(T->type_id,"write")==0 && T->ptr[0]!=NULL && T->ptr[0]->ptr[1]!=NULL)
                        semantic_error(T->pos,"", "��������");
                    if(strcmp(T->type_id,"read")==0 && T->ptr[0]!=NULL)
                        semantic_error(T->pos,"", "��������");
                }
                //��������б���м����
                T0=T->ptr[0];
                while (T0) {
                    result.kind=ID;  strcpy(result.id,symbolTable.symbols[T0->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T0->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ARG,opn1,opn2,result));
                    T0=T0->ptr[1];
                }
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->width-width);
                opn1.kind=ID;     strcpy(opn1.id,T->type_id);  //���溯����
                opn1.offset=rtn;  //����offset���Ա��溯���������,��Ŀ���������ʱ���ܻ�ȡ��Ӧ��Ϣ
                result.kind=ID;   strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=merge(2,T->code,genIR(CALL,opn1,opn2,result)); //���ɺ��������м����
                break;
            case ARGS:  //�˴��������ʵ�α��ʽ����ֵ�Ĵ������У�������ARG��ʵ��ϵ��
                T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                T->code=T->ptr[0]->code;
                if (T->ptr[1]) {
                    T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                    Exp(T->ptr[1]);
                    T->width+=T->ptr[1]->width;
                    T->code=merge(2,T->code,T->ptr[1]->code);
                }
                break;
        }
    }
}

void semantic_Analysis(struct node *T)
{//�Գ����﷨�����ȸ�����,��display�Ŀ��ƽṹ�޸���ɷ��ű������������TAC���ɣ���䲿�֣�
    int rtn,num,width;
    struct node *T0;
    struct opn opn1,opn2,result;
    if (T){
        switch (T->kind) {
            case EXT_DEF_LIST:
                if (!T->ptr[0]) break;
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);    //�����ⲿ�����б��еĵ�һ��
                T->code=T->ptr[0]->code;
                if (T->ptr[1]){
                    T->ptr[1]->offset=T->ptr[0]->offset + T->ptr[0]->width;
                    semantic_Analysis(T->ptr[1]); //���ʸ��ⲿ�����б��е������ⲿ����
                    T->code=merge(2,T->code,T->ptr[1]->code);
                }
                break;
            case EXT_VAR_DEF:   //�����ⲿ˵��,����һ������(TYPE���)�е������͵��ڶ������ӵ�������
                if(!strcmp(T->ptr[0]->type_id,"int"))
                    T->type=T->ptr[1]->type=INT;
                else if(!strcmp(T->ptr[0]->type_id,"float"))
                    T->type=T->ptr[1]->type=FLOAT;
                else if(!strcmp(T->ptr[0]->type_id,"char"))
                    T->type=T->ptr[1]->type=CHAR;
                T->ptr[1]->offset=T->offset;        //����ⲿ������ƫ�������´���
                //��һ�������Ŀ�����´���
                if(T->type==INT)
                    T->ptr[1]->width=4;
                else if(T->type==FLOAT)
                    T->ptr[1]->width=8;
                else if(T->type==CHAR)
                    T->ptr[1]->width=1;    
                ext_var_list(T->ptr[1]);            //�����ⲿ����˵���еı�ʶ������
                //��������ⲿ����˵���Ŀ��
                if(T->type==INT)
                    T->width=4*T->ptr[1]->num;
                else if(T->type==FLOAT)
                    T->width=8*T->ptr[1]->num;
                else if(T->type==CHAR)
                    T->width=1*T->ptr[1]->num;
                T->code=NULL;             //����ٶ��ⲿ������֧�ֳ�ʼ��
                break;
            case FUNC_DEF:      //��д����������Ϣ�����ű�
                //��ȡ�������������͵����������������Ľ��
                if(!strcmp(T->ptr[0]->type_id,"int"))
                    T->ptr[1]->type=INT;
                if(!strcmp(T->ptr[0]->type_id,"float"))
                    T->ptr[1]->type=FLOAT;
                if(!strcmp(T->ptr[0]->type_id,"char"))
                    T->ptr[1]->type=CHAR;
                T->width=0;     //�����Ŀ������Ϊ0��������ⲿ�����ĵ�ַ�������Ӱ��
                T->offset=DX;   //���þֲ������ڻ��¼�е�ƫ������ֵ
                semantic_Analysis(T->ptr[1]); //���������Ͳ�����㲿�֣����ﲻ�����üĴ������ݲ���
                T->offset+=T->ptr[1]->width;   //���βε�Ԫ����޸ĺ����ֲ���������ʼƫ����
                T->ptr[2]->offset=T->offset;
                strcpy(T->ptr[2]->Snext,newLabel());  //���������ִ�н������λ������
                semantic_Analysis(T->ptr[2]);         //����������
                //������¼��С,����offset���Դ�ŵ��ǻ��¼��С������ƫ��
                symbolTable.symbols[T->ptr[1]->place].offset=T->offset+T->ptr[2]->width;
                T->code=merge(3,T->ptr[1]->code,T->ptr[2]->code,genLabel(T->ptr[2]->Snext));//������Ĵ�����Ϊ�����Ĵ���
                break;
            case FUNC_DEC:      //���ݷ������ͣ���������д���ű�
                rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'F',0);//���������������з��䵥Ԫ��ƫ����Ϊ0
                if (rtn==-1){
                    semantic_error(T->pos,T->type_id, "�����ظ�����");
                    break;
                }
                else T->place=rtn;
                result.kind=ID;   strcpy(result.id,T->type_id);
                result.offset=rtn;
                T->code=genIR(FUNCTION,opn1,opn2,result);     //�����м���룺FUNCTION ������
                T->offset=DX;   //������ʽ�����ڻ��¼�е�ƫ������ֵ
                if (T->ptr[0]) { //�ж��Ƿ��в���
                    T->ptr[0]->offset=T->offset;
                    semantic_Analysis(T->ptr[0]);  //�����������б�
                    T->width=T->ptr[0]->width;
                    symbolTable.symbols[rtn].paramnum=T->ptr[0]->num;
                    T->code=merge(2,T->code,T->ptr[0]->code);  //���Ӻ������Ͳ�����������
                }
                else symbolTable.symbols[rtn].paramnum=0,T->width=0;
                break;
            case PARAM_LIST:    //��������ʽ�����б�
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                if (T->ptr[1]){
                    T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                    semantic_Analysis(T->ptr[1]);
                    T->num=T->ptr[0]->num+T->ptr[1]->num;        //ͳ�Ʋ�������
                    T->width=T->ptr[0]->width+T->ptr[1]->width;  //�ۼӲ�����Ԫ���
                    T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code);  //���Ӳ�������
                }
                else {
                    T->num=T->ptr[0]->num;
                    T->width=T->ptr[0]->width;
                    T->code=T->ptr[0]->code;
                }
                break;
            case PARAM_DEC:
                rtn=fillSymbolTable(T->ptr[1]->type_id,newAlias(),1,T->ptr[0]->type,'P',T->offset);
                if (rtn==-1)
                    semantic_error(T->ptr[1]->pos,T->ptr[1]->type_id, "�������ظ�����");
                else T->ptr[1]->place=rtn;
                T->num=1;       //������������ĳ�ʼֵ
                if(T->ptr[0]->type==INT)//�������
                    T->width=4;
                else if(T->ptr[0]->type==FLOAT)
                    T->width=8;
                else if(T->ptr[0]->type==CHAR)
                    T->width=1;
                // T->width=T->ptr[0]->type==INT?4:8;  //�������
                result.kind=ID;   strcpy(result.id, symbolTable.symbols[rtn].alias);
                result.offset=T->offset;
                T->code=genIR(PARAM,opn1,opn2,result);     //���ɣ�FUNCTION ������
                break;
            case COMP_STM:
                LEV++;
                //���ò�ż�1�����ұ���ò�ֲ������ڷ��ű��е���ʼλ����symbol_scope_TX
                symbol_scope_TX.TX[symbol_scope_TX.top++]=symbolTable.index;
                T->width=0;
                T->code=NULL;
                if (T->ptr[0]) {
                    T->ptr[0]->offset=T->offset;
                    semantic_Analysis(T->ptr[0]);  //����ò�ľֲ�����DEF_LIST
                    T->width+=T->ptr[0]->width;
                    T->code=T->ptr[0]->code;
                }
                if (T->ptr[1]){
                    T->ptr[1]->offset=T->offset+T->width;
                    strcpy(T->ptr[1]->Snext,T->Snext);  //S.next�������´���
                    semantic_Analysis(T->ptr[1]);       //�����������������
                    T->width+=T->ptr[1]->width;
                    T->code=merge(2,T->code,T->ptr[1]->code);
                }
                prn_symbol();       //c���˳�һ���������ǰ��ʾ�ķ��ű�
                LEV--;    //��������䣬��ż�1
                symbolTable.index=symbol_scope_TX.TX[--symbol_scope_TX.top]; //ɾ�����������еķ���
                break;
            case DEF_LIST:
                T->code=NULL;
                if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    semantic_Analysis(T->ptr[0]);   //����һ���ֲ���������
                    T->code=T->ptr[0]->code;
                    T->width=T->ptr[0]->width;
                }
                if (T->ptr[1]) {
                    T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                    semantic_Analysis(T->ptr[1]);   //����ʣ�µľֲ���������
                    T->code=merge(2,T->code,T->ptr[1]->code);
                    T->width+=T->ptr[1]->width;
                }
                break;
            case VAR_DEF://����һ���ֲ���������,����һ������(TYPE���)�е������͵��ڶ������ӵ�������
                        //������������ⲿ����EXT_VAR_DEF������һ�ִ�����
                T->code=NULL;
                //ȷ���������и���������
                if(!strcmp(T->ptr[0]->type_id,"int"))
                    T->ptr[1]->type=INT;
                else if(!strcmp(T->ptr[0]->type_id,"float"))
                    T->ptr[1]->type=FLOAT;
                else if(!strcmp(T->ptr[0]->type_id,"char"))
                    T->ptr[1]->type=CHAR;
                T0=T->ptr[1]; //T0Ϊ�������б�������ָ�룬��ID��ASSIGNOP�����ڵǼǵ����ű���Ϊ�ֲ�����
                num=0;
                T0->offset=T->offset;
                T->width=0;
                //һ���������
                if(T->ptr[1]->type==INT)
                    width=4;
                else if(T->ptr[1]->type==FLOAT)
                    width=8;
                else if(T->ptr[1]->type==CHAR)
                    width=1;    
                while (T0) {  //��������DEC_LIST���
                    num++;
                    T0->ptr[0]->type=T0->type;  //�����������´���
                    if (T0->ptr[1]) T0->ptr[1]->type=T0->type;
                    T0->ptr[0]->offset=T0->offset;  //�����������´���
                    if (T0->ptr[1]) T0->ptr[1]->offset=T0->offset+width;
                    if (T0->ptr[0]->kind==ID){
                        rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',T->offset+T->width);//�˴�ƫ����δ���㣬��ʱΪ0
                        if (rtn==-1)
                            semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "�����ظ�����");
                        else T0->ptr[0]->place=rtn;
                        T->width+=width;
                    }
                    else if (T0->ptr[0]->kind==ASSIGNOP){
                        rtn=fillSymbolTable(T0->ptr[0]->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',T->offset+T->width);//�˴�ƫ����δ���㣬��ʱΪ0
                        if (rtn==-1)
                            semantic_error(T0->ptr[0]->ptr[0]->pos,T0->ptr[0]->ptr[0]->type_id, "�����ظ�����");
                        else {
                            T0->ptr[0]->place=rtn;
                            T0->ptr[0]->ptr[1]->offset=T->offset+T->width+width;
                            Exp(T0->ptr[0]->ptr[1]);
                            opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                            result.kind=ID; strcpy(result.id,symbolTable.symbols[T0->ptr[0]->place].alias);
                            T->code=merge(3,T->code,T0->ptr[0]->ptr[1]->code,genIR(ASSIGNOP,opn1,opn2,result));
                        }
                        T->width+=width+T0->ptr[0]->ptr[1]->width;
                    }
                    T0=T0->ptr[1];
                }
                break;
            case STM_LIST:
                if (!T->ptr[0]) { T->code=NULL; T->width=0; break;}   //���������
                if (T->ptr[1]) //2������������ӣ������±����Ϊ��һ���������󵽴��λ��
                    strcpy(T->ptr[0]->Snext,newLabel());
                else     //������н���һ����䣬S.next�������´���
                    strcpy(T->ptr[0]->Snext,T->Snext);
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                if (T->ptr[1]){     //2�������������,S.next�������´���
                    strcpy(T->ptr[1]->Snext,T->Snext);
                    T->ptr[1]->offset=T->offset;  //˳��ṹ����Ԫ��ʽ
                    //T->ptr[1]->offset=T->offset+T->ptr[0]->width; //˳��ṹ˳����䵥Ԫ��ʽ
                    semantic_Analysis(T->ptr[1]);
                    //�����е�1��Ϊ���ʽ��䣬������䣬�������ʱ����2��ǰ����Ҫ���
                    if (T->ptr[0]->kind==RETURN ||T->ptr[0]->kind==EXP_STMT ||T->ptr[0]->kind==COMP_STM)
                        T->code=merge(2,T->code,T->ptr[1]->code);
                    else
                        T->code=merge(3,T->code,genLabel(T->ptr[0]->Snext),T->ptr[1]->code);
                    if (T->ptr[1]->width>T->width) T->width=T->ptr[1]->width; //˳��ṹ����Ԫ��ʽ
                    //T->width+=T->ptr[1]->width;//˳��ṹ˳����䵥Ԫ��ʽ
                }
                break;
            case IF_THEN:
                strcpy(T->ptr[0]->Etrue,newLabel());  //��������������ת��λ��
                strcpy(T->ptr[0]->Efalse,T->Snext);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,T->Snext);
                semantic_Analysis(T->ptr[1]);      //if�Ӿ�
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                T->code=merge(3,T->ptr[0]->code, genLabel(T->ptr[0]->Etrue),T->ptr[1]->code);
                break;  //������䶼��û�д���offset��width����
            case IF_THEN_ELSE:
                strcpy(T->ptr[0]->Etrue,newLabel());  //��������������ת��λ��
                strcpy(T->ptr[0]->Efalse,newLabel());
                T->ptr[0]->offset=T->ptr[1]->offset=T->ptr[2]->offset=T->offset;
                boolExp(T->ptr[0]);      //������Ҫ��������·���봦��
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,T->Snext);
                semantic_Analysis(T->ptr[1]);      //if�Ӿ�
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                strcpy(T->ptr[2]->Snext,T->Snext);
                semantic_Analysis(T->ptr[2]);      //else�Ӿ�
                if (T->width<T->ptr[2]->width) T->width=T->ptr[2]->width;
                T->code=merge(6,T->ptr[0]->code,genLabel(T->ptr[0]->Etrue),T->ptr[1]->code,\
                                genGoto(T->Snext),genLabel(T->ptr[0]->Efalse),T->ptr[2]->code);
                break;
            case WHILE: 
                strcpy(T->ptr[0]->Etrue,newLabel());  //�ӽ��̳����Եļ���
                strcpy(T->ptr[0]->Efalse,T->Snext);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);      //ѭ��������Ҫ��������·���봦��
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,newLabel());
                semantic_Analysis(T->ptr[1]);      //ѭ����
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                T->code=merge(5,genLabel(T->ptr[1]->Snext),T->ptr[0]->code, \
                genLabel(T->ptr[0]->Etrue),T->ptr[1]->code,genGoto(T->ptr[1]->Snext));
                break;
            case EXP_STMT:
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                break;
            case RETURN:
                if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]);
                    num=symbolTable.index;
                    do num--; while (symbolTable.symbols[num].flag!='F');
                    if (T->ptr[0]->type!=symbolTable.symbols[num].type) {
                        semantic_error(T->pos, "����ֵ���ʹ���","");
                        T->width=0;T->code=NULL;
                        break;
                        }
                    T->width=T->ptr[0]->width;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->ptr[0]->code,genIR(RETURN,opn1,opn2,result));
                }
                else{
                    T->width=0;
                    result.kind=0;
                    T->code=genIR(RETURN,opn1,opn2,result);
                }
                break;
            
            
            
            
            
            
            
            case ID:
            case ARRAY_DEF:
            case INT:
            case FLOAT:
            case CHAR:
            case ASSIGNOP:
            case AND:
            case OR:
            case RELOP:
            case COMP_PLUS:
            case COMP_MINUS:
            case PLUS:
            case MINUS:
            case STAR:
            case DIV:
            case INC:
            case DEC:
            case NOT:
            case UMINUS:
            case FUNC_CALL:
                Exp(T);          //����������ʽ
                break;
        }
    }
}

void semantic_Analysis0(struct node *T) {
    symbolTable.index=0;
    fillSymbolTable("read","",0,INT,'F',4);//����Ĭ�ϵ����뺯��
    symbolTable.symbols[0].paramnum=0;//read���βθ���
    fillSymbolTable("write","",0,INT,'F',4);//����Ĭ�ϵ��������
    fillSymbolTable("x","",1,INT,'P',12);//�����б�
    symbolTable.symbols[2].paramnum=1;
    symbol_scope_TX.TX[0]=0;//�ⲿ�����ڷ��ű��е���ʼ���Ϊ0
    symbol_scope_TX.top=1;
    T->offset=0;//�ⲿ��������������ƫ����
    semantic_Analysis(T);
    // objectCode(T->code);
 } 

