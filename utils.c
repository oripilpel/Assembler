#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"

typedef struct Word
{
    char *str;
    int end_idx;
} Word;

/* returns a word structure that has a pointer to the nth word in a line or null if not found and end index of current word */
Word *get_next_word(char *line, int n)
{
    int had_comma = OFF;
    int skipped = 0;
    char *word_start;
    char *curr_word;
    size_t length = 0;
    Word *word;
    line += n;
    word = (Word *)malloc(sizeof(Word));
    if (line == NULL || *line == '\0')
    { /* string is NULL or empty return NULL */
        return NULL;
    }
    while (*line != '\0')
    {
        while (isspace(*line))
        {
            line++;
            skipped++;
        } /* ignore whitespaces */
        if (*line == ',' && !had_comma)
        {
            line++;
            skipped++;
            had_comma = ON;
        }
        if (!isspace(*line))
        {
            length = 0;
            word_start = line;
            while (*line != ',' && !isspace(*line) && *line != '\0')
            {
                line++;
                length++;
            }
            if (!length)
            {
                return NULL;
            }
            curr_word = (char *)malloc(sizeof(char) * (length + 1)); /* allocate memory for nth word */
            strncpy(curr_word, word_start, length);
            curr_word[length] = '\0';
            word->str = curr_word;
            word->end_idx = n + length + skipped;
            return word;
        }
    }
    return NULL; /* not found next word is empty or comma */
}

/* returns a pointer to the nth word in a line or null if not found */
char *get_nth_word(char *line, int n)
{
    int added_in_curr_iteration = OFF; /* indicator if added 1 to word count in the current iteration */
    char *word_start;
    char *nth_word;
    int word_count = 0;
    size_t length = 0;
    if (line == NULL || *line == '\0')
    { /* string is NULL return NULL */
        return NULL;
    }
    while (*line != '\0')
    {
        if (*line != ',' && !isspace(*line))
        {
            length = 0;
            word_count++;
            added_in_curr_iteration = ON;
            word_start = line;
            while (*line != ',' && !isspace(*line) && *line != '\0')
            {
                line++;
                length++;
            }
            if (n == word_count)
            {                                                           /* found nth word */
                nth_word = (char *)malloc(sizeof(char) * (length + 1)); /* allocate memory for nth word */
                strncpy(nth_word, word_start, length);
                nth_word[length] = '\0';
                return nth_word;
            }
        }
        while (isspace(*line))
            line++; /* ignore whitespaces */
        if (*line == ',')
        {
            line++;
            if (!added_in_curr_iteration)
                word_count++;
        }
        added_in_curr_iteration = OFF;
    }
    return NULL; /* not found nth word return NULL */
}

/* count the words seperated by commas and ignores whitespaces */
int count_words(char *line)
{
    int added_in_curr_iteration = OFF; /* indicator if added 1 to word count in the current iteration */
    int word_count = 0;
    if (line == NULL || *line == '\0')
    { /* string is NULL return 0 */
        return 0;
    }
    while (*line != '\0')
    {
        if (*line != ',' && !isspace(*line))
        {
            word_count++;
            added_in_curr_iteration = ON;
            while (*line != ',' && !isspace(*line) && *line != '\0')
            {
                line++;
            }
        }
        while (isspace(*line))
            line++; /* ignore whitespaces */
        if (*line == ',')
        {
            line++;
            if (!added_in_curr_iteration)
                word_count++;
        }
        added_in_curr_iteration = OFF;
    }
    return word_count;
}

/* returns 1 if all the line is space else 0 */
int is_all_space(char *start)
{
    if (start == NULL)
    {
        return 1;
    }
    while (*start != '\0')
    {
        if (!isspace(*start))
        {
            return 0;
        }
        start++;
    }
    return 1;
}

int validate_number(char *number)
{
    char *number_start;
    char *number_copy;
    int num;

    while (isspace(*number))
    {
        number++;
    }

    number_start = number;

    if (*number == '+' || *number == '-')
        number++; /* it could start with plus or minus sign */

    /* check for NULL or an empty string */
    if (!number || *number == '\0')
        return INVALID;

    while (*number != '\0' && isdigit(*number))
    {
        number++;
    }

    /* allocate memory for the number_copy */
    number_copy = (char *)malloc((number - number_start + 1) * sizeof(char));
    strncpy(number_copy, number_start, number - number_start);
    number_copy[number - number_start] = '\0';

    num = atoi(number_copy);

    if (num > MAX_NUM_VALUE || num < MIN_NUM_VALUE)
    {
        printf("number value %d is not in allowed range\n", num); /* print the integer value */
        free(number_copy);
        return INVALID;
    }

    free(number_copy);

    while (isspace(*number))
    {
        number++;
    }

    /* return 1 to indicate success (all characters were processed) */
    return *number == '\0';
}

/* validate all numbers in string */
int validate_numbers(char *numbers)
{
    char *numbers_copy;
    char *curr_num;
    if (*numbers == ',' || numbers[strlen(numbers) - 1] == ',')
        return INVALID;
    numbers_copy = (char *)malloc(sizeof(char) * (strlen(numbers) + 1));
    strcpy(numbers_copy, numbers);
    numbers_copy[strlen(numbers)] = '\0';
    curr_num = strtok(numbers_copy, ",");
    while (curr_num)
    {
        if (!validate_number(curr_num))
        {
            return INVALID;
        }
        curr_num = strtok(NULL, ",");
    }
    return VALID;
}

/* returns if a given char is within ASCII range */
int is_ascii(char ch)
{
    return (ch >= 0 && ch <= 127);
}

/* return if current word is .data or .string */
int is_data_storing(char *word)
{
    return !strcmp(word, ".data") || !strcmp(word, ".string");
}

/* return if current word is .entry or .extern */
int is_entry_or_extern(char *word)
{
    return !strcmp(word, ".entry") || !strcmp(word, ".extern");
}

/* validating data */
int validate_string_data(char *data)
{
    int valid = ON;
    char *data_end = data + strlen(data) - 1;
    if (!data)
    {
        valid = OFF;
        printf("data is falsy");
    }
    while (isspace(*data_end))
    {
        data_end--;
    }
    if (*data_end != '"' || *data != '"')
    {
        valid = OFF;
        printf("invalid data - data string should start and end with double quotes\n");
    }
    while (*data != '\0')
    {
        if (!is_ascii(*data))
        { /* all chars in string must be ASCII chars */
            valid = OFF;
            printf("invalid data - data string contains non ascii characters\n");
        }
        data++;
    }

    return valid;
}

/* returns data length */
int get_data_length(int data_type, char *data)
{
    char *str_end;
    str_end = data + strlen(data) - 1;
    if (data_type == STRING_DATA_TYPE)
    {
        while (isspace(*str_end))
        {
            str_end--;
        }
        return str_end - data;
    }
    return count_words(data);
}

/* initialize string with zeros */
void init_string(char *s)
{
    int i = 0;

    while (i < OUTPUT_LINE_LENGTH - 1)
    {
        s[i] = '0';
        i++;
    }
    s[OUTPUT_LINE_LENGTH - 1] = '\0';
}

/* returns ascii char converted to binary */
char *ascii_char_to_binary(char ascii_char)
{
    char *binary;
    int i;
    int ascii_value = (int)ascii_char;
    binary = (char *)malloc(8 + 1);

    for (i = 7; i >= 0; i--)
    {
        binary[7 - i] = ((ascii_value >> i) & 1) + '0';
    }
    binary[8] = '\0';

    return binary;
}

/* returns the value converted to binary */
char *int_to_binary_string(int number, int num_bits)
{
    char *binary;
    int i;
    binary = (char *)malloc(num_bits + 1);

    if (number < 0)
    {
        number = (1 << num_bits) + number;
    }

    for (i = num_bits - 1; i >= 0; i--)
    {
        binary[num_bits - 1 - i] = ((number >> i) & 1) + '0';
    }
    binary[num_bits] = '\0';

    return binary;
}