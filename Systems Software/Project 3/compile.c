/**
 * Systems Software
 * Project 3
 * 7/14/2015
 *
 * @author Kevin Brown
 * @author Chad Armstrong
 * @license MIT License
 */

// Standard
#include <stdlib.h>
#include <stdio.h>

// Project Functions
#include "assembly.h"

// General I/O Functions
void error_message_exit ( int errorCode );

// Driver Functions
int determine_switches ( int argc, char *argv );
void run_appropiate_sections ( int flag );

// Lexme Functions


// Assembly Generation Instructions
int validate_lexme_list();

// VM Functions

// Global variables
int lexmeState = 0;
FILE  *input, *lexemelist;

// Structs


void main ( int argc, char* argv[] ) {
    run_appropiate_sections(  determine_switches( argc, argv ) );
}

/**
 * Parses command line parameters and returns an int with the appropiate flags
 *
 * Status codes:
 * 3 - l
 * 5 - a
 * 7 - v
 * 15 - l + a
 * 21 - l + v
 * 35 - a + v
 * 105 - l + a + v
 *
 * @param argc number of flags passed in from main
 * @param argv array of flags passed in from main
 * @returns the status code for the flag
 */
int determine_switches ( int argc, char *argv ) {
    int i;
    int status = 1;

    // status codes are generated by multiplying prime numbers together to get various combinations
    // to check if a certain flag is present take the modulus

    for ( i = 0; i < argc; ++i ) {
        switch ( argv[i] ) {
            case 'l':
                status *= 3;
                break;
            case 'a':
                status *= 5;
                break;
            case 'v':
                status *= 7;
                break;
            default:
                if ( argc == 0 ) {
                } else {
                    printf( "Invalid switch statements, must be either l, a, or v. Code: 1238747" );
                    exit( -1 );
                }
        }
    }

    return status;
}

/**
 * Calls appropiate section depending on flag passed from command line switch
 *
 * @param flag the flag determined by multiplying prime numbers
 */
void run_appropiate_sections ( int flag ) {
    if ( flag % 3 == 0 ) {
        print_lexmes();
    }

    if ( flag % 5 == 0 ) {
        print_assembly_code();
    }

    if ( flag % 7 == 0 ) {
        print_vm_execution();
    }
}
