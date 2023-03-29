#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Copyright (c) 2020-2021 Enrico Sorichetti
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file BOOST_LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_guard( GLOBAL )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
function( vscan )
  set( ignore
    HAVE_CMAKE_SIZEOF_UNSIGNED_SHORT
    HAVE_FLAG_SEARCH_PATHS_FIRST
  )
  set( args "${ARGV}" )
  list( GET args "0" retv )
  list( REMOVE_AT args "0" )
  list( SORT args )
  list( REMOVE_DUPLICATES args )
  set( retvars "" )
  get_cmake_property( allvars VARIABLES )
  foreach( argv IN LISTS args )
    string( REGEX MATCH "([^\\*]*)" head "${argv}" )
    if( head STREQUAL argv )
      list( APPEND retvars "${argv}" )
      continue()
    endif()
    string (REGEX MATCHALL "(^|;)${head}[A-Za-z0-9_]*" vars "${allvars}")
    foreach( name IN LISTS vars )
      if( name STREQUAL "" OR
        name IN_LIST ignore )
        continue()
      endif()
    list( APPEND retvars "${name}" )
    endforeach()
  endforeach()
  list( SORT retvars )
  list( REMOVE_DUPLICATES retvars )
  set( "${retv}" "${retvars}" PARENT_SCOPE )
  return()

endfunction()
