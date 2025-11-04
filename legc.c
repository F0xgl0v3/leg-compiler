#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct inst {
	const char *name;
	unsigned char opcode;
} InstructionMapping;
    
InstructionMapping inst[] = {

	{"add", 0x00},
	{"addi", 0x40},
	{"sub", 0x01},
	{"subi", 0x41},
	{"and", 0x02},
	{"andi", 0x42},
	{"or", 0x03},
	{"ori", 0x43},
	{"not", 0x04},
	{"noti", 0x84},
	{"xor", 0x05},
	{"xori", 0x45},
	{"shl", 0x06},
	{"shli", 0x46},
	{"shr", 0x07},
	{"shri", 0x47},
	{"eq", 0x20},
	{"eqi", 0x60},
	{"ne", 0x21},
	{"nei", 0x61},
	{"l", 0x22},
	{"li", 0x62},
	{"le", 0x23},
	{"lei", 0x63},
	{"g", 0x24},
	{"gi", 0x64},
	{"ge", 0x25},
	{"gei", 0x65},
	{"call", 0x10},
	{"ret", 0x08},

	{"cp", 0x20},
	{"cpi", 0xA0},
	{"to", 0x00},
	{"mv", 0x20},
	{"rr", 0x05},

	{"r0", 0x00},
	{"r1", 0x01},
	{"r2", 0x02},
	{"r3", 0x03},
	{"r4", 0x04},
	{"r5", 0x05},
	{"cnt", 0x06},
	{"in", 0x07},
	{"out", 0x08},
	{"stack", 0x40}

};

unsigned char getopcode ( unsigned char* arr ) {

	long size = sizeof(inst) / sizeof(inst[0]);
	for ( int i = 0; i < size; i++ ) {
		if ( strcmp( arr, inst[i].name ) == 0 ) {
        		return inst[i].opcode;	
		}	
	}
}


int main( int argc, char *argv[]) {

    
    char* inf = argv[1];
    char* outf = argv[2];
    char mnemo[10];

    // Open the file in binary write mode
    FILE *of = fopen(outf, "wb");
    if ( of == NULL) {
        perror("Error opening in-file");
        return 1;
    }

    FILE *source = fopen(inf, "rb");
    if ( source == NULL ) {
        perror("Error opening out-file");
        return 1;
    }
    
    fseek(source, 0, SEEK_END);
    long size = ftell( source );
    fseek(source, 0, SEEK_SET);
    printf("%ld\n", size);

    unsigned char data[size+1];
    unsigned char datac[size];
    fread(data, 1, size, source);
    size_t length = sizeof(data) / sizeof(data[0]) - 1;
    data[size] = '\0';
    
    /*
    for ( int i = 0; i < size; i++ ) {
	
	printf("%c", (char)data[i]);

    }
    printf("\n");
    printf("%s\n", data);
    */
    
    // convert sourcefile into mnemonics, store the number of mnemonics in k and convert the mnemonics into binary.
    int k = 0;
    for ( int i = 0, j = 0; i < size; i++ ) {
	    if ( data[i] != ' ' && data[i] != '\n' ) {
		mnemo[j] = data[i];	
		j++;
	    }
	    else {
		mnemo[j] = '\0';
		j = 0;
		if ( strcmp( mnemo, "label" ) == 0 ) {
			printf( "undifined so far" );
			return 1;
		}
		else {
			datac[k++] = getopcode( mnemo );
		}
	    }
    }


    // Write the binary data to the file
    size_t written = fwrite(datac, sizeof(unsigned char), k, of);				// change log i replaced length by mnemo cnt
    if (written != length) {
        perror("Error writing to file");
        fclose(of);
	fclose(source);
        return 1;
    }
    
    // Close the file
    fclose(of);
    fclose(source);
    printf("Binary data written to %s successfully.\n", outf);

    return 0;
}
