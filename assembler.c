#include "file.h"
#include "constants.h"

int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++)
    {
        macro_expansion(argv[i]);
        first_run(argv[i]);
    }
    return NO_ERROR_CODE;
}