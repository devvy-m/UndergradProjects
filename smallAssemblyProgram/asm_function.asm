/* assembly function should take one argument (pointer to string) and print it */

.global printstring


.text 
printstring:
	
	mov %rdi, %rdx	/*first argument passed to rdi*/
	mov $0, %rax		/* system call to %rax to read*/
	call printf			/*c-library print*/
	syscall
	ret				/*exit function*/
