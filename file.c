#include <stdio.h>
#include "constants.h"
#include <ctype.h>

int check_line_type(char *line)
{
    if (strncmp(line, ";", 1) == 0)
    {
        return COMMENT_LINE_CODE;
    }
    if (isspace(line))
    {
        return EMPTY_LINE_CODE;
    }
    if (strncmp(line, "mcro", 4) == 0)
    {
        return MACRO_LINE_CODE;
    }
    return 0;
}

int read_file_and_validate_file(char *file_name)
{
    lines = [];
    FILE *file;
    char line[MAX_LENGTH];

    file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1; // Exit the program with an error
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int line_type = check_line_type(line);
        if (line_type == 0)
        {
        }
    }

    // Close the file
    fclose(file);

    return 0;
}
