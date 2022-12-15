/*
	Psy-Q Symbol Extractor
	By Devon Artmeier 2022
	
	Grabs symbol data from a generated Psy-Q symbol file
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char sig[4];
	char numStr[16];
	FILE *inFile;
	FILE *outFile;
	uint32_t value;
	char nameLen;
	char *name;
	char *equ = " EQU $";

	if (argc < 3) {
		printf("Usage: Get-PsyQ-Syms [input] [output]");
		return 1;
	}

	inFile = fopen(argv[1], "rb");
	if (!inFile) {
		printf("Error: Cannot open \"%s\" for reading.", argv[1]);
		return 1;
	}

	outFile = fopen(argv[2], "wb");
	if (!outFile) {
		fclose(inFile);
		printf("Error: Cannot open \"%s\" for writing.", argv[2]);
		return 1;
	}

	/* Check signature */
	fread(sig, 1, 3, inFile);
	sig[3] = 0;
	if (strcmp(sig, "MND")) {
		printf("Error: Not a valid symbols file.");
		return 1;
	}
	fseek(inFile, 8, SEEK_SET);

	while (1) {
		/* Read value */
		if (fread(&value, 4, 1, inFile) != 1) {
			/* End of file */
			break;
		}
#ifdef _MSC_VER
		_itoa(value, numStr, 16);
#else
		itoa(value, numStr, 16);
#endif

		/* Read name */
		fgetc(inFile);
		nameLen = fgetc(inFile);
		name = malloc(nameLen + 1);
		name[nameLen] = 0;
		fread(name, 1, nameLen, inFile);

		/* Write to output file */
		fputs(name, outFile);
		fputs(equ, outFile);
		fputs(numStr, outFile);
		fputc('\n', outFile);

		free(name);
	}

	fclose(inFile);
	fclose(outFile);

	return 0;
}