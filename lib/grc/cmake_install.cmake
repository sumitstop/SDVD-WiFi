# Install script for directory: /home/john/myprefix/src/gr-ieee-80211/grc

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_ether_encap.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_mapper.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_sync_short.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_sync_long.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_soft_frame_equalizer.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_moving_average_xx.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_parse_mac.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_decode_mac.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_frame_equalizer.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_decode_mac_soft.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_chunks_to_symbols.xml"
    "/home/john/myprefix/src/gr-ieee-80211/grc/ieee802_11_mac.xml"
    )
endif()

