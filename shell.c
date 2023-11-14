#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "StringVector.h"

void
shell_run( struct Shell *s )
{
    s->running = true;
    while ( s->running ) {
        shell_read_line( s );
        shell_execute_line( s );
    }
}

void
shell_init( struct Shell *s )
{
    s->running     = false;
    s->line_number = 0;
    s->buffer      = NULL;
    s->buffer_size = 0;
}

void
shell_free( struct Shell *s )
{
    free( s->buffer );
}

void
shell_read_line( struct Shell *s )
{
    char *pwd = getcwd( NULL, 0 );
    printf( "\033[0;32m" );
    printf( "Math-X [%d] - %s $>", ++s->line_number, pwd );
    ssize_t bytes_read = getline( &s->buffer, &s->buffer_size, stdin );
    if ( bytes_read < 0 ) {
        s->running = false;
    }
    free( pwd );
}

void
shell_execute_line( struct Shell *s )
{
    struct StringVector tokens = split_line( s->buffer );

    char *command = string_vector_get( &tokens, 0 );
    if ( strcmp( command, "exit" ) == 0 ) {
        printf( "\033[0;36m" );
        printf( "Goodby ! \n" );
        s->running = false;
    }
    else if ( strcmp( command, "!" ) == 0 ) {
        if ( string_vector_size( &tokens ) == 1 ) {
            char *current_shell = getenv( "SHELL" );
            assert( current_shell != NULL );
            system( current_shell );
        }
        else {
            // char *tmp = malloc( 256 * sizeof( char ) );
            // tmp       = strjoinarray( tmp, &tokens, 1, string_vector_size( &tokens ), " " );
            char **arg;
            arg = malloc( sizeof( char * ) * string_vector_size( &tokens ) );
            size_t i;

            for ( i = 1; i < string_vector_size( &tokens ); i++ ) {
                // strcpy( arg[i - 1], string_vector_get( &tokens, i ) );
                arg[i - 1] = strdup( string_vector_get( &tokens, i ) );
            }
            arg[i - 1] = NULL;

            pid_t pid = fork();
            if ( pid == 0 ) {
                execvp( arg[0], arg );
                perror( "execvp" );
                exit( EXIT_FAILURE );
            }
            else {
                int status;
                waitpid( pid, &status, 0 );
            }
            for( i = 0; i < string_vector_size( &tokens ); i++ ) {
                free( arg[i] );
            }
            free( arg );
        }
    }
    else if ( strcmp( command, "tmp" ) == 0 ) {
        char *tmp = malloc( 256 * sizeof( char ) );
        tmp       = strjoinarray( tmp, &tokens, 1, string_vector_size( &tokens ), " " );
        pid_t pid = fork();
        if ( pid == 0 ) {
            char *arg[] = { "ls", "-l", "/tmp", NULL };
            execvp( "ls", arg );
            perror( "execvp" );
            exit( EXIT_FAILURE );
        }
        else {
            int status;
            waitpid( pid, &status, 0 );
        }
        free( tmp );
    }
    else if ( strcmp( command, "pokemon" ) == 0 ) {
        if ( string_vector_size( &tokens ) == 1 ) {
            system(
                "curl -s -L "
                "'https://raw.githubusercontent.com/shinya/pokemon-terminal-art/main/hello.sh' | "
                "bash" );
        }
    }
    else if ( strcmp( command, "matrix" ) == 0 ) {
        system( "~/notroot/usr/bin/cmatrix -r" );
    }
    else if ( strcmp( command, "cd" ) == 0 ) {
        char *path = NULL;
        if ( string_vector_size( &tokens ) > 1 ) {
            path = string_vector_get( &tokens, 1 );
        }
        else {
            path = getenv( "HOME" );
            assert( path != NULL );
        }
        int ret = chdir( path );
        if ( ret != 0 ) {
            printf( "cd: %s: No such file or directory\n", path );
        }
    }
    else if ( strcmp( command, "" ) == 0 ) {
        printf( "\n" );
    }
    else {
        printf( "Unknown command: %s\n", command );
    }
    string_vector_free( &tokens );
}
