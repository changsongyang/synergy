/*
 * Deskflow -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2002 Chris Schoeneman
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

#include "deskflow/AppUtil.h"

#define ARCH_APP_UTIL AppUtilUnix

class IEventQueue;

class AppUtilUnix : public AppUtil
{
public:
  AppUtilUnix(IEventQueue *events, bool runEventLoop);
  virtual ~AppUtilUnix();

  int run(int argc, char **argv) override;
  void startNode() override;
  std::vector<String> getKeyboardLayoutList() override;
  String getCurrentLanguageCode() override;
  void showNotification(const String &title, const String &text) const override;
};
