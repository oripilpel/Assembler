#include "lexer.h"

// Static variables for the memory image and counters
static BinaryWord *binaryImg = NULL;
static HexWord *hexImg = NULL;
unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;

// External function declarations
extern HexWord *convertBinaryWordToHex(BinaryWord *word);
extern char *numToBin(int num);
extern unsigned binaryStringToHexNumber(char binaryStr[4]);

// Getter functions for counters
unsigned getDC() { return DC; }
unsigned getIC() { return IC; }
unsigned getICF() { return ICF; }
unsigned getDCF() { return DCF; }

// Increment data counter
void increaseDataCounter(int amount) {
    DC += amount;
}

// Increment instruction counter
void increaseInstructionCounter(int amount) {
    IC += amount;
}

// Allocate memory image
void allocMemoryImg() {
    const int totalSize = DCF - MEMORY_START;
    int i, j;

    // Free previously allocated memory
    if (binaryImg != NULL)
        free(binaryImg);
    if (hexImg != NULL)
        free(hexImg);

    // Allocate memory for the image
    binaryImg = (BinaryWord *)malloc(totalSize * sizeof(BinaryWord));
    hexImg = (HexWord *)malloc(totalSize * sizeof(HexWord));

    // Initialize memory image
    for (i = 0; i < totalSize; i++) {
        hexImg[i]._A = 0;
        hexImg[i]._B = 0;
        hexImg[i]._C = 0;
        hexImg[i]._D = 0;
        hexImg[i]._E = 0;

        for (j = 0; j < BINARY_WORD_SIZE; j++) {
            binaryImg[i].digit[j].on = 0;
        }
    }
}

// Reset memory counters
void resetMemoryCounters() {
    IC = MEMORY_START;
    DC = 0;
    ICF = 0;
    DCF = 0;
}

// Print binary memory image
void printBinaryImg() {
    int i;
    int totalSize = DCF - MEMORY_START;

    // Print memory contents
    for (i = 0; i < totalSize; i++) {
        printf("%04d ", MEMORY_START + i);
        printWordBinary(i);
    }
}

// Add word to memory image based on data type
void addWord(int value, DataType type) {
    if (type == Code)
        addWordToCodeImage(numToBin(value));
    else if (type == Data)
        addWordToDataImage(numToBin(value));
}

// Add word to data image
void addWordToDataImage(char *s) {
    wordStringToWordObj(s, Data);
    DC++;
}

// Add word to code image
void addWordToCodeImage(char *s) {
    wordStringToWordObj(s, Code);
    IC++;
}

// Convert word string to word object and store in memory image
void wordStringToWordObj(char *s, DataType type) {
    int j;
    int index = type == Code ? IC - MEMORY_START : DC - MEMORY_START;
    for (j = 0; j < BINARY_WORD_SIZE; j++)
        binaryImg[index].digit[j].on = s[j] == '1' ? 1 : 0;
}

// Print binary representation of a word
void printWordBinary(unsigned index) {
    int j;
    for (j = 0; j < BINARY_WORD_SIZE; j++) {
        if (j % 4 == 0)
            printf(" ");
        printf("%d", binaryImg[index].digit[j].on ? 1 : 0);
    }
    printf("\n");
}

// Calculate final address counters values
void calcFinalAddrsCountersValues() {
    ICF = IC;
    DCF = ICF + DC;
    DC = IC;
    IC = MEMORY_START;
}

// Print memory image in required object file format
void printMemoryImgInRequiredObjFileFormat() {
    extern BinaryWord *binaryImg;
    extern HexWord *hexImg;
    int i;
    int totalSize = DCF - MEMORY_START;
    printf("%d %d\n", ICF - MEMORY_START, DCF - ICF);

    // Print memory contents in hex format
    for (i = 0; i < totalSize; i++) {
        hexImg[i] = *convertBinaryWordToHex(&binaryImg[i]);
        printf("%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, hexImg[i]._A, hexImg[i]._B, hexImg[i]._C, hexImg[i]._D, hexImg[i]._E);
    }
}

// Write memory image to object file
void writeMemoryImageToObFile(FILE *fp) {
    extern BinaryWord *binaryImg;
    extern HexWord *hexImg;
    int i;
    int totalSize = DCF - MEMORY_START;
    fprintf(fp, "%d %d\n", ICF - MEMORY_START, DCF - ICF);

    // Write memory contents to object file
    for (i = 0; i < totalSize; i++) {
        hexImg[i] = *convertBinaryWordToHex(&binaryImg[i]);
        fprintf(fp, "%04d A%x-B%x-C%x-D%x-E%x\n", MEMORY_START + i, hexImg[i]._A, hexImg[i]._B, hexImg[i]._C, hexImg[i]._D, hexImg[i]._E);
    }
}
