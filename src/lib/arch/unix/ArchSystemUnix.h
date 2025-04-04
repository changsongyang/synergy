/*
 * Deskflow -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2004 Chris Schoeneman
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "arch/IArchSystem.h"

#define ARCH_SYSTEM ArchSystemUnix

//! Unix implementation of IArchString
class ArchSystemUnix : public IArchSystem
{
public:
  ArchSystemUnix();
  virtual ~ArchSystemUnix();

  // IArchSystem overrides
  virtual std::string getOSName() const;
  virtual std::string getPlatformName() const;
  virtual std::string setting(const std::string &) const;
  virtual void setting(const std::string &, const std::string &) const;
  virtual std::string getLibsUsed(void) const;
  virtual void clearSettings() const;

#ifndef __APPLE__
  enum class InhibitScreenServices
  {
    kScreenSaver,
    kSessionManager
  };
  static bool DBusInhibitScreenCall(InhibitScreenServices serviceID, bool state, std::string &error);
#endif
};
