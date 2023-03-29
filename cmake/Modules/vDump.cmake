#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Copyright (c) 2020-2021 Enrico Sorichetti
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file BOOST_LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_guard( GLOBAL )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
function( vdump _arglist _argline )

if( VDUMP )
  if( ${_argline} MATCHES   "^[0-9]+$" )
    set( _line "0000${_argline}" )
    string( LENGTH "${_line}"  _len )
    math( EXPR _indx "${_len} - 4" )
    string(SUBSTRING ${_line} ${_indx} -1 _line )
  else()
    string( REGEX REPLACE "[^a-zA-Z0-9_]" "_" _line  "${_argline}" )
  endif()
  get_filename_component( _list "${_arglist}" NAME_WE)
  if( "${_line}" STREQUAL "" )
    string( REGEX REPLACE "[^a-zA-Z0-9_]" "_"
      _out "vars_for_${_list}" )
  else()
    string( REGEX REPLACE "[^a-zA-Z0-9_]" "_"
      _out "vars_for_${_list}_at_${_line}" )
  endif()
  set( _out "${CMAKE_BINARY_DIR}/${_out}.txt" )
  file( REMOVE ${_out} )
  set( _buf "" )
  get_cmake_property( _vars VARIABLES )
  list( SORT _vars )
  list( REMOVE_DUPLICATES _vars )
  set( heads "ARG" "CMAKE_Fortran" "BISON_version_" )
  set( tails "_CONTENT" "_COMPILER_ID_TOOL_MATCH_REGEX"
    _SRCS _DIRS _LIBS _DIAG )
  set( anywh "_OBJC" )

  foreach( _var IN LISTS _vars )

    # variables starting with "_" short SCOPED or internal
    if( "${_var}" MATCHES "^_" )
      continue()
    endif()

    string( TOLOWER "${_var}" _lc )
    if( "${_var}" STREQUAL "${_lc}" )
      continue()
    endif()

    # heads
    unset( _skip )
    unset( _skip  CACHE )
    foreach( head IN LISTS heads )
      if( "${_var}" MATCHES "^(${head})" )
        set( _skip "1" )
        break()
      endif()
    endforeach()
    if( _skip )
      continue()
    endif()

    # tails
    unset( _skip )
    unset( _skip  CACHE )
    foreach( tail IN LISTS tails )
      if( "${_var}" MATCHES "(${tail})$" )
        set( _skip "1" )
        break()
      endif()
    endforeach()
    if( _skip )
      continue()
    endif()

    # anywh
    unset( _skip )
    unset( _skip  CACHE )
    foreach( anyw IN LISTS anywh )
      if( "${_var}" MATCHES "(${anyw})" )
        set( _skip "1" )
        break()
      endif()
    endforeach()
    if( _skip )
      continue()
    endif()

    if( "${_var}" STREQUAL "OUTPUT" )
      continue()
    endif()
    string( APPEND _buf "[[ ${_var}='${${_var}}'\n" )
  endforeach()

  file( WRITE "${_out}" "${_buf}" )

endif( VDUMP )

endfunction()
