
/* argument parsing ------------------------------------------------------------------------------------------------- */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "args.hpp"


using bamfile::READ;
using bamfile::bamfile_t;


// some crazy shit for stringifying preprocessor directives
#define STRIFY(x) #x
#define TO_STR(x) STRIFY(x)

const char usage[] =
    "usage: " EXEC " [-h] [-c CUTOFF] -B BAM_IN\n";

const char help_msg[] =
    "filter sequencing data using some simple heuristics\n"
    "\n"
    "required arguments:\n"
    "  -B BAM_IN                BAM input file\n"
    "\n"
    "optional arguments:\n"
    "  -h, --help               show this help message and exit\n"
    "  -c CUTOFF                mutations with p < CUTOFF will be assumed real (default="
                                TO_STR( DEFAULT_CUTOFF ) ")\n";

inline
void help()
{
    fprintf( stderr, "%s\n%s", usage, help_msg );
    exit( 1 );
}

#define ERROR( msg, args... ) \
{ \
    fprintf( stderr, "%s" EXEC ": error: " msg "\n", usage , ##args ); \
    exit( 1 ); \
}

// args_t ----------------------------------------------------------------------------------------------------------- //

args_t::args_t( int argc, const char * argv[] ) :
    bamin( NULL ),
    cutoff( DEFAULT_CUTOFF )
{
    int i;

    // skip arg[0], it's just the program name
    for ( i = 1; i < argc; ++i ) {
        const char * arg = argv[i];

        if ( arg[0] == '-' && arg[1] == '-' ) {
            if ( !strcmp( &arg[2], "help" ) ) help();
#if 0
            else if ( !strcmp( &arg[2], "fasta" ) ) parse_fasta( argv[++i] );
            else if ( !strcmp( &arg[2], "fastq" ) ) parse_fastq( argv[++i] );
            else if ( !strcmp( &arg[2], "output" ) ) parse_output( argv[++i] );
            else if ( !strcmp( &arg[2], "length" ) ) parse_length( argv[++i] );
#endif
            else
                ERROR( "unknown argument: %s", arg );
        }
        else if ( arg[0] == '-' ) {
            if ( !strcmp( &arg[1], "h" ) ) help();
            else if ( !strcmp( &arg[1], "B" ) ) parse_bamfile( argv[ ++i ] );
            else if ( !strcmp( &arg[1], "c" ) ) parse_cutoff( argv[ ++i ] );
            else
                ERROR( "unknown argument: %s", arg );
        }
        else
            ERROR( "unknown argument: %s", arg );
    }

    if ( !bamin )
        ERROR( "missing required argument -B BAM_IN" );
}

args_t::~args_t()
{
    delete bamin;
}

void args_t::parse_bamfile( const char * input )
{
    bamin = new bamfile_t( input, READ );
}

void args_t::parse_cutoff( const char * str )
{
    cutoff = atof( str );

    if ( cutoff <= 0.0 || cutoff >= 1.0 )
        ERROR( "cutoff must be a real number between 0.0 and 1.0, exclusive" );
}
