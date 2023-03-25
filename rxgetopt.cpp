/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Copyright Enrico Sorichetti 2023 - 2023
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file BOOST_LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "oorexxapi.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "xstringops.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  static objects
*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  LONG_OPT is returned when a long option is found.
*/
#define LONG_OPT 0
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  NON_OPT is returned when a non-option is found in '+' mode
  to be investigated
*/
#define NON_OPT 1


/* Allow changing which getopt is in use with function pointer */
int (*getopt_long_fp) (int , char * const * , const char * ,
	const struct option * , int * );

static struct option *long_options = NULL ;
static int long_options_size = 0; 	/* size of array */
static int long_options_used = 0;		/* used elements in array */

static int  getopt_alternate = 0;
static char alternate[] = "alternate" ;
static int  getopt_caseless = 0;
static char caseless[]  = "caseless" ;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
static void getopt_options( const char * options ) {
  char * tmp = strdup(options) ;
  char * ptr;
  ptr = strstr( tmp, alternate ) ;
  if ( ptr )
    getopt_alternate = 1;
  ptr = strstr( tmp, caseless ) ;
  if ( ptr )
    getopt_caseless = 1;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
static void *xmalloc(const size_t size) {
  void *ret = malloc(size);
  if (!ret && size)
    fprintf(stderr, "%s: error: cannot allocate %zu bytes", "rxgetopt", size);
  return ret ;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
static void *xrealloc(void *ptr, const size_t size)
{
  void *ret = realloc(ptr, size);
  if (!ret && size)
    fprintf(stderr, "%s: error: cannot allocate %zu bytes", "rxgetopt", size);
  return ret;
}

#define LONG_OPTIONS_SPARES 16
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  initialize long options.
*/
static void init_longOptions()
{
	free(long_options);
	long_options = NULL;
	long_options_size = LONG_OPTIONS_SPARES ;
	long_options_used = 0;

	long_options = (struct option *)xrealloc(long_options,
	  sizeof(struct option) * long_options_size ) ;

	long_options[long_options_used].name = NULL;
	long_options[long_options_used].has_arg = 0;
	long_options[long_options_used].flag = NULL;
	long_options[long_options_used].val = 0;

  long_options_used++;

}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Add a long option.
*/
static void add_longOption( const char *name, int has_arg)
{
	if (long_options_size == long_options_used) {
		long_options_size += LONG_OPTIONS_SPARES;
		long_options = (struct option *)xrealloc(long_options,
			sizeof(struct option) * long_options_size);
	}

	char * tmp = strdup(name);
	long_options[long_options_used - 1].name = tmp;
	long_options[long_options_used - 1].has_arg = has_arg;
	//  long_options[long_options_used - 1].flag = &flag ;
	long_options[long_options_used - 1].flag = &long_options[long_options_used-1].val;
	long_options[long_options_used - 1].val = long_options_used;

  // the end marker
	long_options[long_options_used].name = NULL;
	long_options[long_options_used].has_arg = 0;
	long_options[long_options_used].flag = NULL ;
	long_options[long_options_used].val = 0;

	long_options_used++;
}

static void add_longOptions(char *options)
{
	int has_arg;
	RexxObjectPtr obj ;

	char *ptr = strtok(options, ",");
	while (ptr) {
		has_arg = no_argument;
		if (strlen(ptr) > 0) {
			if ( ptr[strlen(ptr) - 1] == ':' &&
				   ptr[strlen(ptr) - 2] == ':' ) {
			  ptr[strlen(ptr) - 2] = '\0';
				has_arg = optional_argument;
				}
			else
			if ( ptr[strlen(ptr) - 1] == ':' ) {
				ptr[strlen(ptr) - 1] = '\0';
				has_arg = required_argument;
			}
			else {
        //  context->InvalidRoutine();
        //return context->NullString() ;
			}
			add_longOption(ptr, has_arg);
		}
		ptr = strtok(NULL, ",");
	}
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine4( RexxObjectPtr, rxgetopts ,
  OPTIONAL_CSTRING, options,
  CSTRING, optsShort, CSTRING, optsLong, RexxArrayObject, syscargs )
{
  RexxObjectPtr obj;

  if ( options != NULL )
    getopt_options( options ) ;

  int RC;
  int ln; char n[256];
  int lb; char b[1024];

  int syscargs_size ;

  int keywargs_size = 0;
  int have_keywargs = 0;

  RexxArrayObject retArgs;
  retArgs = context->NewArray(0) ;

  if ( getopt_alternate )
    getopt_long_fp = getopt_long_only ;
  else
    getopt_long_fp = getopt_long;

  syscargs_size = context->ArrayItems(syscargs) ;

  if ( syscargs_size == 0 ) {
    context->ArrayAppendString(retArgs, "--", 2);
    return ( retArgs ) ;
  }

  // short options

  // long options
  init_longOptions();

  char * tmp = strdup(optsLong);
  add_longOptions( tmp );

  int argc;
  argc = LONG_OPTIONS_SPARES + long_options_size + strlen(optsShort) + syscargs_size ;
  char **argv = (char**)xmalloc( argc * sizeof(char*));

  argc = 0 ;
  argv[argc] = strdup( "getopt" );
  argc +=1 ;
  for ( int i=1; i <= context->ArrayItems( syscargs ) ; i++ ) {
    obj = context->ArrayAt( syscargs, i) ;
    argv[argc] = strdup( context->ObjectToStringValue(obj) );
    argc += 1 ;
  }

  opterr=1; optind=0;

	int optc;
	int index;

	while ( 1 ) {
	  optc = getopt_long_fp( argc, argv, optsShort, long_options, &index) ;
    if ( optc == EOF )
      break;

    const char *ch ;

    if (optc == '?' || optc == ':') {
      context->InvalidRoutine();
      return context->NullString() ;
    }

		if (optc == LONG_OPT) {
			ln = sprintf(n, "--%s", long_options[index].name);
      RC = context->ArrayAppendString(retArgs, n, ln);

      switch ( long_options[index].has_arg ) {
    	  case no_argument :
    	    break ;
    	  case required_argument :
        	if ( optarg == NULL || *optarg == '-' ) {
				    printf( "rexx: option '--%s' requires an argument\n", long_options[index].name ) ;
				    context->InvalidRoutine();
            return context->NullString() ;
				  }
        case optional_argument :
			    lb = sprintf(b, "%s", optarg ? optarg : "");
          RC = context->ArrayAppendString(retArgs, b, lb);
          break ;
        default :
        	printf( "rexx: logic error for long_options[%d].has_arg value (%d) name >%s< \n",
        	  index, long_options[index].has_arg, long_options[index].name) ;
				  context->InvalidRoutine();
          return context->NullString() ;
		  }
		}
    else if (optc == NON_OPT) {
		  printf( "rexx: getopt_long returned optc=(%d), NON_OPT\n", optc);
		  printf( "rexx: getopt_long returned optarg >%s<\n", optarg ? optarg : "");
		  context->InvalidRoutine();
      return context->NullString() ;
		  //  lb = sprintf(b, "%s", optarg ? optarg : "");
		  //  RC = context->ArrayAppendString(retArgs, b, lb);
		}
    else {
      ln = sprintf( n, "-%c", optc );
      RC = context->ArrayAppendString(retArgs, n, ln);
      ch = strchr( optsShort, optc);
      if ( ch != NULL && *(ch+1) == ':') {
        if ( optarg == NULL || *optarg == '-' ) {
				  printf( "rexx: option '-%c' requires an argument\n", *ch ) ;
				  context->InvalidRoutine();
          return context->NullString() ;
			  }
				lb = sprintf(b, "%s", optarg ? optarg : "") ;
        RC = context->ArrayAppendString(retArgs, b, lb);
      }
    }
  }

  context->ArrayAppendString(retArgs, "--", 2);

	while (optind < argc) {
		lb = sprintf(b, "%s", argv[optind++] );
		RC = context->ArrayAppendString(retArgs, b, lb);
  }

  return ( retArgs ) ;

}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine3( RexxObjectPtr, rxargpars ,
  OPTIONAL_CSTRING, options,
  CSTRING, optsLong, RexxArrayObject, syscargs )
{
  RexxObjectPtr obj;

  if ( options != NULL )
    getopt_options( options ) ;

  int RC;
  int ln; char n[256];
  int lb; char b[1024];

  int syscargs_size ;

  int keywargs_size = 0;
  int have_keywargs = 0;

  RexxArrayObject retArgs;
  retArgs = context->NewArray(0) ;

  if ( getopt_alternate )
    getopt_long_fp = getopt_long_only ;
  else
    getopt_long_fp = getopt_long;

  syscargs_size = context->ArrayItems(syscargs) ;

  // short options ( dummied )
  char optsShort[]  = { static_cast<char>(0xff) };

  // long options
  init_longOptions();

  char * tmp = strdup(optsLong);
  add_longOptions( tmp );

  for ( int i = 0; i < long_options_used-1; i++ ) {

    if (long_options[i].has_arg) {
      ln = sprintf(n, "have_%s", long_options[i].name);
      context->SetContextVariable( n, context->NewString( "0", 1) );
    }
    else {
      ln = sprintf(n, "%s", long_options[i].name);
      context->SetContextVariable( n, context->NewString( "0", 1) );
    }
  }

  lb = sprintf(b, "%d", keywargs_size);
  context->SetContextVariable( "have_keywargs", context->NewString( b, lb) );

  if ( syscargs_size == 0 ) {
    return ( retArgs ) ;
  }

  int argc;
  argc = LONG_OPTIONS_SPARES + long_options_size + strlen(optsShort) + syscargs_size ;
  char **argv = (char**)xmalloc( argc * sizeof(char*));

  argc = 0 ;
  argv[argc] = strdup( "getopt" );
  argc +=1 ;
  for ( int i=1; i <= context->ArrayItems( syscargs ) ; i++ ) {
    obj = context->ArrayAt( syscargs, i) ;
    argv[argc] = strdup( context->ObjectToStringValue(obj) );
    argc += 1 ;
  }

  opterr=1; optind=0;

	int optc;
	int index;

  while ( 1 ) {
	  optc = getopt_long_fp( argc, argv, optsShort, long_options, &index) ;
    if ( optc == EOF )
      break;

    const char *ch ;

    if (optc == '?' || optc == ':') {
      context->InvalidRoutine();
      return context->NullString() ;
    }

    if (optc == LONG_OPT) {
		  switch ( long_options[index].has_arg ) {
		    case no_argument :
		      ln = sprintf(n, "%s", long_options[index].name);
          context->SetContextVariable( n, context->NewString( "1", 1) );
          have_keywargs +=1;
		      break ;
        case required_argument :
        	if ( optarg == NULL || *optarg == '-' ) {
				    printf( "rexx: option '--%s' requires an argument\n", long_options[index].name ) ;
				    context->InvalidRoutine();
            return context->NullString() ;
				  }
        case optional_argument :
          ln = sprintf(n, "%s", long_options[index].name);
				  lb = sprintf(b, "%s", optarg ? optarg : "");
				  context->SetContextVariable( n, context->NewString( b, lb) );
          ln = sprintf(n, "have_%s", long_options[index].name);
          context->SetContextVariable( n, context->NewString( "1", 1) );
          have_keywargs +=1;
          break ;
        default :
        	printf( "rexx: logic error for long_options[%d].has_arg value (%d) name >%s< \n",
        	  index, long_options[index].has_arg, long_options[index].name) ;
				  context->InvalidRoutine();
          return context->NullString() ;
		  }
    }
    else if (optc == NON_OPT) {
		  printf( "rexx: getopt_long returned optc=(%d), NON_OPT\n", optc);
		  printf( "rexx: getopt_long returned optarg >%s<\n", optarg ? optarg : "");
		  context->InvalidRoutine();
      return context->NullString() ;
		}
		else {
		  printf( "rexx: getopt_long returned (%d), >%c<\n", optc, optc);
		  printf( "rexx: getopt_long returned optarg >%s<\n", optarg ? optarg : "");
		  context->InvalidRoutine();
      return context->NullString() ;
    }
  }

  lb = sprintf(b, "%d", have_keywargs);
  context->SetContextVariable( "have_keywargs", context->NewString( b, lb) );

  // context->ArrayAppendString(retArgs, "--", 2);

	while (optind < argc) {
		lb = sprintf(b, "%s", argv[optind++] );
		RC = context->ArrayAppendString(retArgs, b, lb);
  }

  return ( retArgs ) ;

}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutineEntry rxgetopt_functions[] =
{
// long names
  REXX_TYPED_ROUTINE( rxgetopts,    rxgetopts ),
  REXX_TYPED_ROUTINE( rxargpars,    rxargpars ),


//  short names
  REXX_TYPED_ROUTINE( getopts,      rxgetopts ),
  REXX_TYPED_ROUTINE( argpars,      rxargpars ),

  REXX_LAST_ROUTINE()
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxPackageEntry rxgetopt_package_entry =
{
  STANDARD_PACKAGE_HEADER
  REXX_INTERPRETER_5_1_0,
  "rxgetopt", "1.0.0",
  NULL,                                // no load/unload functions
  NULL,
  rxgetopt_functions,
  NULL
} ;

OOREXX_GET_PACKAGE( rxgetopt ) ;
