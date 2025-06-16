/*
 * Purpose:
 * This program uses a finite-state machine and a transition table to determine if a binary string
 * ends with the character '1'.
 *
 * The program maintains two states:
 * - State 0: Indicates that the string does not end with '1' (last character is '0').
 * - State 1: Indicates that the string ends with '1'.
 *
 * Based on the last character, the program will print "The string is accepted (ends with 1)."
 * if the string ends in '1', or "The string is rejected (does not end with 1)." if it does not.
 *
 * Execution:
 * 1. Compile the code using a C compiler (e.g., `gcc program.c -o program`).
 * 2. Run the compiled program (e.g., `./program`).
 *
 * Note: The program initialises a fixed input of {'1', '0', '1', '1'}.
 * To modify the input, change the values in the `input` array.
 */

#include <stdio.h>

#define STATE_0 0
#define STATE_1 1

// Transition table
int transitionTable[2][2] = {
    {STATE_0, STATE_1}, // Transitions from state 0
    {STATE_0, STATE_1}  // Transitions from state 1
};

// Function to get the input symbol index (0 for '0', 1 for '1')
int getInputIndex(char input)
{
    if (input == '0')
        return 0;
    if (input == '1')
        return 1;
    return -1; // Invalid input
}

int main()
{
    char input[100] = {'1', '0', '1', '1'};

    int currentState = STATE_0; // Start at state 0
    int index = 0;
    int n = 4;
    int inputIndex;

    // Process the input string
    while (index < n)
    {
        inputIndex = getInputIndex(input[index]);

        // If input is invalid, reject the string
        if (inputIndex == -1)
        {
            printf("The string is rejected (contains invalid characters).\n");
            return 0;
        }

        // Transition to the next state
        currentState = transitionTable[currentState][inputIndex];
        index++;
    }

    // Check if the string is accepted
    if (currentState == STATE_1)
    {
        printf("The string is accepted (ends with 1).\n");
    }
    else
    {
        printf("The string is rejected (does not end with 1).\n");
    }

    return 0;
}
