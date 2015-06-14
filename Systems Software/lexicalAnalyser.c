//
// Created by Kevin on 6/12/2015.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void error_message_exit();
char* determine_keyword ( char first_statement_char, bool change_state );
void disambiguate_keyword ( char secondChar );
char get_input();
int get_token_id_from_string ( char *tokenChar );
bool keyword_is_valid ( char keyword[], char validKeyword[], int keywordRemainderLength );
void manage_file_pointers ( char action[] );




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
            if ( change_state ) {
                state = 2;
                disambiguate_keyword(get_input());
            }
            if(state =21) return "call";
            else if(state = 22) return "const";
            else error_message_exit();
                                                          //should have an error function
        case 'd':
            // do keyword
            if ( change_state ) { state = 3; }
            return "do";
        case 'e':
            //end or else
            if ( change_state ) {
                state = 4;
                 disambiguate_keyword(get_input());
            }
            if(state =31) return "end";
            else if(state = 32) return "else";
            else error_message_exit();
                                                        //should have error function "Problem with incorrect input"
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
            if ( change_state ) {
                state = 10;
                disambiguate_keyword(get_input());
            }
            if(state =41) return "while";
            else if(state = 42) return "write";
            else error_message_exit();                                     //error message


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
void disambiguate_keyword ( char secondChar ) {
    switch ( state ) {
        case 2:
            // call/const
            if ( secondChar == 'a' ) { state = 21; } //call
            else if ( secondChar == 'o') { state = 22; } //const
            else { state = -1; }
            return;
        case 4:
            // end/else
            if ( secondChar == 'n' ) { state = 31; } //end
            else if ( secondChar == 'l' ) { state = 32; } // else
            else { state = -1; }
            return;
        case 10:
            // while/write
            if ( secondChar == 'h' ) { state = 41; } //while
            else if ( secondChar == 'r' ) { state = 42; } //write
            else { state = -1; }
            return;
        default:
            return;
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
    int i;
    for ( i = 0; i < keywordRemainderLength; ++i ) {
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
   } else if ( strcmp( tokenChar, "" ) == 0 ) {
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
 *
 */
void manage_file_pointers ( char action[] ) {
    if ( strcmp( "open", action ) == 0 ) {
        input = fopen( "input.txt", "r" );
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

/**
 *  A function to call if the input file contains inappropriate format
 */
void error_message_exit(){
    printf("There was an error reading in the input");
    exit(1);
}





void run_program(){
    manage_file_pointers("open");

    char* keyword = NULL;
    keyword = determine_keyword(get_input(), true);         //if this returns the correct keyword how will we validate it?


    fprintf(cleanoutput, keyword);
    fputs(keyword, lexemetable);
    fputs(keyword, lexemelist);




}




void main () {
  //  manage_file_pointers("open");


//    printf( "Keyword: %c \n",  );
//    printf( "Number: %d \n", get_token_id_from_string( ";" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "0" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "9" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "5" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "if" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "begins" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "end" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "." ) );
//    printf( "Number: %d \n", get_token_id_from_string( "<" ) );
//    printf( "Number: %d \n", get_token_id_from_string( "<=" ) );
//    printf( "Number: %d \n", get_token_id_from_string( ">" ) );
//    printf( "Number: %d \n", get_token_id_from_string( ">=" ) );
   // printf( "Number: %d \n", get_token_id_from_string( "" ) );

}
