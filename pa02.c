#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*function prototypes*/
void limit_80(char *s);
unsigned long checksum_calculations(char *input_string, int bits);

int main(int argc, char **argv){
	
	/*creating file pointer*/
	FILE *f;
	
	/*input string pointer*/
	char *input_str;
	
	/*creating integer variables*/
	int i = 0;
	int c_size = 0;
	int m;

	/*in case less amount of arguments are passed*/
	if (argc != 3){
		/*printing in format specified by professor*/
		fprintf(stderr, "Error. Please run `checksum.c filename.txt 8 || 16 || 32`\n");
		return -2;
	}
	
	/*in case files cannot be opened*/
	if (!(f = fopen(argv[1],"r"))){
		/*printing in format specified by professor*/
		fprintf(stderr, "Error. Couldn't open file. Try again. \"%s\"\n", argv[1]);
		return -2;
	}
	
	/*initializing checksum size read from file*/
	c_size = atoi(argv[2]);
	
	/*in case checksum read is not 8, 16, or 32*/
	if (!(c_size == 8 || c_size == 16 || c_size == 32)){
		/*printing in format specified by professor*/
		fprintf(stderr,"Valid checksum sizes are 8, 16, or 32\n");
		return -2;
	}
	
	/*allocating space for input string*/
	input_str = malloc(sizeof(char) * 1000);
	
	/*storing string in "input_str"*/
	while ((m = getc(f)) != EOF && i < 1000){
		input_str[i] = (char)m;
		i++;
	}
	
	/*establishing new end of string*/
	input_str[i] = '\0';
	
	/*closing file*/
	fclose(f);
	
	/*creating variables (nonegative numbers) for checksum result*/
	unsigned long checksum_8bits = 0;
	unsigned long checksum_16bits = 0;
	unsigned long checksum_32bits = 0;
	
	/*if checksum size read is 8*/
	if(c_size == 8){
		/*if checksum size is 8, calculate checksum > limit 80 chars per line > new line > print result*/
		checksum_8bits = checksum_calculations(input_str, c_size);
		/*assuting only 80 chars per line*/
		limit_80(input_str);
		printf("\n");
		/*need to cast strlen to int*/
		printf("%2d bit checksum is %8lx for all %4d chars\n", c_size, checksum_8bits & 0xff, (int)strlen(input_str));
	} 
	else if(c_size == 16){
		/*if checksum size is 16, calculate checksum > limit 80 chars per line > new line > print result*/
		if (strlen(input_str) % 2){
			strcat(input_str,"X");
		}
		limit_80(input_str);
		printf("\n");
		checksum_16bits = checksum_calculations(input_str, c_size);
		printf("%2d bit checksum is %8lx for all %4d chars\n", c_size, checksum_16bits & 0xffff, (int)strlen(input_str));
	} 
	else if(c_size == 32){
		/*if checksum size is 32, calculate checksum > limit 80 chars per line > new line > print result*/
		while (strlen(input_str) % 4){
			strcat(input_str,"X");
		}
		limit_80(input_str);
		printf("\n");
		checksum_32bits = checksum_calculations(input_str, c_size);
		printf("%2d bit checksum is %8lx for all %4d chars\n", c_size, checksum_32bits & 0xffffffff, (int)strlen(input_str));
	} 
	else{
		fprintf(stderr,"Valid checksum sizes are 8, 16, or 32\n");
		return -2;
	}
	return 0;
}

/*function that limits printing to 80 chars per line*/
void limit_80(char *s){
	int len = strlen(s);
	int i = 0;
	
	for (i; i < len; i++){
		if (i % 80 == 0){
			printf("\n");
		}
		printf("%c",s[i]);
	}
}

/*function that calculates 8, 16, or 32 bit checksums using bitwise operations (& gate)*/
unsigned long checksum_calculations(char *input_string, int bits){
	int j = 0;
	unsigned long x = 0;
	int len = strlen(input_string);
	
	/*while iterator is less than lenght of the input string passed into function*/
	while(j < len){
		/*for 32 bits*/
		if (bits == 32){
			x = x + (input_string[j] << 24);
			x = x + (input_string[j+1] << 16);
			x = x + (input_string[j+2] << 8);
			x = x + input_string[j+3];
			j = j + 4;
		}
		/*for 16 bits*/
		else if(bits == 16){
			x = x + (input_string[j] << 8);
			x = x + input_string[j+1];
			j = j + 2;
		}
		/*for 8 bits*/
		else if(bits == 8){
			x = x + input_string[j];
			j = j + 1;
		}
		else{
			/*else > error*/
			return 0;
		}
	}
	return x;
}