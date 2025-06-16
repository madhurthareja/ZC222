/*
 * This code is authored by Madhur Thareja, 2023ebcs412
 * Lexical Analyser with Floating Point Support - Extended Tutorial 2
 * Purpose: This lexer version extends the basic lexer by recognising floating-point numbers
 * (e.g., 3.14, 0.001) in addition to unsigned integers, keywords, operators, and identifiers.
 * 
 * How to Compile and Run:
 * 1. Save this file as "lexer_float.c".
 * 2. Open a terminal and navigate to the directory where the file is saved.
 * 3. Compile the code using the command:
 *      gcc lexer_float.c -o lexer_float
 * 4. Run the lexer with the command:
 *      ./lexer_float
 * 5. Enter a string when prompted to see recognised tokens, including floating-point numbers.
 *
 * This version introduces a TOKEN_FLOAT type, allowing the lexer to classify floating-point literals.
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
    TOKEN_FLOAT,            // Floating-point number (e.g., 3.14)
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
    DOT_SEEN,          // State after seeing a '.' after digits (waiting for digits)
    FLOAT,             // State for recognising floating-point numbers
    IDENTIFIER_PREFIX, // State when "id" is recognised as an identifier prefix
    IDENTIFIER,        // State for continuing identifier recognition
    OPERATOR,          // State for recognising operators
    ERROR              // Error state for invalid inputs
};

// Character classes based on character type (letters, digits, operators, dot, unknown)
enum
{
    CHAR_LETTER,   // Alphabetic characters
    CHAR_DIGIT,    // Digits (0-9)
    CHAR_OPERATOR, // Arithmetic operators (+, -, *, /)
    CHAR_DOT,      // Decimal point '.'
    CHAR_UNKNOWN   // Any other character
};

// Transition table (rows: states, columns: character classes)
// Defines how the DFA transitions between states for each input character type
// Columns: CHAR_LETTER, CHAR_DIGIT, CHAR_OPERATOR, CHAR_DOT, CHAR_UNKNOWN
int transitionTable[10][5] = {
    // CHAR_LETTER,  CHAR_DIGIT,  CHAR_OPERATOR, CHAR_DOT,   CHAR_UNKNOWN
    {IDENTIFIER_PREFIX, UNSIGNED_INTEGER, OPERATOR, ERROR, ERROR}, // START
    {ERROR, ERROR, ERROR, ERROR, ERROR},                           // IN_KEYWORD
    {ERROR, ERROR, ERROR, ERROR, ERROR},                           // OUT_KEYWORD
    {ERROR, UNSIGNED_INTEGER, ERROR, DOT_SEEN, ERROR},             // UNSIGNED_INTEGER
    {ERROR, FLOAT, ERROR, ERROR, ERROR},                           // DOT_SEEN (must see digit after '.')
    {ERROR, FLOAT, ERROR, ERROR, ERROR},                           // FLOAT (digits after '.')
    {IDENTIFIER, ERROR, ERROR, ERROR, ERROR},                      // IDENTIFIER_PREFIX
    {IDENTIFIER, IDENTIFIER, ERROR, ERROR, ERROR},                 // IDENTIFIER
    {ERROR, ERROR, ERROR, ERROR, ERROR},                           // OPERATOR
    {ERROR, ERROR, ERROR, ERROR, ERROR}                            // ERROR
};

// Function to classify characters into CHAR_LETTER, CHAR_DIGIT, CHAR_OPERATOR, CHAR_DOT, or CHAR_UNKNOWN
int getCharClass(char c)
{
    if (isalpha(c)) // Check if the character is an alphabet
        return CHAR_LETTER;
    if (isdigit(c)) // Check if the character is a digit
        return CHAR_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/') // Check for arithmetic operators
        return CHAR_OPERATOR;
    if (c == '.') // Check for decimal point
        return CHAR_DOT;
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

    // Recognise float: must end in FLOAT state and contain at least one digit before and after '.'
    if (state == FLOAT)
    {
        // Check for at least one digit before and after '.'
        const char *dot = strchr(input, '.');
        if (dot != NULL && dot != input) // dot not at start
        {
            int before = dot - input;
            int after = strlen(dot + 1);
            if (before > 0 && after > 0)
                return TOKEN_FLOAT;
        }
    }

    if (state == UNSIGNED_INTEGER)
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
        case TOKEN_FLOAT:
            tokenName = "Floating Point";
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

//This code is authored by Madhur Thareja, 2023ebcs412