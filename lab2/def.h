#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "parser.tab.h"

enum node_kind{EXT_DEF_LIST,EXT_VAR_DEF,FUNC_DEF,FUNC_DEC,EXT_DEC_LIST,STRUCT_TYPE_DEF,STRUCT_VAR_DEF,STRUCT_DEF,ARRAY_DEF,TWO_ARRAY_DEF,STRUCT_SPECIFIER,PARAM_LIST,PARAM_DEC,VAR_DEF,DEC_LIST,DEF_LIST,COMP_STM,STM_LIST,EXP_STMT,IF_THEN,IF_THEN_ELSE,FUNC_CALL,ARGS,FUNCTION,PARAM,ARG,CALL,LABEL,GOTO,JLT,JLE,JGT,JGE,EQ,NEQ,TAG};
//EXT_DEF_LIST:�ⲿ�����б�
//EXT_VAR_DEF:�ⲿ��������
//FUNC_DEF:��������
//FUNC_DEC:��������
//EXT_DEC_LIST:�ⲿ�����б�

//STRUCT_SPECIFIER:�ṹ���������
//STRUCT_TYPE_DEF:�ṹ�����Ͷ��壬��������
//STRUCT_VAR_DEF:�ṹ���������
//STRUCT_DEF:�ṹ�嶨��

//ARRAY_DEF:���鶨��
//TWO_ARRAY_DEF:��ά���鶨��
//PARAM_LIST:�β��б�
//PARAM_DEC:�β�����
//VAR_DEF:��������
//DEC_LIST:�����б�
//DEF_LIST:�����б�
//COMP_STM:
//STM_LIST:
//EXP_STMT:
//IF_THEN:
//IF_THEN_ELSE:
//FUNC_CALL:
//ARGS:
//FUNCTION:
//PARAM:
//ARG:
//CALL:
//LABEL:
//GOTO:
//JLT:
//JLE:
//JGT:
//JGE:
//EQ:
//NEQ:
#define MAXLENGTH 1000//������ű��Ĵ�С
#define DX 3*sizeof(int)//���¼������Ϣ��Ҫ�ĵ�Ԫ��

struct opn{
    int kind;//��ʶ����������
    int type;//��ʶ������������
    union{
        int   const_int;//������ֵ��������
        float const_float;//���㳣��ֵ��������
        char  const_char;//�ַ�����ֵ��������
        double const_double;//?????????????????????????????????????????????????????????????????????????????????
        char  id[33];//��������ʱ�����ı��������ַ���
    };
    int level;//�����Ĳ�ţ�0��ʾ��ȫ�ֱ��������ݱ����ھ�̬������
    int offset;//������Ԫƫ�����������ڷ��ű��Ķ���λ����ţ�Ŀ���������ʱ��
};

struct codenode{//����ַTAC������,����˫��ѭ����������м����Դ���
    int  op;//TAC��������������
    struct opn opn1,opn2,result;//2����������������
    struct codenode *next,*prior;
};

struct node{//���¶Խ�����Զ���û�п��Ǵ洢Ч�ʣ�ֻ�Ǽ򵥵��г�Ҫ�õ���һЩ����
	enum node_kind kind;//�������
	union{
		char type_id[33];//�ɱ�ʶ�����ɵ�Ҷ���
		int type_int;//�����������ɵ�Ҷ���
		float type_float;//�ɸ��㳣�����ɵ�Ҷ���
        double type_double;//��˫���ȸ��������ɵ�Ҷ�ڵ�
        char type_char;//���ַ����ɵ�Ҷ�ڵ�
        int array_size[2];//�����С
	};
    struct node *ptr[3];//����ָ�룬��kindȷ���ж��ٿ�����
    int level;//���
    int place;//��ʾ����Ӧ�ı��������������ű���λ�����
    char Etrue[15],Efalse[15];//�Բ�������ʽ�ķ���ʱ�����ת��Ŀ��ı��
    char Snext[15];//�ý��������ִ�к����һ�����λ�ñ��
    struct codenode *code;//�ý���м��������ͷָ��
    char op[10];
    int type;//����Ӧֵ������
    int pos;//�﷨��λ����λ���к�
    int offset;//ƫ����
    int width;//ռ�����ֽ���
    int num;//��������
};

struct symbol{//����ֻ�г���һ�����ű���Ĳ������ԣ�û�������Լ�Ļ���
    char name[33];//����������
    int level;//��ţ��ⲿ���������������Ϊ0���β���Ϊ1��ÿ��1����������ż�1���˳���1
    int type;//�������ͻ�������ֵ����
    int paramnum;//��ʽ��������
    char alias[10];//������Ϊ���Ƕ�ײ��ʹ�ã�ʹ��ÿһ����������Ψһ
    char flag;//���ű�ǣ�������'F'  ������'V'   ������'P'  ��ʱ������'T'
    char offset;//�ⲿ�����;ֲ��������侲̬����������¼�е�ƫ����
				//�������¼��С��Ŀ���������ʱʹ��
    //����...
};

//���ű�����һ��˳��ջ��index��ֵΪ0
struct symboltable{
    struct symbol symbols[MAXLENGTH];
    int index;//ջ��ָ��
}symbolTable;

struct symbol_scope_begin{/*��ǰ������ķ����ڷ��ű�����ʼλ�����,����һ��ջ�ṹ��ÿ����һ��������䣬�����ű���indexֵ��ջ���뿪�������ʱ��ȡ����ջֵ�޸ķ��ű���indexֵ�����ɾ���ø�������е����б�������ʱ����*/
    int TX[30];
    int top;//����ʲô������������������������������������������
} symbol_scope_TX;


struct node *mknode(int kind,struct node *first,struct node *second,struct node *third,int pos);
void semantic_Analysis0(struct node *T);
void semantic_Analysis(struct node *T);
void boolExp(struct node *T);
void Exp(struct node *T);
void objectCode(struct codenode *head);