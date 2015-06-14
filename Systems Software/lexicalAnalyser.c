//
// Created by Kevin on 6/12/2015.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

FILE * input, * cleanoutput, * lexemetable, * lexemelist;

int state = 0;

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
            //begin
            if ( change_state ) { state = 1; }
            return "begin";
        case 'c':
            //call or const
            if ( change_state ) { state = 2; }
            return "";
        case 'd':
            // do keyword
            if ( change_state ) { state = 3; }
            return "do";
        case 'e':
            //end or else
            if ( change_state ) { state = 4; }
            return "";
        case 'i':
            //if
            if ( change_state ) { state = 5; }
            return "if";
        case 't':
            //then
            if ( change_state ) { state = 6; }
            return "then";
        case 'p':
            //procedure
            if ( change_state ) { state = 7; }
            return "procedure";
        case 'r':
            //read
            if ( change_state ) { state = 8; }
            return "read";
        case 'v':
            //var
            if ( change_state ) { state = 9; }
            return "var";
        case 'w':
            //while or write
            if ( change_state ) { state = 10; }
            return "";
        case '\222':
            if ( change_state ) { state = 11; }
            return "\222";
        default:
            if ( change_state ) { state = -1; }
            return "";
    }
}

/**
 *
 */
bool disambiguate_keyword ( char secondChar ) {
    switch ( state ) {
        case 2:
            // call/const
            if ( secondChar == 'a' ) { state = 21; } //call
            else if ( secondChar == 'o') { state = 22; } //const
            else { state = -1; }
            return true;
        case 4:
            // end/else
            if ( secondChar == 'n' ) { state = 31; } //end
            else if ( secondChar == 'l' ) { state = 32; } // else
            else { state = -1; }
            break;
        case 10:
            // while/write
            if ( secondChar == 'h' ) { state = 41; } //while
            else if ( secondChar == 'r' ) { state = 42; } //write
            else { state = -1; }
            break;
        default:
            break;
    }
}

/**
 * Checks the remainder of a keyword string to determine if the remainder of the string is a valid keyword
 *
 * @param char keyword the keyword that you are testing against
 * @param int keywordRemainderLength the length of the keyword after the determination point is made, (will be either 2 or 1)
 * @returns boolean
 */
bool keyword_is_valid ( char keyword[], char validKeyword[], int keywordRemainderLength ) {
    for ( int i = 0; i < keywordRemainderLength; ++i ) {
        if ( keyword[i] != validKeyword[i] ) {
            return false;
        }
    }

    return true;
}

/**
 *
 *
 */
int get_token_id_from_string ( char *tokenChar ) {
   if ( strcmp( tokenChar, "\0" ) == 0 ) {
       //null
       return 1;
   } else if ( strcmp( tokenChar, "" ) ) {
       //variable
       return 2;/*
   } else if ( *tokenChar >= 48 && *tokenChar <= 57) {
       // any number
       return 3;
   } else if ( strcmp( *tokenChar, "+" ) == 0 ) {
       return 4;
   } else if ( strcmp( *tokenChar, "-" ) == 0 ) {
       return 5;
   } else if ( strcmp( *tokenChar, "*" ) == 0 ) {
       return 6;
   } else if ( strcmp( *tokenChar, "/" ) == 0 ) {
       return 7;
   } else if ( strcmp( *tokenChar, "" ) == 0 ) {
       return 8;
   } else if ( strcmp( *tokenChar, "=" ) == 0 ) {
       return 9;
   } else if ( strcmp( *tokenChar, "!" ) == 0 ) {
       return 10;
   } else if ( strcmp( *tokenChar, "<" ) == 0 ) {
       return 11;
   } else if ( strcmp( *tokenChar, "<=" ) == 0 ) {
       return 12;
   } else if ( strcmp( *tokenChar, ">" ) == 0 ) {
       return 13;
   } else if ( strcmp( *tokenChar, ">=" ) == 0 ) {
       return 14;
   } else if ( strcmp( *tokenChar, "(" ) == 0 ) {
       return 15;
   } else if ( strcmp( *tokenChar, ")" ) == 0 ) {
       return 16;
   } else if ( strcmp( *tokenChar, "," ) == 0 ) {
       return 17;
   } else if ( strcmp( *tokenChar, ";" ) == 0 ) {
       return 18;
   } else if ( strcmp( *tokenChar, "." ) == 0 ) {
       return 19;
   } else if ( strcmp( *tokenChar, "becomes" ) == 0 ) {
       return 20;
   } else if ( strcmp( *tokenChar, "begins" ) == 0 ) {
       return 21;
   } else if ( strcmp( *tokenChar, "ends" ) == 0 ) {
       return 22;
   } else if ( strcmp( *tokenChar, "if" ) == 0 ) {
       return 23;
   } else if ( strcmp( *tokenChar, "then" ) == 0 ) {
       return 24;
   } else if ( strcmp( *tokenChar, "while" ) == 0 ) {
       return 25;
   } else if ( strcmp( *tokenChar, "do" ) == 0 ) {
       return 26;
   } else if ( strcmp( *tokenChar, "call" ) == 0 ) {
       return 27;
   } else if ( strcmp( *tokenChar, "const" ) == 0 ) {
       return 28;
   } else if ( strcmp( *tokenChar, "var" ) == 0 ) {
       return 29;
   } else if ( strcmp( *tokenChar, "proc" ) == 0 ) {
       return 30;
   } else if ( strcmp( *tokenChar, "write" ) == 0 ) {
       return 31;
   } else if ( strcmp( *tokenChar, "read" ) == 0 ) {
       return 32;
   } else if ( strcmp( *tokenChar, "else" ) == 0 ) {
       return 33;
   }*/
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
        return false;
    }
}

/**
 *
 */
void manage_file_pointers ( char action[] ) {
    if ( strcmp( "open", action ) == 0 ) {
        input = fopen( "input.txt", 'r' );
        cleanoutput = fopen( "cleanoutput.txt", "w" );
        lexemetable = fopen( "lememetable.txt", "w" );
        lexemelist - fopen( "lexmelist.txt", "w");
    } else if ( strcmp( "close", action ) == 0 ) {
        fclose(input);
        fclose(cleanoutput);
        fclose(lexemelist);
        fclose(lexemetable);
    }
}

void main () {
    /*while ( get_input() != false ) {

    }*/

    printf( "Number: %d \n", get_token_id_from_string( "," ) );
    printf( "Number: %d \n", get_token_id_from_string( ";" ) );
    printf( "Number: %d \n", get_token_id_from_string( "0" ) );
    printf( "Number: %d \n", get_token_id_from_string( "9" ) );
    printf( "Number: %d \n", get_token_id_from_string( "5" ) );
    printf( "Number: %d \n", get_token_id_from_string( "if" ) );
    printf( "Number: %d \n", get_token_id_from_string( "begins" ) );
    printf( "Number: %d \n", get_token_id_from_string( "end" ) );
    printf( "Number: %d \n", get_token_id_from_string( "." ) );
    printf( "Number: %d \n", get_token_id_from_string( "<" ) );
    printf( "Number: %d \n", get_token_id_from_string( "<=" ) );
    printf( "Number: %d \n", get_token_id_from_string( ">" ) );
    printf( "Number: %d \n", get_token_id_from_string( ">=" ) );
    //printf( "Number: %d \n", get_token_id_from_string( "" ) );

}