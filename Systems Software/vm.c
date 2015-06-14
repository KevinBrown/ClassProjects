// Kevin Brown
// Systems Software

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// CPU Registers
int sp = 0;
int bp = 1;
int pc = 0;
int ir = 0;

#define MAX_STACK_SIZE 2000
#define MAX_CODE_LEVELS 500
#define MAX_LEXI_LEVELS 3

int stack[MAX_STACK_SIZE];

/**
 * Base of stack frame l levels down from the top
 * From slides
 */
int base ( int level, int b ) {
    while ( level > 0 ) {
        b = stack[ b + 2 ];
        --level;
    }
    return b;
}

/**
 * LIT Instruciton
 */
void push_to_stack( int modifier ){
    ++sp;
    stack[sp] = modifier;
    return;
}

/**
 * OPR Instruction
 * @returns result of operation
 */
int perform_arithmatic_op( int modifier) {
    switch ( modifier ) {
        case 0:
            //return
            sp = bp - 1;
            pc = stack[ sp + 4 ];
            bp = stack[ sp + 3 ];
            return sp;
        case 1:
            //negative (NEG)
            stack[sp] = -stack[sp];
            return stack[sp];
        case 2:
            //ADD - addition
            --sp;
            stack[sp] = stack[sp] +  stack[ sp + 1 ];
            return stack[sp];
        case 3:
            //SUB - subtract
            --sp;
            stack[sp] = stack[sp] - stack[ sp + 1 ];
            return stack[sp];
        case 4:
            //MUL - multiply
            --sp;
            stack[sp] = stack[sp] * stack[ sp + 1 ];
            return stack[sp];
        case 5:
            //DIV - divide
            --sp;
            stack[sp] = stack[sp] / stack[ sp + 1 ];
            return stack[sp];
        case 6:
            //ODD -- is odd (true/false)
            stack[sp] = stack[sp] % 2;
            return stack[sp];
        case 7:
            //MOD - modulus
            --sp;
            stack[sp] = stack[sp] % stack[ sp + 1 ];
            return stack[sp];
        case 8:
            //EQL -- is equal (true/false)
            --sp;
            stack[sp] = stack[sp] == stack[sp+1];
            return stack[sp];
        case 9:
            //NEQ -- is not equal (true/false)
            --sp;
            stack[sp] = stack[sp] != stack[sp+1];
            break;
        case 10:
            //LSS - less than
            --sp;
            stack[sp] = stack[sp] < stack[sp+1];
            return stack[sp];
        case 11://LEQ - less than or equal to
            --sp;
            stack[sp] = stack[sp] <= stack[sp+1];
            return stack[sp];
        case 12:
            //GTR - greater than
            --sp;
            stack[sp] = stack[sp] > stack[sp+1];
            return stack[sp];
        case 13:
            //GEQ - greater than or equal to
            --sp;
            stack[sp] = stack[sp] >= stack[sp+1];
            return stack[sp];
    }
}

/**
 * LOD Instruction
 */
void push_value_to_stack_top( int lexLevel, int modifier ) {
    ++sp;
    stack[sp] = stack[ base( lexLevel, bp ) + modifier ];
    return;
}

/**
 * STO Instruction
 */
void push_stack_top_down_nLevels(int lexLevel, int numLevels ) {
    stack[ base( lexLevel, bp ) + numLevels] = stack[sp];
    --sp;
    return;
}

/**
 * CAL Instruction
 */
void call_procedure_at_location( int lexLevel, int location ) {
    stack[ sp + 1 ] = 0;
    stack[ sp + 2 ] = base( lexLevel, bp );
    stack[ sp + 3 ] = bp;
    stack[ sp + 4 ] = pc;
    bp = sp + 1;
    pc = location;
    return;
}

/**
 * INC
 */
void allocate_local_stack_vars( int modifier) {
    sp += modifier;
    return;
}

/*
 * JMP
 */
void jump( int location ) {
    pc = location;
    return;
}

/*
 * JPC
 */
void jump_if_equal( int location ) {
    if ( stack[sp] == 0 ) {
        pc = location;
    }
    --pc;
    return;
}

/**
 * SIO Mode 0
 */
void pop_stack () {
    printf( "%d", stack[sp] );
    --sp;
    return;
}

/**
 * SIO Mode 1
 */
void push_stack () {
    ++sp;
    int pushval;
    fscanf( stdin, "%d", &pushval );
    stack[sp] = pushval;
    return;
}

/**
 * SIO Mode 2
 */
void halt() {
    printf( "\n Stopping Execution... \n" );
    exit(0);
}

/**
 * Prints out the header for the output file
 * @param type must me either assembly or exec trace
 */
void print_header( FILE* fp, char type[] ) {
    if ( type == "assembly" ) {
        printf( "Line\tOP\tL\tM");
    } else if ( type == "exectrace") {
        printf( "\t\t\t\t\tPC\tBP\tSP\t Stack");
        printf( "Initial Values\t\t0\t1\t0");
    }
}

/**
 * Prints a line for the assembly decode section of the output file
 */
void print_assembly_instruction_line( FILE* fp, int op, int l, int m ) {
    fprintf( fp, "%d %d %d", op, l, m );
}

/**
 * Decode a Given Instruction
 */
char* decode_instruction( char opcode, int level, int modifier ) {
    switch ( opcode ) {
        case 1:
            //Push Value to Stack (LIT)
            push_to_stack( modifier );
            return "LIT";
        case 2:
            //Artmatic Operation (OPR)
            return "OPR";
        case 3:
            //Get Value at Offset Frame (LOD)
            return "LOD";
        case 4:
            //pop stack and insert value (STO)
            return "STO";
        case 5:
            //call procedure at M
            return "CAL";
        case 6:
            //allocated M levels on stack
            return "INC";
        case 7:
            //jump
            return "JMP";
        case 8:
            //jump if equal to 0 plus pop
            return "JPC";
        case 9:
            // SIO Instruction
            switch ( modifier ) {
                case 1:
                    //pop stack and print
                    break;
                case 2:
                    //read input and push
                    break;
                case 3:
                    //halt machine
                    halt();
                default:
                    printf( "Invalid modifier #d", modifier);
            }
            return "SIO";
        default:
            printf("Encoutered Unexpected Opcode %s", &opcode );
            exit(-1);
    }
}

/**
 * Loads all assembly instructions into memory
 */
void load_assembly_instructions( FILE* fp, int* opcode, int* lexlevel, int* modifier) {
    int linenum = 0;
    int oc, l, m;

    while (fscanf(fp, "%d %d %d", &oc, &l, &m)) {
        opcode[linenum] = oc;
        lexlevel[linenum] = l;
        modifier[linenum] = m;
        ++linenum;
    }
}

FILE* openFile( char path[], char filename[] ) {
    char* filePath;
    if ( ( filePath = malloc( strlen( path ) + strlen( filename ) + 1 ) ) == NULL ) {
       // FIX THIS: printf( &stderr, "Failed to allocate new memory")
    }


}

int main () {
    FILE* input = fopen( "./mcode.txt", "r");
    FILE* output = fopen( "./stacktrace.txt", "w" );
    FILE* assemblyout = fopen( "./assembly.txt", "w" );
    FILE* executiontrace = fopen( "executiontrace.txt", "w");

    int* opcode = malloc( sizeof( int ) * 30000 );
    int* lexlevel = malloc( sizeof(int) * 30000 );
    int* modifier = malloc( sizeof(int) * 30000 );

    int linenum = 0;

    int test1, test2, test3;

    fscanf( input, "%d %d %d", &test1, &test2, &test3);

    //load_assembly_instructions( input, opcode, lexlevel, modifier );

    char* insName = decode_instruction( 3, 0, 0 );
    printf( "%s", insName );

    free( opcode );
    free( lexlevel );
    free( modifier );

    fclose( input );
    fclose( output );
    return 0;
}