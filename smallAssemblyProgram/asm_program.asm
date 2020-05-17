/* assembly program that can either use the assembly or c function to print a haiku to the command line
*/
  
  
.global main

.text
	
main:
	mov		$ haiku, %rdi		/*address of string to output*/
	mov		$ 1, % rdx		/*invoke operating system call to sys_write*/
	call 		printString		/*function call*/
	ret						/*exit the program*/
	
.data
	/*haiku contains the enter haiku, I declared multiple lines with the type under the same label*/
haiku:		.ascii	"\nHours of brutal work.\n"
			.ascii	" All for two magical words.\n"  /*Haiku*/
			.ascii	"  Hello World...At last!\n"
			.ascii	"   By: Lynnli W.\0"
