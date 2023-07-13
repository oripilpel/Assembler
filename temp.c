#include <stdio.h>
#include <string.h>

void processLine() {
    char line[81];  // Assuming a maximum line length of 80 characters plus null terminator
    printf("Enter a line: ");
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = '\0';  // Remove the newline character if present

    if (strlen(line) > 80) {
        line[80] = '\0';  // Truncate the line to 80 characters
    }

    printf("%s\n", line);
}

int main() {
    processLine();
    return 0;
}
