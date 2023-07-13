#include <stdio.h>
#include <ctype.h>
#include "linked_list.h"
#include "constants.h"

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
    struct Node *head = NULL;

    FILE *file;
    char line[MAX_LENGTH];
    int line_type;
    file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1; // Exit the program with an error
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_type = check_line_type(line);
        switch (line_type)
        {
        case EMPTY_LINE_CODE:
        case COMMENT_LINE_CODE:
            line ='';
            break;
        case MACRO_LINE_CODE:
            handle_macro();
        }
    }

    // Close the file
    fclose(file);

    return 0;
}
