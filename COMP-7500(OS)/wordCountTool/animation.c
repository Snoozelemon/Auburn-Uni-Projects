#include<stdio.h>
#include<stdlib.h>

#include<time.h>
#include<unistd.h>

int print_progress(int time){
    char spinner[] = "|/-\\";
    int i, j;
    printf( "Loading...." );
    for ( i = 0 ; i < 10 ; i++ ) {
        for ( j = 0 ; j < 4 ; j++ ) {
            printf( "%c\b", spinner[j] );
            fflush( stdout );
            usleep( time * 10000 );
        }
    }
    printf("\n");
}