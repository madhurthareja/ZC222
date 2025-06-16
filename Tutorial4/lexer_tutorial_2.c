/*
 * Lexical Analyser with Unsigned Integer Support - Tutorial 2
 * Purpose: This lexer version extends the basic lexer by recognising unsigned integers
 * (whole numbers without any sign, e.g., 123, 456). The lexer now distinguishes between:
 * - Keywords ("in", "out")
 * - Arithmetic operators (+, -, *, /)
 * - Identifiers (starting with "id")
 * - Unsigned integers
 *
 * How to Compile and Run:
 * 1. Save this file as "lexer_unsigned_int.c".
 * 2. Open a terminal and navigate to the directory where the file is saved.
 * 3. Compile the code using the command:
 *      gcc lexer_unsigned_int.c -o lexer_unsigned_int
 * 4. Run the lexer with the command:
 *      ./lexer_unsigned_int
 * 5. Enter a string when prompted to see recognised tokens, including unsigned integers.
 *
 * This version introduces a TOKEN_UNSIGNED_INTEGER type, allowing the lexer to classify numeric literals.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Token types that the lexer will recognise
typedef enum
{
    TOKEN_KEYWORD_IN,       // The keyword "in"
    TOKEN_KEYWORD_OUT,      // The keyword "out"
    TOKEN_UNSIGNED_INTEGER, // A sequence of digits
    TOKEN_OPERATOR,         // Arithmetic operators (+, -, *, /)
    TOKEN_IDENTIFIER,       // An identifier starting with "id"
    TOKEN_UNKNOWN           // An unknown token that doesn't match any rule
} TokenType;

// DFA states to process each character of the input
enum
{
    START,             // Starting state
    IN_KEYWORD,        // State for recognising the "in" keyword
    OUT_KEYWORD,       // State for recognising the "out" keyword
    UNSIGNED_INTEGER,  // State for recognising integers
    IDENTIFIER_PREFIX, // State when "id" is recognised as an identifier prefix
    IDENTIFIER,        // State for continuing identifier recognition
    OPERATOR,          // State for recognising operators
    ERROR              // Error state for invalid inputs
};

// Character classes based on character type (letters, digits, operators, unknown)
enum
{
    CHAR_LETTER,   // Alphabetic characters
    CHAR_DIGIT,    // Digits (0-9)
    CHAR_OPERATOR, // Arithmetic operators (+, -, *, /)
    CHAR_UNKNOWN   // Any other character
};

// Transition table (rows: states, columns: character classes)
// Defines how the DFA transitions between states for each input character type
int transitionTable[8][4] = {
    // CHAR_LETTER,  CHAR_DIGIT,  CHAR_OPERATOR, CHAR_UNKNOWN
    {IDENTIFIER_PREFIX, UNSIGNED_INTEGER, OPERATOR, ERROR}, // START
    {ERROR, ERROR, ERROR, ERROR},                           // IN_KEYWORD
    {ERROR, ERROR, ERROR, ERROR},                           // OUT_KEYWORD
    {ERROR, UNSIGNED_INTEGER, ERROR, ERROR},                // UNSIGNED_INTEGER
    {IDENTIFIER, ERROR, ERROR, ERROR},                      // IDENTIFIER_PREFIX
    {IDENTIFIER, IDENTIFIER, ERROR, ERROR},                 // IDENTIFIER
    {ERROR, ERROR, ERROR, ERROR},                           // OPERATOR
    {ERROR, ERROR, ERROR, ERROR}                            // ERROR
};

// Function to classify characters into CHAR_LETTER, CHAR_DIGIT, CHAR_OPERATOR, or CHAR_UNKNOWN
int getCharClass(char c)
{
    if (isalpha(c)) // Check if the character is an alphabet
        return CHAR_LETTER;
    if (isdigit(c)) // Check if the character is a digit
        return CHAR_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/') // Check for arithmetic operators
        return CHAR_OPERATOR;
    return CHAR_UNKNOWN; // Any other character is classified as unknown
}

// Function to recognise the type of token from the input string
TokenType recogniseToken(const char *input)
{
    int state = START; // Start at the initial state
    int i = 0;

    // Process each character of the input string
    while (input[i] != '\0')
    {
        char c = input[i];
        int charClass = getCharClass(c); // Get character class

        // Transition to the next state based on current state and character class
        state = transitionTable[state][charClass];

        i++;
    }

    // Determine token type based on the final state after processing all characters
    if (state == IDENTIFIER && strncmp(input, "id", 2) == 0)
        return TOKEN_IDENTIFIER;       // If the string starts with "id", it's an identifier
    if (state == UNSIGNED_INTEGER)     // IMPLEMENTATION CHANGE
        return TOKEN_UNSIGNED_INTEGER; // If final state is UNSIGNED_INTEGER, return that type
    if (state == OPERATOR && strlen(input) == 1)
        return TOKEN_OPERATOR; // If it is a single operator, return operator type
    if (strcmp(input, "in") == 0)
        return TOKEN_KEYWORD_IN; // Recognise the "in" keyword
    if (strcmp(input, "out") == 0)
        return TOKEN_KEYWORD_OUT; // Recognise the "out" keyword

    return TOKEN_UNKNOWN; // If no rules match, return unknown token type
}

// Main function to tokenise the input string
void lexer(const char *input)
{
    const char *delimiters = " \t\n"; // Delimiters to split the input string (space, tab, newline)
    char buffer[100];
    strcpy(buffer, input); // Copy input to buffer to avoid modifying the original string

    // Tokenise the input using strtok, splitting by the delimiters
    char *token = strtok(buffer, delimiters);
    while (token != NULL)
    {
        // Recognise the type of the token
        TokenType type = recogniseToken(token);
        const char *tokenName;

        // Map the recognised token type to a human-readable name
        switch (type)
        {
        case TOKEN_KEYWORD_IN:
            tokenName = "Keyword 'in'";
            break;
        case TOKEN_KEYWORD_OUT:
            tokenName = "Keyword 'out'";
            break;
        case TOKEN_UNSIGNED_INTEGER:
            tokenName = "Unsigned Integer";
            break;
        case TOKEN_OPERATOR:
            tokenName = "Operator";
            break;
        case TOKEN_IDENTIFIER:
            tokenName = "Identifier";
            break;
        default:
            tokenName = "Unknown"; // If no match, label the token as unknown
            break;
        }

        // Print the token type and its value
        printf("Token: %s; String: %s\n", tokenName, token);

        // Move to the next token
        token = strtok(NULL, delimiters);
    }
}

int main()
{
    char input[100];

    // Prompt user to enter a string for tokenisation
    printf("Enter a string to tokenise: ");
    fgets(input, sizeof(input), stdin); // Read the input string

    // Call the lexer to tokenise the input string
    lexer(input);

    return 0;
}
