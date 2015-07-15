//Chad Armstrong


#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <stdbool.h>

bool checkComment(char c);
void printClean(char c);
void manage_file_pointers ( char action[] );




typedef enum {
 nulsym = 1, identsym =2 , numbersym = 3, plussym= 4, minussym = 5,
 multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
 gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
 periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
 whilesym =25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym =31,
 readsym =32 , elsesym =33
} token_type;


int state=0;

FILE * input, * cleanoutput, * lexemetable, * lexemelist;


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

char get_input () {
    char inputchar;

    if ( ( inputchar = fgetc( input ) ) != EOF ) {
        return inputchar;
    } else {
        return '0';
    }
}


void printClean(char c){
    if(checkComment(c))
        fprintf(cleanoutput, "%c", c);
    else
        return;

}

bool checkComment(char c){
    char d;
    if(c == "/")
    {
        if((d = get_input()) == ("/" || "*") )
        {
           return false;
        }
        else
        {
            fprintf(cleanoutput, "%c", d);
            return true;
        }
    }
    return true;
}


void printLexTab(char c){
    if((c != ' ') && (c != '\n') && (c != '\t')){
        //determine if a keyword, if it is determine token value as well
        //otherwise print it
        while((c >= 65 && c <=90) || (c >= 97 && c <= 122)){
            fprintf(lexemetable, "%c\t\t%d\n", c, nulsym);



        }







    }




   // fprintf(lexemetable, "%c\t\t%d\n", c, nulsym);
}

void printXTimes(){
    //while its letters keep printing until keyword is through
 //   while(())
        fprintf(lexemetable, "%c", get_input());
}





int main(){

    manage_file_pointers("open");


    //prints out clean output
    char c;
    while((c = get_input()) != '0')
        printClean(c);



    manage_file_pointers("close");
    manage_file_pointers("open");       //resets files
    //prints out lexemetable

    fprintf(lexemetable, "lexeme\t\tTokenType\n"); //header for table
    while((c = get_input()) != '0')
        printLexTab(c);












    return 0;
}
