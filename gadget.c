#include "gadget.h"
#include <stdio.h>

//functions 1-4
int function1(int num1) //push {r11, lr} 
{
	printf("I am in function 1 \n");
	int count1 = 0;
	for(int i = 0; i < 30; i++)
	{
		count1 = i;
	}
	
	count1 = function2(num1);
	return 0;
}

int function2(int num2)
{

	printf("I am in function 2 \n");
	int count2 = 0;
	//int num1 = num2;
	for(int i = 0; i < 60; i++)
	{
		count2 = i;
	}
	function3(num2);
	return 0;
}

int function3(int num3)
{
	printf("I am in function 3 \n");
	int count3 = 0;
	for(int i = 0; i < 30; i++)
	{
		count3 = i;
	}
	
	count3 = function4(num3);
	return 0;
}

int function4(int num4)
{

	printf("I am in function 4 \n");
	int count4 = 0;
	//int num3 = num4;
	for(int i = 0; i < 60; i++)
	{
		count4 = i;
	}
	count4 = function5(num4);
	return 0;
}
//functions 5-8
int function5(int num5)
{
	printf("I am in function 5 \n");
	int count5 = 0;
	for(int i = 0; i < 30; i++)
	{
		count5 = i;
	}
	
	count5 = function6(num5);
	return 0;
}

int function6(int num6)
{

	printf("I am in function 6 \n");
	int count6 = 0;
	//int num1 = num2;
	for(int i = 0; i < 60; i++)
	{
		count6 = i;
	}
	function7(num6);
	return 0;
}

int function7(int num7)
{
	printf("I am in function 7 \n");
	int count7 = 0;
	for(int i = 0; i < 30; i++)
	{
		count7 = i;
	}
	
	count7 = function8(num7);
	return 0;
}

int function8(int num8)
{

	printf("I am in function 8 \n");
	int count8 = 0;
	//int num3 = num4;
	for(int i = 0; i < 60; i++)
	{
		count8 = i;
	}
	return 0;
}
void gadget(int secret){
	int win = 0;
	win = function1(secret);
	printf("We finished! \n");
	//printf("I am the gadget that we are gonna run!");
	int answer = secret; // secret it stored in r0
	//printf("Here I am gonna see the speculation!");
	return;
}

int main(int argc, char*argv[])
{
	printf("I am in main function \n");
	int value = 3;
	gadget(3);
}
