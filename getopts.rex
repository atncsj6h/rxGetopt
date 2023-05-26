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
optsShort = "ab:cd:"
--  for the long options the casing is RELEVANT
--  will try provide later a caseless options scan
optsLong  = "mach:,rot1:,rot2:,rot3:,refl:,plgs:,flag,optval::"

args = getopts("alternate", optsShort, optsLong, .syscargs)

say "args - normalized "
say "("right(args~dimension(1),2)") number of entries"
say

do i = 1 to args~dimension(1)
  say "("right(i,2)") >>"args[i]"<<"
end
say

do args~dimension(1)
  select

  when args[1] = "--" then do
    args~delete(1);
    leave
  end

  when args[1] = "-a" then do
    flga = 1
    args~delete(1);
    iterate
  end
  when args[1] = "-b" then do
    optb = args[2]
    args~delete(1);args~delete(1);
    iterate
  end

  when args[1] = "--rot1" then do
    rot1 = args[2]
    args~delete(1);args~delete(1);
    iterate
  end
    when args[1] = "--rot2" then do
    rot2 = args[2]
    args~delete(1);args~delete(1);
    iterate
  end
    when args[1] = "--optval" then do
    optval = args[2]
    args~delete(1);args~delete(1);
    iterate
  end

  otherwise do
    say "option provided, but not handled >>"args[1]"<<"
    args~delete(1)
    iterate
    signal logic_error
  end

  end

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
