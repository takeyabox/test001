#include <stdio.h>

int read1(char *buffer) {

    if (*buffer <= '9' && *buffer >= '0') {
        return *buffer - '0'; // Indicates a valid hex digit
    } else if (*buffer <= 'F' && *buffer >= 'A') {
        return *buffer - 'A' + 10; // Convert hex digit to decimal value
    } else if (*buffer <= 'f' && *buffer >= 'a') {
        return *buffer - 'a' + 10; // Convert hex digit to decimal value
    }
    return -1; // Indicates an invalid hex digit
}

int read2(char *buffer){
    int one = read1(buffer);
    if (one < 0) {
        return -1; // Invalid first hex digit
    }
    int two = read1(buffer + 1);
    if (two < 0) {
        return -1; // Invalid second hex digit
    }
    
    return 16*one + two; // Combine the two hex digits into a decimal value

}

int read4(char *buffer) {
    int one = read2(buffer);
    if (one < 0) {
        return -1; // Invalid first two hex digits
    }
    int two = read2(buffer + 2);
    if (two < 0) {
        return -1; // Invalid second two hex digits
    }
    
    return 256 * one + two; // Combine the four hex digits into a decimal value
}

int readdata(int length, char *buffer[], int *out[]){
	if (length < 1){
		return -1;
	}
	for (i = 0; i < length; i++){
		*out[i] = read2(buffer + i * 2);
	}
	return 0;
}		

int readline (char *buffer){

    if (*buffer != ':'){

        printf("Error: Line does not start with ':'\n");
        return -1;

    }
    buffer++;
    int length = read2(buffer);
    if (length < 0) {
        printf("Error: Invalid length in line\n");
        return -1;
    }
	
    buffer += 2; // Move past the length bytes

    int address = read4(buffer);
    if (address < 0) {
        printf("Error: Invalid address in line\n");
        return -1;
    }
    buffer += 4; // Move past the address bytes
		
	int record = read2(buffer);
	if (record < 0) {
		printf("Error: Record type error");
	}
	buffer += 2;
		
	int data[125];
	readdata(length, buffer, data);
	if (data < 0) {
		printf("Error: Data has no length.\n");
	}
	
    printf("Length: %d, Address: %04X\n, record = %02X\n, data = %064X\n", length, address, record, data);

    return 0;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file;
    file = fopen(argv[1], "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        //printf("%s", buffer);
        if (readline(buffer) < 0) {
            break;
        }

    }
    fclose(file);
    return 0;
}

