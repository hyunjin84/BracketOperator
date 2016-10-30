#include <stdio.h>
#include "string.h"

#define MAX 10000

int stack[MAX];
int top;

void init_stack();
int push(unsigned int item);
int pop();
void print_stack();
int get_stack_top();
int is_stack_empty();
int is_operator(int k);
int precedence(int op);
int bracket_to_infix(char *dst, char *src);
void infix_to_postfix(char *dst, char *src);
int is_postfix_legal(char *postfix);
int calculate_postfix(char *p);

/*
 * @brief     Calculus composed of bracket operator.
 *            f(B) like () = 1, {} = 2, [] = 3, ()[]{} = 1+3+2=6, {()[]} = 2*(3+2)=10
 *            Result = f (B) % 100,000,000  
 */
int main()
{
	FILE *file_open = fopen("data\\1.input.txt", "rt");
	if(file_open == NULL)
	{
		printf("Can not open file!\n");
	}
	else
	{
		char buffer[MAX];
		int count = 0;
		while(fgets(buffer, MAX, file_open) != NULL)
		{
			char bracket_calculus[MAX];
			// Skip first line.
			if(count == 0)
			{
				count ++;
				continue;
			}
			init_stack();
			// Remove \n in buffer.
			if(sscanf(buffer, "%s", bracket_calculus) != 1)
				continue;

			char infix_calculus[MAX*2];
			char postfix_calculus[MAX];
			memset(infix_calculus,0, sizeof(infix_calculus));
			memset(postfix_calculus,0, sizeof(postfix_calculus));

			// Convert bracket calculus to infix calculus. 
			// If bracket calculus is not legal. return 0. 
			if(bracket_to_infix(infix_calculus, bracket_calculus) == -1)
			{
				printf("%d\n",0);
				continue;
			}

			// Convert infix calculus to postfix calculus.
			infix_to_postfix(postfix_calculus, infix_calculus);

			if(!is_postfix_legal(postfix_calculus))
			{
				printf("%d\n",0);
				continue;
			}

			int result = calculate_postfix(postfix_calculus);
			printf("%d\n",result);
			count ++;
		}
		fclose(file_open);
	}
	return 0;
 }

/*
 * @brief     Initialize stack  
 */
void init_stack()
{
	memset(stack, 0, MAX);
	top = -1;
}

/*
 * @brief     Push item to stack  
 */
int push(unsigned int item)
{
	if (top >= MAX - 1)
	{
		printf("\nStack overflow.\n");
		return -1;
	}
	stack[++top] = item;
	return item;
}

/*
 * @brief     Pop item from stack  
 */
int pop()
{
	if (top < 0)
	{
		printf("\nStack underflow.\n");
		return -1;
	}
	return stack[top--];
}

/*
 * @brief     Print stack item  
 */
void print_stack()
{
	int i;
	printf("\nStack contents : TOP ----> Bottom\n");
	for (i = top; i >= 0; i--)
	{
		printf("%-6d", stack[i]);
	}
}

/*
 * @brief     Get top item of stack  
 */
int get_stack_top()
{
	return (top < 0) ? -1 : stack[top];
}

/*
 * @brief     If top less than 0, stack is empty  
 */
int is_stack_empty()
{
	return (top < 0);
}

/*
 * @brief     If k is operation, return true   
 */
int is_operator(int k)
{
	return (k == '+' || k == '-' || k == '*' || k == '/');
}

/*
 * @brief     Set precedence of operation
 */
int precedence(int op){
	if (op == '(' || op == '{' || op == '[') return 0;
	if (op == '+' || op == '-') return 12;
	if (op == '*' || op == '/') return 13;
	else return 20;
}

/*
 * @brief     Convert bracket calculus to infix calculus
 * @param     char *dst			,infix calculus
 * @param     char *src			,bracket calculus
 */
int bracket_to_infix(char *dst, char *src){
	char *ch = dst;
	init_stack();
	while (*src)
	{
		switch(*src)
		{
		case '(':
			src++;
			
			if(*src == '(' || *src == '{' || *src == '[') // (( or ({ or ([ -> 1*( 
			{
				*dst++ = '1';
				*dst++ = '*';
				*dst++ = '(';
				push('(');
			}
			else if (*src == ')') 
			{
				src++;
				if(*src == '(' || *src == '{' || *src == '[') // ()( or (){ or ()[ -> 1+
				{
					*dst++ = '1';
					*dst++ = '+';
				}
				else // () -> 1
				{
					*dst++ = '1';
				}
			}
			else
			{
				return -1;
			}
			break;
		case '{':
			src++;
			if(*src == '(' || *src == '{' || *src == '[') // {( or {{ or {[ -> 2*( 
			{
				*dst++ = '2';
				*dst++ = '*';
				*dst++ = '(';
				push('{');
			}
			else if (*src == '}')
			{
				src++;
				if(*src == '(' || *src == '{' || *src == '[') // {}( or {}{ or {}[ -> 2+
				{
					*dst++ = '2';
					*dst++ = '+';
				}
				else // () -> 2
				{
					*dst++ = '2';
				}
			}
			else
			{
				return -1;
			}
			break;
		case '[':
			src++;
			if(*src == '(' || *src == '{' || *src == '[') // [( or [{ or [[ -> 3*( 
			{
				*dst++ = '3';
				*dst++ = '*';
				*dst++ = '(';
				push('[');
			}
			else if (*src == ']')
			{
				src++;
				if(*src == '(' || *src == '{' || *src == '[') // []( or []{ or [][ -> 3+
				{
					*dst++ = '3';
					*dst++ = '+';
				}
				else // [] -> 3
				{
					*dst++ = '3';
				}
			}
			else
			{
				return -1;
			}
			break;
		case ')':
			if(get_stack_top() == '(')
			{
				*dst++ = ')';
				pop();
			}
			else
			{
				return -1;
			}
			src++;
			if(*src == '(' || *src == '{' || *src == '[') // )( or ){ or )[ -> +
			{
				*dst++ = '+';
			}
			break;
		case '}':
			if(get_stack_top() == '{')
			{
				*dst++ = ')';
				pop();
			}
			else
			{
				return -1;
			}
			src++;
			if(*src == '(' || *src == '{' || *src == '[') // }( or }{ or }[ -> +
			{
				*dst++ = '+';
			}
			break;
		case ']':
			if(get_stack_top() == '[')
			{
				*dst++ = ')';
				pop();
			}
			else
			{
				return -1;
			}
			src++;
			if(*src == '(' || *src == '{' || *src == '[') // ]( or ]{ or ][ -> +
			{
				*dst++ = '+';
			}
			break;
		}
	}

	if(!is_stack_empty())
	{
		return -1;
	}

	dst++;
	*dst = 0;
	return 0;
}

/*
 * @brief     Convert infix calculus to postfix calculus
 * @param     char *dst			,postfix calculus
 * @param     char *src			,infix calculus
 */
void infix_to_postfix(char *dst, char *src){
	char *ch = dst;
	init_stack();
	while (*src){
		if (*src == '('){
			push(*src);
			src++;
		}
		else if (*src == ')'){
			while (get_stack_top() != '('){
				*dst++ = pop();
				*dst++ = ' ';
			}
			pop();
			src++;
		}
		else if (is_operator(*src)){
			while (!is_stack_empty() &&
				precedence(get_stack_top()) >= precedence(*src)){
					*dst++ = pop();
					*dst++ = ' ';
			}
			push(*src);
			src++;
		}
		else if (*src >= '0' && *src <= '9'){
			do{
				*dst++ = *src++;
			} while (*src >= '0' && *src <= '9');
			*dst++ = ' ';
		}
		else{
			src++;
		}
	}

	while (!is_stack_empty()){
		*dst++ = pop();
		*dst++ = ' ';
	}
	dst--;
	*dst = 0;
}

/*
 * @brief      Check postfix calculus is legal
 */
int is_postfix_legal(char *s){
	int f = 0;
	while (*s){
		// Skip fist blank.
		while (*s == ' '){
			s++;
		}

		if (is_operator(*s)){
			f--;
		}
		else{
			f++;
			while (*s != ' '){
				s++;
			}
		}
		if (f < 1) break;
		s++;
	}
	return (f == 1);
}

/*
 * @brief      Calculate postfix calculus
 */
int calculate_postfix(char *p){
	unsigned int i;
	init_stack();
	while (*p){
		if (*p >= '0' && *p <= '9'){
			i = 0;
			do{
				i = i * 10 + *p - '0';
				p++;
			} while (*p >= '0' && *p <= '9');
			push(i);
		}
		else if (*p == '+'){
			push(pop() + pop());
			p++;
		}
		else if (*p == '*'){
			push(pop() * pop());
			p++;
		}
		else if (*p == '-'){
			i = pop();
			push(pop() - i);
			p++;
		}
		else if (*p == '/'){
			i = pop();
			push(pop() / i);
			p++;
		}
		else{
			p++;
		}
	}
	return pop()%100000000;
}