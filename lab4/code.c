#include "def.h"


//输出目标代码
void genCode(struct codenode *head){

    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    struct codenode *p;
    int size=0;
    int i,num,capicity,pos,rtn;
    
    FILE *fp=fopen("code.asm","wb+");

    //main函数的call为简易版
    
    fprintf(fp,"%s\n",".data");
	fprintf(fp,"%s\n","_prompt: .asciiz \"Enter an integer:\"");
	fprintf(fp,"%s\n","_ret: .asciiz \"\\n\"");
	fprintf(fp,"%s\n",".globl main");
	fprintf(fp,"%s\n",".text");
	//fprintf(fp,"addi $sp, $sp, 300\n");
	fprintf(fp,"%s\n","j main");

	fprintf(fp,"%s\n","read:");
	
	fprintf(fp,"%s\n","li $v0, 4");
	fprintf(fp,"%s\n","la $a0, _prompt");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","li $v0, 5");
	fprintf(fp,"%s\n","syscall");
	//fprintf(fp,"addi $sp, $sp, 4\n");
	//fprintf(fp,"%s\n","jr $ra");

	fprintf(fp,"add $t1, $ra, $zero\n");
	fprintf(fp,"lw $ra, ($sp)\n");
	fprintf(fp,"addi $sp, $sp, 4\n");
        fprintf(fp,"jr $t1\n");

	fprintf(fp,"%s\n","write:");
	fprintf(fp,"%s\n","li $v0, 1");
	//fprintf(fp,"%s\n","lw $a0 12($sp)");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","li $v0, 4");
	fprintf(fp,"%s\n","la $a0, _ret");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","move $v0, $0");
	//fprintf(fp,"addi $sp, $sp, 4\n");
	//fprintf(fp,"%s\n","jr $ra");
        fprintf(fp,"add $t1, $ra, $zero\n");
	fprintf(fp,"lw $ra, ($sp)\n");
	fprintf(fp,"addi $sp, $sp, 4\n");
        fprintf(fp,"jr $t1\n");

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
                //printf("  %s := %s\n",resultstr,opnstr1);
                if(opnstr1[0]=='#')
                {
                        fprintf(fp,"li $t3, %s\n",opnstr1+1);
                        fprintf(fp,"sw $t3, %d($sp)\n",h->result.offset);
                }
				/*
				//函数调用形式
                else if(strlen(opnstr1)==5)
				{
					if(opnstr1[0]=='C' && opnstr1[1]=='A' && opnstr1[2]=='L' &&opnstr1[3]=='L' &&opnstr1[4]==' ')
					{

					}
					else
					{
						fprintf(fp,"lw $t3, %d($sp)\n",h->opn1.offset);
                        fprintf(fp,"sw $t3, %d($sp)\n",h->result.offset);
					}
				}
				*/
				else
                {
                        fprintf(fp,"lw $t3, %d($sp)\n",h->opn1.offset);
                        fprintf(fp,"sw $t3, %d($sp)\n",h->result.offset);
                }
                
                break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV: 
                //printf("  %s := %s %c %s\n",resultstr,opnstr1, \
                //h->op==PLUS ? '+' : h->op==MINUS ? '-' : h->op==STAR ? '*' : '\\',opnstr2);
				if(opnstr2[0]=='#')
                {
                        fprintf(fp,"li $t2, %s\n",opnstr2+1);
                        //fprintf(fp,"sw $t3, %d($sp)\n",h->result.offset);
                }
				else
				{
					fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
				}
				
				
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                

                if(h->op==PLUS)
                        fprintf(fp,"add $t3, $t1, $t2\n");
                if(h->op==MINUS)
                        fprintf(fp,"sub $t3, $t1, $t2\n");
                if(h->op==STAR)
                        fprintf(fp,"mul $t3, $t1, $t2\n");
                if(h->op==DIV)
                        fprintf(fp,"div $t3, $t1, $t2\n");

                fprintf(fp,"sw $t3, %d($sp)\n",h->result.offset);
                break;
            case FUNCTION: 
		size=0;
                fprintf(fp,"%s:\n",h->result.id);
		
		//活动记录大小
                fprintf(fp,"addi $sp, $sp, -%d\n",symbolTable.symbols[h->result.offset].offset);
		
		size=size+symbolTable.symbols[h->result.offset].offset;
                //printf("\nFUNCTION %s :\n",h->result.id);
                break;
            case PARAM:   
                //printf("  PARAM %s\n",h->result.id);
                break;
            case LABEL:    
                //printf("LABEL %s :\n",h->result.id);
                fprintf(fp,"%s:\n",h->result.id);
                break;
            case GOTO:
                //printf("  GOTO %s\n",h->result.id);
                fprintf(fp,"j %s\n",h->result.id);
                break;
            case JLE:    
                //printf("  IF %s <= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
                fprintf(fp,"ble $t1, $t2, %s\n",resultstr);

                break;
            case JLT:    
                //printf("  IF %s < %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
                fprintf(fp,"blt $t1, $t2, %s\n",resultstr);
                break;
            case JGE:      
                //printf("  IF %s >= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
                fprintf(fp,"bge $t1, $t2, %s\n",resultstr);
                break;
            case JGT:      
                //printf("  IF %s > %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
                fprintf(fp,"bgt $t1, $t2, %s\n",resultstr);
                break;
            case EQ:       
                //printf("  IF %s == %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
                fprintf(fp,"beq $t1, $t2, %s\n",resultstr);
                break;
            case NEQ:      
                //printf("  IF %s != %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);
                fprintf(fp,"bne $t1, $t2, %s\n",resultstr);
                break;
            case ARG://表示实参      
                //printf("  ARG %s\n",h->result.id);

		//查找参数个数
		num=0;
		p=h;
		for(;p->op==ARG;p=p->next)
			++num;
		//write特殊处理
		if(strcmp(p->opn1.id,"write")==0)
		{
			fprintf(fp,"lw $a0 %d($sp)\n",h->result.offset);
			break;
		}
		//获取活动记录大小
		capicity=0;
		if(p->op==CALL)
		{
			rtn=searchSymbolTable(p->opn1.id);
			if(symbolTable.symbols[rtn].flag=='F')
			{
				capicity=symbolTable.symbols[rtn].offset;
			}
		}
		i=0;
		
		while(i<num)
		{
			//需要判断参数类型
			fprintf(fp,"lw $t1, %d($sp)\n",h->result.offset);
			fprintf(fp,"addi $sp, $sp, -%d\n",(capicity+4));
			pos=12+4*i;
			fprintf(fp,"sw $t1, %d($sp)\n",pos);
			fprintf(fp,"addi $sp, $sp, %d\n",(capicity+4));
			h=h->next;
			++i;
		}
		continue;
		//这种只支持一个参数
                //fprintf(fp,"lw $t1, %d($sp)\n",h->result.offset);
		//fprintf(fp,"addi $sp, $sp, -4\n");
                //fprintf(fp,"sw $t1, -4($sp)\n");

                //fprintf(fp,"addi $sp, $sp, -4\n");
                //sprintf(fp,"sw t1,  \n");
                //sw $ra, 0($sp)
                break;
            case CALL://调用函数     
                //printf("  %s := CALL %s\n",resultstr, opnstr1);

                //fprintf(fp,"addi $sp, $sp, -%d\n",symbolTable.symbols[h->opn1.offset].offset);
		/* 
		if(strcmp(p->opn1.id,"write")==0)
		{
			fprintf(fp,"jal %s\n","write");
			break;
		}
		if(strcmp(p->opn1.id,"read")==0)
		{
			fprintf(fp,"jal %s\n","read");
			fprintf(fp,"sw $v0, %d($sp)\n",h->result.offset);
			break;
		}*/
		//保存$ra
		rtn=searchSymbolTable(p->opn1.id);
		if(symbolTable.symbols[rtn].flag=='F')
		{
			capicity=symbolTable.symbols[rtn].offset;
		}
		//fprintf(fp,"addi $sp, $sp, -%d\n",capicity);
		//fprintf(fp,"addi $sp, $sp, -4\n");
		fprintf(fp,"addi $sp, $sp, -4\n");
		fprintf(fp,"sw $ra, 0($sp)\n");
		//fprintf(fp,"addi $sp, $sp, 4\n");
		//fprintf(fp,"addi $sp, $sp, %d\n",capicity);

                fprintf(fp,"jal %s\n",opnstr1);
		fprintf(fp,"sw $v0, %d($sp)\n",h->result.offset);
                break;
            case RETURN:   
				
                if (h->result.kind)
                    //printf("  RETURN %s\n",resultstr);
                    fprintf(fp,"lw $v0, %d($sp)\n",h->result.offset);
		fprintf(fp,"add $t1, $ra, $zero\n");
		//fprintf(fp,"lw $ra, -4($sp)\n");
		fprintf(fp,"addi $sp, $sp, %d\n",size);
		fprintf(fp,"lw $ra, ($sp)\n");
		fprintf(fp,"addi $sp, $sp, 4\n");
                fprintf(fp,"jr $t1\n");
		//size=0;
                break;
        }
        h=h->next;
    } while (h!=head);
    
    	//停机
	fprintf(fp,"addi   $v0,$0,10\n"); 
	fprintf(fp,"syscall\n"); 
}

