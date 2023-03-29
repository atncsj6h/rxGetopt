#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Copyright (c) 2020-2021 Enrico Sorichetti
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file BOOST_LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_guard( GLOBAL )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
function( vsnap )

if( VSNAP )
  set( _args "${ARGV}" )
  list( SORT _args )
  list( REMOVE_DUPLICATES _args )
  set( _selvars "" )
  get_cmake_property( _allvars VARIABLES )
  foreach( _argv IN LISTS _args )
    string( REGEX MATCH "([^\\*]*)" _head "${_argv}" )
    if( _head STREQUAL _argv )
      list( APPEND _selvars "${_argv}" )
      continue()
    endif()
    string (REGEX MATCHALL "(^|;)${_head}[A-Za-z0-9_]*" _vars "${_allvars}")
    foreach( _var IN LISTS _vars )
      if( "${_var}" STREQUAL "" )
        continue()
      endif()
      if( "${_var}" MATCHES "^(CMAKE_Fortran)" )
        continue()
      endif()
      if( "${_var}" MATCHES "(_CONTENT)$" )
        continue()
      endif()
      if( "${_var}" MATCHES "(_COMPILER_ID_TOOL_MATCH_REGEX)$" )
        continue()
      endif()
      list( APPEND _selvars "${_var}" )
    endforeach()
  endforeach()
  list( SORT _selvars )
  list( REMOVE_DUPLICATES _selvars )
  foreach( _var IN LISTS _selvars )
    message( "[[ ${_var} '${${_var}}' " )
  endforeach()
  return()
else( VSNAP )
  return()
endif( VSNAP )

endfunction()
