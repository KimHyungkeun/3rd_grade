#include "Stack.c"

int main(void){
		
push(5);
push(2);
push(3);
push(4);
push(1);
printStack();
pop();
pop();
push(15);
printf("TOP OF STACK : %d\n", peek());
printStack();
pop();
pop();
pop();
pop();
push(30);
printStack();
	}
