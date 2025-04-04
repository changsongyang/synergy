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

set(target ${DAEMON_BINARY_NAME})

file(GLOB headers "*.h")
file(GLOB sources "*.cpp")

# Daemon is only needed on Windows for elevating processes to deal with UAC.
if(WIN32)
  add_executable(${target} WIN32 ${sources}
                                 ${PROJECT_BINARY_DIR}/src/version.rc)

  target_link_libraries(
    ${target}
    arch
    base
    io
    ipc
    mt
    net
    platform
    app
    ${libs}
    Qt6::Core)

endif()
