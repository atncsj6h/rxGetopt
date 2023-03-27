#   README
*   a rexx external function that provides the same services provided by
*   the extended getopt command available in the util-linux package
*   ( descendent of the getopt package developed by Frodo Looijaard )
*
*   can do anything that the GNU getopt(3) routines can do.
*   can parse long parameters.
*   can shuffle parameters,
*   so you can mix options and other parameters on the command-line.
*   all this in the ooRexx environment
* * *

##  COPYRIGHT
*   Copyright (c) 2023, 2023 Enrico Sorichetti
*   Distributed under the Boost Software License, Version 1.0
*   See accompanying file `LICENSE_1_0.txt` or copy at
*   `[http://www.boost.org/LICENSE_1_0.txt](http://www.boost.org/LICENSE_1_0.txt)`
* * *

##  ADDITIONAL COPYRIGHT NOTICE
*   the man page for the util-linux getopt is provided under the fair use assumption
* * *

##  Repository
*   `https://github.com/atncsj6h/rxgetopt.git`
*   `git@github.com:atncsj6h/rxgetopt.git`
* * *

##  prerequisites
*   ooRexx 5.0.0, it needs the .syscargs constructs
*   NOTE
*   the Makefile will have to be changed for the standard ooRexx distribution
*   because the oorexx-config is not provided
* * *

##  nice to have
*
* * *

##  How to
*   clone the repository
*   run the make/gmake command
*   NO installer for now,
*   just copy the librxgetopt.dylib to the oorexx lib directory
*
*   use
*   man ./getopt.1.gz
*   to see what the enhanced getopt does and what getopts is trying to emulate
*
*   REMEMBER ...
*   getopt is application agnostic ( does not do the application parsing )
*   it just reformats the options passed so that it makes easy the parsing
*   and ensures that the options array is formally correct
*   I.E.
*   an options with optional arguments will create if needed
*   an empty string argument
*
*   the behaviour options modify the way functions work
*   it is just a string with comma separated tokens, the string can be empty
*   the only option considered for now is "alternate",
*   which makes acceptable and recognized ( as per `getopt_long` man page )
*   long options in the format -longopt in addition to the traditional --longopt
*
*   CALL FORMAT FOR PURE GETOPT EMULATION
*
*   args = getopts(behaviour options, short options, long options, .syscargs )
*   see the getopts.rex scripts
*
*   CALL FORMAT FOR ARGPARS - ONLY LONG OPTIONS HANDLED
*
*   args = argpars( behaviour options, long options, .syscargs )
*   see the argpars.rex scripts
*
*   argpars will create and intialize a set of variables of the pattern
*
*   options with arguments ( keyword/value pairs )
*   have_longOptionName = 0/1
*   longOptionName = the value of the argument
*
*   options with no arguments ( flags )
*   longOptionName = 0/1
*
*   the returned options/args array will be filled with the positional options
*   see the sample argpars.rex script
*
*   the testing can be done in the build directory without installing
*
* * *

##  Additional considerations/warnings
*   the system `getopt_long` ( all systems, not just OSX )
*   does not deal well ( at all ) with options with optional arguments
*   so beware about using options with the "::" indicator
*   a workaround is to use the form --option=argument instead of --option argument
* * *

##  tested and working on
*   APPLE Big Sur / High Sierra
*   should/might work asis on any linux like system
* * *

##  NOTES
*   [Markdown Information](https://bitbucket.org/tutorials/markdowndemo)
* * *
