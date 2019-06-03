#include "def.h"


//输出中间代码
void genCode(struct codenode *head){

    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    int size=0;
    FILE *fp=fopen("code.asm","wb+");
    /*
    fprintf(fp,"%s\n",".data");
	fprintf(fp,"%s\n","_prompt: .asciiz \"Enter an integer:\"");
	fprintf(fp,"%s\n","_ret: .asciiz \"\\n\"");
	fprintf(fp,"%s\n",".globl main");
	fprintf(fp,"%s\n",".text");
	fprintf(fp,"%s\n","read:");
	fprintf(fp,"%s\n","li $v0, 4");
	fprintf(fp,"%s\n","la $a0, _prompt");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","li $v0, 5");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","jr $ra");
	fprintf(fp,"%s\n","write:");
	fprintf(fp,"%s\n","li $v0, 1");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","li $v0, 4");
	fprintf(fp,"%s\n","la $a0, _ret");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","move $v0, $0");
	fprintf(fp,"%s\n","jr $ra");
        */
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

                fprintf(fp,"lw $t1, %d($sp)\n",h->opn1.offset);
                fprintf(fp,"lw $t2, %d($sp)\n",h->opn2.offset);

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

                fprintf(fp,"%s:\n",h->result.id);
                fprintf(fp,"addi $sp, $sp, -%d\n",symbolTable.symbols[h->result.offset].offset);
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
                size=size+4;
                fprintf(fp,"addi $sp, $sp, -4\n");
                fprintf(fp,"lw $t1, %d($sp)\n",h->result.offset);
                fprintf(fp,"sw $t1, ($sp)\n");

                //fprintf(fp,"addi $sp, $sp, -4\n");
                //sprintf(fp,"sw t1,  \n");
                //sw $ra, 0($sp)
                break;
            case CALL://调用函数     
                //printf("  %s := CALL %s\n",resultstr, opnstr1);

                //fprintf(fp,"addi $sp, $sp, -%d\n",symbolTable.symbols[h->opn1.offset].offset);
                fprintf(fp,"jal %s\n",opnstr1);
                break;
            case RETURN:   
                if (h->result.kind)
                    //printf("  RETURN %s\n",resultstr);
                    fprintf(fp,"lw $v0, %d($sp)\n",h->result.offset);

                fprintf(fp,"jr $ra\n");
                break;
        }
        h=h->next;
    } while (h!=head);
    
}

/*
void function_tra(struct code_node* begin,struct code_node* end)
{
	count = 0;
	size = 0;
	arg_count = 0;
	fprintf(fprint,"%s:\n",begin->args[1]);
	struct code_node* p = begin->next;
	while(p != end)
	{
		switch(p->args_count)
		{
			case 2:
				if(strcmp("GOTO",p->args[0]) != 0) var_add(p->args[1],4);
				break;
			case 3:
				if(strcmp(p->args[1],":=") == 0)
				{
					var_add(p->args[0],4);
					var_add(p->args[2],4);
				}
				else if(strcmp(p->args[0],"DEC") == 0)
				{
					int a = strtol(p->args[2],NULL,10);
					var_add(p->args[1],a);
				}
				break;
			case 4:
				var_add(p->args[0],4);
				break;
			case 5:
				var_add(p->args[0],4);
				var_add(p->args[2],4);
				var_add(p->args[4],4);
				break;
			case 6:
				var_add(p->args[1],4);
				var_add(p->args[3],4);
				break;
			default:;
		}
		p = p->next;
	}
	fprintf(fprint,"addi $sp, $sp, -%d\n",size);
	p = begin->next;
	while(p != end)
	{
		switch(p->args_count)
		{
			case 2:
				if(strcmp(p->args[0],"GOTO") == 0){fprintf(fprint,"j %s\n",p->args[1]);}
				else if(strcmp(p->args[0],"RETURN") == 0)
				{
					prep_register(p->args[1],0);
					fprintf(fprint,"move $v0, $t0\n");
					fprintf(fprint,"addi $sp, $sp, %d\n",size);
					fprintf(fprint,"jr $ra\n");
				}
				else if(strcmp(p->args[0],"ARG") == 0)
				{
					prep_register(p->args[1],0);
					fprintf(fprint,"move $a%d, $t0\n",arg_count);
					arg_count++;
				}
				else if(strcmp(p->args[0],"PARAM") == 0)
				{
					int para_count = 0;
					struct code_node* q = p;
					while(strcmp(q->next->args[0],"PARAM") == 0)
					{
						q = q->next;
						para_count++;
					}
					 fprintf(fprint,"sw $a%d, %d($sp)\n",para_count,var_get(p->args[1]));
				}
				else if(strcmp(p->args[0],"READ") == 0)
				{
					fprintf(fprint,"addi $sp, $sp, -4\n");
					fprintf(fprint,"sw $ra, 0($sp)\n");
					fprintf(fprint,"jal read\n");
					fprintf(fprint,"lw $ra, 0($sp)\n");
					fprintf(fprint,"addi $sp, $sp, 4\n");
					if(p->args[1][0] == '*')
					{
						fprintf(fprint,"lw $t0, %d($sp)\n",var_get(p->args[1]));
						fprintf(fprint,"add $t0, $t0, $sp\n");
						fprintf(fprint,"sw $v0, 0($t0)\n");
					}
					else fprintf(fprint,"sw $v0, %d($sp)\n",var_get(p->args[1]));
				}
				else if(strcmp(p->args[0],"WRITE") == 0)
				{
					prep_register(p->args[1],0);
					fprintf(fprint,"move $a0, $t0\n");
					fprintf(fprint,"addi $sp, $sp, -4\n");
					fprintf(fprint,"sw $ra, 0($sp)\n");
					fprintf(fprint,"jal write\n");
					fprintf(fprint,"lw $ra, 0($sp)\n");
					fprintf(fprint,"addi $sp, $sp, 4\n");
				}
				break;
			case 3:
				if(strcmp(p->args[1],":=") == 0)
				{
					prep_register(p->args[2],0);
					if(p->args[0][0] == '*')
					{
						fprintf(fprint,"lw $t1, %d($sp)\n",var_get(p->args[0]));
						fprintf(fprint,"add $t1, $t1, $sp\n");
						fprintf(fprint,"sw $t0, 0($t1)\n");
					}
					else{fprintf(fprint,"sw $t0, %d($sp)\n",var_get(p->args[0]));}
				}
				else if(strcmp(p->args[0],"DEC") != 0){fprintf(fprint,"%s:\n",p->args[1]);}
				break;
			case 4:
				arg_count = 0;
				fprintf(fprint,"addi $sp, $sp, -4\n");
				fprintf(fprint,"sw $ra, 0($sp)\n");
				fprintf(fprint,"jal %s\n",p->args[3]);
				fprintf(fprint,"lw $ra, 0($sp)\n");
				fprintf(fprint,"addi $sp, $sp, 4\n");
				if(p->args[0][0] == '*')
				{
					fprintf(fprint,"lw $t0, %d($sp)\n",var_get(p->args[0]));
					fprintf(fprint,"add $t0, $t0 ,$sp\n");
					fprintf(fprint,"sw $v0, 0($t0)\n");
				}
				else fprintf(fprint,"sw $v0, %d($sp)\n",var_get(p->args[0]));
				break;
			case 5:
				prep_register(p->args[2],0);
				prep_register(p->args[4],1);
				switch(p->args[3][0])
				{
					case '+':
						fprintf(fprint,"add $t0, $t0, $t1\n");
						break;
					case '-':
						fprintf(fprint,"sub $t0, $t0, $t1\n");
						break;
					case '*':
						fprintf(fprint,"mul $t0, $t0, $t1\n");
						break;
					case '/':
						fprintf(fprint,"div $t0, $t1\n");
						fprintf(fprint,"mflo $t0\n");
						break;
					default:;
				}
				if(p->args[0][0] == '*')
				{
					 fprintf(fprint,"lw $t1, %d($sp)\n",var_get(p->args[0]));
					 fprintf(fprint,"add $t1, $t1 ,$sp\n");
					 fprintf(fprint,"sw $t0, 0($t1)\n");
				}
				else fprintf(fprint,"sw $t0, %d($sp)\n",var_get(p->args[0]));
				break;
			case 6:
			{
				char temp[4];
				if(strcmp(p->args[2],"==") == 0)
					strcpy(temp,"beq");
				else if(strcmp(p->args[2],"!=") == 0)
					strcpy(temp,"bne");
				else if(strcmp(p->args[2],">") == 0)
					strcpy(temp,"bgt");
				else if(strcmp(p->args[2],"<") == 0)
					strcpy(temp,"blt");
				else if(strcmp(p->args[2],">=") == 0)
					strcpy(temp,"bge");
				else if(strcmp(p->args[2],"<=") == 0)
					strcpy(temp,"ble");
				prep_register(p->args[1],0);
				prep_register(p->args[3],1);
				fprintf(fprint,"%s $t0, $t1,%s\n",temp,p->args[5]);
				break;
			}
			default:;
		}
		p = p->next;
	}
}

/*
void gen_dst_code(struct codenode *h,FILE* f)
{
	struct codenode *head=h;
	FILE *fp=f;
	fprintf(fp,"%s\n",".data");
	fprintf(fp,"%s\n","_prompt: .asciiz \"Enter an integer:\"");
	fprintf(fp,"%s\n","_ret: .asciiz \"\\n\"");
	fprintf(fp,"%s\n",".globl main");
	fprintf(fp,"%s\n",".text");
	fprintf(fp,"%s\n","read:");
	fprintf(fp,"%s\n","li $v0, 4");
	fprintf(fp,"%s\n","la $a0, _prompt");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","li $v0, 5");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","jr $ra");
	fprintf(fp,"%s\n","write:");
	fprintf(fp,"%s\n","li $v0, 1");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","li $v0, 4");
	fprintf(fp,"%s\n","la $a0, _ret");
	fprintf(fp,"%s\n","syscall");
	fprintf(fp,"%s\n","move $v0, $0");
	fprintf(fp,"%s\n","jr $ra");
	struct codenode * p=head,*q;
	do
	{
		q=p->next;
		while(1)
		{
			if(strcmp(q->args[0],"FUNCTION")==0)
				break;
			else if(q==head) break;
			q=q->next;
		}
		gen_one_function(p,q);
		p=q;
	}while(p!=head);
}*/