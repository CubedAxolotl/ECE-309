#include <stdio.h>   // needed for printf, fgets, etc.
#include <string.h>  // needed for strstr, strcspn

int main(void) {
    // This buffer will hold whatever the user types.
    // 100 characters should be plenty for simple input.
    char input[100];

    // Start an infinite loop. We'll break out of it manually when needed.
    while (1) {
        // Ask the user for input.
        printf("Enter something (type 'exit' to quit): ");

        // fgets reads a line of text from the keyboard (stdin)
        // and stores it in 'input'. It also stores the newline character.
        // If fgets fails (e.g., end of input), we break out of the loop.
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // fgets keeps the newline character ('\n') at the end of the string.
        // strcspn finds the position of the first '\n' and we replace it
        // with '\0' (end of string) to remove it, so comparisons work cleanly.
        input[strcspn(input, "\n")] = '\0';

        // Check if the user typed exactly "exit".
        // strcmp returns 0 if the two strings are identical.
        if (strcmp(input, "exit") == 0) {
            printf("Exiting program. Goodbye!\n");
            break; // This exits the while loop, ending the program.
        }

        // Check if the word "hello" appears anywhere in the input.
        // strstr searches for a substring and returns NULL if not found.
        if (strstr(input, "hello") != NULL) {
            printf("Hello there! Nice to see you.\n");
        }
        else {
            // If it's not "exit" and doesn't contain "hello",
            // just echo the input back to the user.
            printf("You said: %s\n", input);
        }
    }

    // Return 0 to indicate the program finished successfully.
    return 0;
}
