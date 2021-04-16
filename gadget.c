void gadget()
{
	//change address of lr
	r14 = 0xabcdef //fake address to try and mess with the return stack and see if it will work. 
	//then push new lr 8 times to overwrite all other places on the return stack....
	push {r14}; //attempting to overfill the return stack???
}



