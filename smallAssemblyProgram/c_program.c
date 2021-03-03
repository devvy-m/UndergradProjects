#include <string.h>
#include <stdio.h>
void printstring(const char *message);

int main() {
	
	const char *lineOne = "\nHours of brutal work.\n All for two magical words.\n"
					    "  Hello World! At last!\n  Lynnli W.\n\0";
	printstring(lineOne);
	
	return 0;
}
