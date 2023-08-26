#include "file.h"
#include "constants.h"
#include "string.h"

int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strlen(argv[i]) > MAX_FILE_NAME_LENGTH - 8) /* it's bigger than the filename and the max appended extention */
        {
            continue;
        }
        macro_expansion(argv[i]);
        first_run(argv[i]);
    }
    return NO_ERROR_CODE;
}