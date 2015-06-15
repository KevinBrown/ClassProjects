/**
 * Systems Software
 * Project 2
 * 6/14/2015
 *
 * @author Kevin Brown
 * @author Chad Armstrong
 * @license MIT
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char* determine_keyword ( char first_statement_char, bool change_state );
char* disambiguate_keyword ( char secondChar );
bool keyword_is_valid ( char keyword[], char validKeyword, int keywordRemainderLength );
int get_token_id_from_string ( char *tokenChar );
char get_input();
bool manage_file_pointers ( char action[] );
void error_message_exit ( char* errorCode );
void print_token_output( char* tokenName );
void run_program();

FILE * input, * cleanoutput, * lexemetable, * lexemelist;

int state = 0;
char outputBuffer[2000];

/**
 * 1 = begin
 * 2 = call/const
 * 3 = do
 * 4 = end/else
 * 5 = if
 * 6 = then
 * 7 = procedure
 * 8 = read
 * 9 = var
 * 10 = while/write
 * 11 = epsilon
 * 12 = odd
 *
 * -1 =  error
 *
 * 21 = call
 * 22 = const
 *
 * 31 = end
 * 32 = else
 *
 * 41 = while
 * 42 = write
 *
 * 51 = / character of possible comment
 * 52 = * start of comment
 * 53 = * possible end of comment
 */

/*
 * null = 1
 * ident = 2
 * number = 3
 * plus = 4
 * minus = 5
 * multiplication = 6
 * slash = 7
 * odd = 8
 * equals = 9
 * not equals = 10
 * less than = 11
 * less than or equal to = 12
 * greater than = 12
 * greater than or equal to = 13
 * left parent = 15
 * right parent = 16
 * comma = 17
 * semi colon 18
 * period = 19
 * becomes = 20
 *
 * Actual Control:
 * begin = 21
 * end = 22
 * if = 23
 * then = 24
 * while = 25
 * do = 26
 * call = 27
 * const = 28
 * var = 29
 * procedure = 30
 * write = 31
 * read = 32
 * else = 33
 */

void main () {
    state = 0;
    manage_file_pointers("open");
    fprintf(lexemetable, "lexeme\ttoken type");
    run_program();
<<<<<<< HEAD
    manage_file_pointers("close");

=======
>>>>>>> origin/master
}


/**
 *  Decision tree to determine what the next valid step is for a given first character of a control structure
 *
 *  Following case statements are used:
 *  case 1 - disambiguation between call or const
 *  case 2 - disambiguation between end or else
 *  case 3 - disambiguation between while or write
 *  error - did not encounter a valid keyword
 *
 * @param char the first character of the potential keyword
 * @returns int the expected state for the keyword
 */
char* determine_keyword ( char first_statement_char, bool change_state ) {
    switch ( first_statement_char ) {
        case 'b':
        case 'B':
            //begin
            if ( change_state ) { state = 1; }
            return "begin";
        case 'c':
        case 'C':
            //call or const
            if ( change_state ) {
                state = 2;
                disambiguate_keyword(get_input());
            }
            return "case1";
        case 'd':
        case 'D':
            // do keyword
            if ( change_state ) { state = 3; }
            return "do";
        case 'e':
        case 'E':
            //end or else
            if ( change_state ) {
                state = 4;
                disambiguate_keyword(get_input());
            }
            return "case2";
        case 'i':
        case 'I':
            //if
            if ( change_state ) { state = 5; }
            return "if";
        case 't':
        case 'T':
            //then
            if ( change_state ) { state = 6; }
            return "then";
        case 'o':
        case 'O':
            //odd
            //forgot this state so the number is 12 until we refactor
            if ( change_state ) { state = 12; }
            return "odd";
        case 'p':
        case 'P':
            //procedure
            if ( change_state ) { state = 7; }
            return "procedure";
        case 'r':
        case 'R':
            //read
            if ( change_state ) { state = 8; }
            return "read";
        case 'v':
        case 'V':
            //var
            if ( change_state ) { state = 9; }
            return "var";
        case 'w':
        case 'W':
            //while or write
            if ( change_state ) {
                state = 10;
                disambiguate_keyword(get_input());
            }
            return "case3";
        case '\222':
            if ( change_state ) { state = 11; }
            return "\222";
        case ':':
            if ( change_state ) { state = 12; }
            return ":=";
        case '/':
            if ( change_state ) { state = 51; }
        default:
            if ( change_state ) { state = -1; }
            return "error";
    }
}

/**
 * Disambiguates the second character of a given token
 *
 * If token not found returns string of "error"
 *
 * @param char secondChar
 * @returns char* String of name of token
 */
char* disambiguate_keyword ( char secondChar ) {
    switch ( state ) {
        case 2:
            if ( secondChar == 'a' ) {
                //call
                state = 21;
                return "call";
            } else if ( secondChar == 'o') {
                //const
                state = 22;
                return "const";
            } else {
                state = -1;
                return "error";
            }
        case 4:
            if ( secondChar == 'n' ) {
                //end
                state = 31;
                return "end";
            } else if ( secondChar == 'l' ) {
                // else
                state = 32;
                return "else";
            } else {
                state = -1;
                return "error";
            }
        case 10:
            if ( secondChar == 'h' ) {
                //while
                state = 41;
                return "while";
            } else if ( secondChar == 'r' ) {
                //write
                state = 42;
                return "write";
            } else {
                state = -1;
                return "error";
            }
        default:
            return "error";
    }
}

/**
 * Returns the id of any given valid token string or returns -1 on error
 *
 * @param char* string of token, full keyword if applicable
 * @returns int
 */
int get_token_id_from_string ( char *tokenChar ) {
    if ( strcmp( tokenChar, "\0" ) == 0 ) {
        //null
        return 1;
        //FIXME State 2
    } else if ( *tokenChar >= 48 && *tokenChar <= 57) {
        // any number
        return 3;
    } else if ( strcmp( tokenChar, "+" ) == 0 ) {
        return 4;
    } else if ( strcmp( tokenChar, "-" ) == 0 ) {
        return 5;
    } else if ( strcmp( tokenChar, "*" ) == 0 ) {
        return 6;
    } else if ( strcmp( tokenChar, "/" ) == 0 ) {
        return 7;
    } else if ( strcmp( tokenChar, "odd" ) == 0 ) {
        return 8;
    } else if ( strcmp( tokenChar, "=" ) == 0 ) {
        return 9;
    } else if ( strcmp( tokenChar, "!" ) == 0 ) {
        return 10;
    } else if ( strcmp( tokenChar, "<" ) == 0 ) {
        return 11;
    } else if ( strcmp( tokenChar, "<=" ) == 0 ) {
        return 12;
    } else if ( strcmp( tokenChar, ">" ) == 0 ) {
        return 13;
    } else if ( strcmp( tokenChar, ">=" ) == 0 ) {
        return 14;
    } else if ( strcmp( tokenChar, "(" ) == 0 ) {
        return 15;
    } else if ( strcmp( tokenChar, ")" ) == 0 ) {
        return 16;
    } else if ( strcmp( tokenChar, "," ) == 0 ) {
        return 17;
    } else if ( strcmp( tokenChar, ";" ) == 0 ) {
        return 18;
    } else if ( strcmp( tokenChar, "." ) == 0 ) {
        return 19;
    } else if ( strcmp( tokenChar, "becomes" ) == 0 ) {
        return 20;
    } else if ( strcmp( tokenChar, "begins" ) == 0 ) {
        return 21;
    } else if ( strcmp( tokenChar, "end" ) == 0 ) {
        return 22;
    } else if ( strcmp( tokenChar, "if" ) == 0 ) {
        return 23;
    } else if ( strcmp( tokenChar, "then" ) == 0 ) {
        return 24;
    } else if ( strcmp( tokenChar, "while" ) == 0 ) {
        return 25;
    } else if ( strcmp( tokenChar, "do" ) == 0 ) {
        return 26;
    } else if ( strcmp( tokenChar, "call" ) == 0 ) {
        return 27;
    } else if ( strcmp( tokenChar, "const" ) == 0 ) {
        return 28;
    } else if ( strcmp( tokenChar, "var" ) == 0 ) {
        return 29;
    } else if ( strcmp( tokenChar, "proc" ) == 0 ) {
        return 30;
    } else if ( strcmp( tokenChar, "write" ) == 0 ) {
        return 31;
    } else if ( strcmp( tokenChar, "read" ) == 0 ) {
        return 32;
    } else if ( strcmp( tokenChar, "else" ) == 0 ) {
        return 33;
    } else if ( (*tokenChar >= 65 && *tokenChar <=90) || (*tokenChar >= 97 && *tokenChar <= 122) ) {
        //variable
        return 2;
    } else {
        return -1;
    }
}

/**
 * Gets and input from the file stream and returns false if at end of input
 *
 * @returns char
 */
char get_input () {
    char inputchar;

    if ( ( inputchar = fgetc( input ) ) != EOF ) {
        return inputchar;
    } else {
        return '0';
    }
}

/**
 * Either opens or closes all file pointers, does not validate success of file pointer operation
 *
 * @param char[] either open or close
 * @returns bool false if action sring is not valid
 */
bool manage_file_pointers ( char action[] ) {
    if ( strcmp( "open", action ) == 0 ) {
        input = fopen( "input.txt", "r" );
        cleanoutput = fopen( "cleanoutput.txt", "w" );
        lexemetable = fopen( "lememetable.txt", "w" );
        lexemelist = fopen( "lexmelist.txt", "w");
    } else if ( strcmp( "close", action ) == 0 ) {
        fclose(input);
        fclose(cleanoutput);
        fclose(lexemelist);
        fclose(lexemetable);
    } else {
        return false;
    }

    return true;
}

/**
 *  A function to call if the input file contains inappropriate format
 *
 *  Error codes don't have any reason to them, they are just unique digits so you can find
 *
 *  @param char* the exit code you'd like to display, should be unique
 */
void error_message_exit ( char*errorCode) {
    printf( "There was an error reading in the input. Location: %s", errorCode);
    exit( -1 );
}

/**
 * Finds and Prints output of token to all relevant output files based on token ID
 *
 * @param *char Name of Token to be printed to file
 */
void print_token_output( char* tokenName ) {

    if( strcmp(tokenName, "\n") == 0 ){
        fprintf(cleanoutput, "%s", outputBuffer);
        memset(outputBuffer, 0, sizeof(outputBuffer));
    } else {
        strcat(outputBuffer, tokenName);
    }

    int token = get_token_id_from_string( tokenName );
    fprintf( lexemetable, "%s\t\t%d", tokenName, token);
    fprintf( lexemelist, "%d ", token );
    if(token ==2){
        fprintf( lexemelist, "%s ", tokenName );
    }

}

/**
 *
 */
void run_program(){
    char* keyword;
    char currentChar;
    int numCharsToEnd = 0;
    int curTokenId;

    manage_file_pointers( "open" );

    while ( ( currentChar = get_input() ) != '0' ) {
        // comment handling
        if ( state == 51 ) {
            if ( currentChar == '*' ) {
                state = 52;
                continue;
            } else {
                // division sign
                state = 0;
                // FIXME: print division sign token needed
            }
        }

        if ( state == 52 ) {
            // we are inside a comment, ignore everything until end of comment or end of file
            if ( currentChar == '*' ) {
                // possible end of comment
                state = 53;
            }
            continue;
        }

        if ( state == 53 ) {
            if ( currentChar == '/') {
                // congrats we hit the end of the comment
                state = 0;
            } else {
                state = 52;
            }
            continue;
        }

        // if the state is 0 we are not currently within a token, this token's output does not rely on the previous one's
        // if state is 0, the program looks for a new keyword
        if ( state == 0 ) {
            keyword = determine_keyword( currentChar, true);
            curTokenId = get_token_id_from_string( keyword );
            numCharsToEnd = strlen( keyword ) - 1;

            if ( *keyword == "error" ) {
                // we didn't get a token as a multicharacter string, check for single character tokens
                curTokenId = get_token_id_from_string( currentChar );

                if ( curTokenId == -1 ) {
                    error_message_exit( 134238 );
                }
            }
        }   //end of if(state == 0)

        // disambiguate next operation
        if ( state == 2 || state == 4 || state == 10 ) {
            disambiguate_keyword( currentChar );    //if the keyword being examined starts with a "c" "e" or "w", it checks which keyword is intended
            if ( state == -1 ) {                    //if the keyword can't be determined the program exits with an error
                error_message_exit( 239492 );       //calls a function that closes program
            }
        } else if ( state > 0 && state < 50 ) {
            if (strcmp(keyword[strlen(keyword) - numCharsToEnd], keyword    ) != 0) {
                printf( "Keyword %s does not have all characters matching the correct value. Syntax Error.", keyword );
                error_message_exit( 121858 );
            }

            if ( numCharsToEnd > 1 ) {
                if ( !keyword_is_valid( *keyword, curTokenId, numCharsToEnd ) ) {

                }
                --numCharsToEnd;
                continue;
            } else {
                state = 0;
                --numCharsToEnd;
                continue;
            }
        }
    }

    manage_file_pointers( "close" );
}
