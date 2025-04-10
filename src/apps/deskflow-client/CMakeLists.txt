# Deskflow -- mouse and keyboard sharing utility
# Copyright (C) 2012-2024 Symless Ltd.
# Copyright (C) 2009-2012 Nick Bolton
#
# This package is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# found in the file LICENSE that should have accompanied this file.
#
# This package is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(client_source_name "deskflow-client")
set(target ${CLIENT_BINARY_NAME})
set(sources ${client_source_name}.cpp)

if(WIN32)
  file(GLOB arch_headers "MSWindows*.h")
  file(GLOB arch_sources "MSWindows*.cpp")
  list(
    APPEND
    sources
    resource.h
    ${client_source_name}.ico
    ${client_source_name}.rc
    tb_error.ico
    tb_idle.ico
    tb_run.ico
    tb_wait.ico
    ${client_source_name}.exe.manifest
    ${PROJECT_BINARY_DIR}/src/version.rc)
elseif(APPLE)
  file(GLOB arch_headers "OSX*.h")
  file(GLOB arch_sources "OSX*.cpp")
elseif(UNIX)
  file(GLOB arch_headers "XWindows*.h")
  file(GLOB arch_sources "XWindows*.cpp")
endif()

list(APPEND sources ${arch_sources})
list(APPEND headers ${arch_headers})

if(ADD_HEADERS_TO_SOURCES)
  list(APPEND sources ${headers})
endif()

add_executable(${target} ${sources})
target_link_libraries(
  ${target}
  arch
  base
  client
  io
  mt
  net
  ipc
  platform
  server
  app
  ${libs})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  install(TARGETS ${target} DESTINATION ${DESKFLOW_BUNDLE_BINARY_DIR})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  install(TARGETS ${target} DESTINATION bin)
endif()
