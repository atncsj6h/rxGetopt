#! /usr/bin/env rexx
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- Copyright Enrico Sorichetti 2023 - 2023
-- Distributed under the Boost Software License, Version 1.0.
-- (See accompanying file BOOST_LICENSE_1_0.txt or copy at
-- http://www.boost.org/LICENSE_1_0.txt)
--

Trace "O"

signal on novalue name novalue
signal on syntax  name syntax

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--
if 0 then do
  say ">> a reminder about array indexing"
  a = .array~of('one', 'two', 'three', 'four')
  say ">> .array~of('one', 'two', 'three', 'four') "
  say ">> a~index('two') =" a~index('two')
  say
end

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--
say ".syscargs"
say "("right(.syscargs~dimension(1),2)") number of option/argument strings"
do i= 1 to .syscargs~dimension(1)
  say "("right(i,2)") >>".syscargs[i]"<<"
end
say

--  for the short options the casing is RELEVANT
--  hidden value for optsShort = "ff"x

--  for the long options the casing is RELEVANT
--  will try provide later a caseless options scan
optsLong = "mach:,rot1:,rot2:,rot3:,refl:,plgs:,flag"
args = getargs("alternate", optsLong, .syscargs)

say "the processed args"
do i = 1 to args~dimension(1)
  say i args[i]
end

say "keyword options/arguments - number of entries ("have_keywargs") "
say

tmp = optsLong
do while ( tmp \= "" )
  parse var tmp name  "," tmp

  noArgs = ( name  = strip(name,,":") )
  name  = strip(name,,":")

  if ( noArgs ) then do
    say "flg  " left(name,12) "("value(name)")"
  end
  else do
    ind = "have_"name
    say "ind  " left(ind,12) value(ind)
    ind = value( ind )
    if ind then ,
      say "opt  " left(name,12) ">"value(name)"<"
  end
  say
end

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--
say "positional options/arguments - using: do i = 1 to args~dimension(1)"
say "("right(args~dimension(1),2)") number of entries "
say

do i = 1 to args~dimension(1)
  say "("right(i,2)") >>"args[i]"<<"
end
say

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--
say "positional options/arguments - using: do argv over args"
say "do argv over args"
do argv over args
  say "("right(args~index(argv),2)") >>"argv"<<"
end
say

exit

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
logic_error:
say   "@@" || " -"~copies( 39 )
say   "@@" || " Logic error at line '"sigl"' "
say   "@@"
exit  1
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
novalue:
say   "@@" || " -"~copies( 39 )
say   "@@" || " Novalue trapped, line '"sigl"' var '"condition("D")"' "
say   "@@"
exit  1
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
syntax:
say   "@@" || " -"~copies( 39 )
say   "@@" || " Syntax  trapped, line '"sigl"' var '"condition("CODE")"' "
say   "@@"
exit  1

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
--
::requires "rxgetopt" library
