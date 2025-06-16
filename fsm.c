/*
 * Purpose:
 * This program determines if a binary string ends with the character '1'.
 * It uses a finite-state machine with two states:
 * - State 0: Indicates that the last character was '0' or the string does not end with '1'.
 * - State 1: Indicates that the last character was '1', meaning the string ends with '1'.
 *
 * The program will print "The string is accepted (ends with 1)." if the last character in the
 * binary input string is '1'. Otherwise, it prints "The string is rejected (does not end with 1)."
 *
 * Execution:
 * 1. Compile the code using a C compiler (e.g., `gcc program.c -o program`).
 * 2. Run the compiled program (e.g., `./program`).
 *
 * Note: The program initialises a fixed input of {'1', '0', '1', '0'}.
 * To modify the input, change the values in the `input` array.
 */

#include <stdio.h>

int main()
{
    char input[100] = {'1', '0', '1', '1'};

    int state = 0; // Start at state 0
    int index = 0;
    int n = 4;

    // Loop through the input characters
    while (index < n)
    {
        switch (state)
        {
        case 0: // Initial state
            if (input[index] == '0')
            {
                state = 0; // Stay in state 0
            }
            else
            {
                state = 1; // Move to state 1
            }
            break;

        case 1: // State for strings that end with 1
            if (input[index] == '0')
            {
                state = 0; // Go back to state 0
            }
            else
            {
                state = 1; // Stay in state 1
            }
            break;
        }
        index++;
    }

    if (state == 1)
    {
        printf("The string is accepted (ends with 1).\n");
    }
    else
    {
        printf("The string is rejected (does not end with 1).\n");
    }

    return 0;
}
