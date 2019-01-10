# Install script for directory: /home/john/myprefix/src/gr-ieee-80211/include/ieee802-11

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/john/myprefix")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ieee802_11_devel")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ieee802-11/gnuradio/ieee802_11" TYPE FILE FILES
    "/home/john/myprefix/src/gr-ieee-80211/lib/include/ieee802-11/moving_average_ff.h"
    "/home/john/myprefix/src/gr-ieee-80211/lib/include/ieee802-11/moving_average_cc.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/api.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/chunks_to_symbols.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/constellations.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/decode_mac.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/decode_mac_soft.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/ether_encap.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/frame_equalizer.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/mac.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/mapper.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/parse_mac.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/signal_field.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/sync_long.h"
    "/home/john/myprefix/src/gr-ieee-80211/include/ieee802-11/sync_short.h"
    )
endif()

