#include "data.h"

extern void fileCreationFailure(char *fileName);

/* Function pointer to getFileNamePath */
static char *(*baseFileName)() = &getFileNamePath;

/* Function to export files (main handler) */
void exportFilesMainHandler()
{
    printf("Finished Successfully, about to export files!\n");
    generateObFile();
    if (areEntriesExist())
        createEntriesFile();
    if (areExternalsExist())
        createExternalsFile();
}

/* Function to generate .ob file */
void generateObFile()
{
    FILE *ob;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ob");
    ob = fopen(fileName, "w+");

    if (ob != NULL)
    {
        writeMemoryImageToObFile(ob);
        fclose(ob);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}

/* Function to create .ent file */
void createEntriesFile()
{
    FILE *ent;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ent");
    ent = fopen(fileName, "w+");

    if (ent != NULL)
    {
        writeEntriesToFile(ent);
        fclose(ent);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}

/* Function to create .ext file */
void createExternalsFile()
{
    FILE *ext;
    char *fileName = (*baseFileName)();
    strcat(fileName, ".ext");
    ext = fopen(fileName, "w+");
    if (ext != NULL)
    {
        writeExternalsToFile(ext);
        fclose(ext);
        free(fileName);
    }
    else
        fileCreationFailure(fileName);
}
