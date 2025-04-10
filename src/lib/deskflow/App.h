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

#include "base/EventQueue.h"
#include "base/Log.h"
#include "base/String.h"
#include "common/common.h"
#include "deskflow/IApp.h"
#include "ipc/IpcClient.h"

#if SYSAPI_WIN32
#include "deskflow/win32/AppUtilWindows.h"
#elif SYSAPI_UNIX
#include "deskflow/unix/AppUtilUnix.h"
#endif

#include <stdexcept>

class IArchTaskBarReceiver;
class BufferedLogOutputter;
class ILogOutputter;
class FileLogOutputter;
namespace deskflow {
class Screen;
}
class IEventQueue;
class SocketMultiplexer;

typedef IArchTaskBarReceiver *(*CreateTaskBarReceiverFunc)(const BufferedLogOutputter *, IEventQueue *events);

class App : public IApp
{
public:
  class XNoEiSupport : public std::runtime_error
  {
  public:
    XNoEiSupport() : std::runtime_error("libei is not supported")
    {
    }
  };

  App(IEventQueue *events, CreateTaskBarReceiverFunc createTaskBarReceiver, deskflow::ArgsBase *args,
      bool runEventLoop = true);
  App(App const &) = delete;
  App(App &&) = delete;
  virtual ~App();

  App &operator=(App const &) = delete;
  App &operator=(App &&) = delete;

  virtual void help() = 0;
  virtual void parseArgs(int argc, const char *const *argv) = 0;
  virtual void loadConfig() = 0;
  virtual bool loadConfig(const String &pathname) = 0;
  virtual const char *daemonInfo() const = 0;
  virtual std::string configSection() const = 0;

  virtual void version();
  virtual void setByeFunc(void (*bye)(int))
  {
    m_bye = bye;
  }
  virtual void bye(int error)
  {
    m_bye(error);
  }
  virtual IEventQueue *getEvents() const
  {
    return m_events;
  }

  ARCH_APP_UTIL &appUtil()
  {
    return m_appUtil;
  }
  deskflow::ArgsBase &argsBase() const
  {
    return *m_args;
  }
  int run(int argc, char **argv);
  int daemonMainLoop(int, const char **);
  void setupFileLogging();
  void loggingFilterWarning();
  void initApp(int argc, const char **argv);
  void initApp(int argc, char **argv)
  {
    initApp(argc, (const char **)argv);
  }
  void setSocketMultiplexer(SocketMultiplexer *sm)
  {
    m_socketMultiplexer = sm;
  }

  virtual IArchTaskBarReceiver *taskBarReceiver() const
  {
    return m_taskBarReceiver;
  }

  SocketMultiplexer *getSocketMultiplexer() const
  {
    return m_socketMultiplexer;
  }

  static App &instance()
  {
    assert(s_instance != nullptr);
    return *s_instance;
  }

  void (*m_bye)(int);

private:
  void handleIpcMessage(const Event &, void *);

protected:
  void initIpcClient();
  void cleanupIpcClient();
  void runEventsLoop(void *);

  IArchTaskBarReceiver *m_taskBarReceiver;
  bool m_suspended;
  IEventQueue *m_events;

private:
  deskflow::ArgsBase *m_args;
  static App *s_instance;
  FileLogOutputter *m_fileLog;
  CreateTaskBarReceiverFunc m_createTaskBarReceiver;
  ARCH_APP_UTIL m_appUtil;
  IpcClient *m_ipcClient;
  SocketMultiplexer *m_socketMultiplexer;
};

class MinimalApp : public App
{
public:
  MinimalApp(IEventQueue *events);
  virtual ~MinimalApp();

  // IApp overrides
  virtual int standardStartup(int argc, char **argv) override;
  virtual int runInner(int argc, char **argv, ILogOutputter *outputter, StartupFunc startup) override;
  virtual void startNode() override;
  virtual int mainLoop() override;
  virtual int foregroundStartup(int argc, char **argv) override;
  virtual deskflow::Screen *createScreen() override;
  virtual void loadConfig() override;
  virtual bool loadConfig(const String &pathname) override;
  virtual const char *daemonInfo() const override;
  virtual const char *daemonName() const override;
  virtual void parseArgs(int argc, const char *const *argv) override;

  //
  // App overrides
  //
  std::string configSection() const override
  {
    return "";
  }
};

#if WINAPI_MSWINDOWS
#define DAEMON_RUNNING(running_) ArchMiscWindows::daemonRunning(running_)
#else
#define DAEMON_RUNNING(running_)
#endif

#define HELP_COMMON_INFO_1                                                                                             \
  "  -d, --debug <level>      filter out log messages with priority below "                                            \
  "level.\n"                                                                                                           \
  "                             level may be: FATAL, ERROR, WARNING, NOTE, "                                           \
  "INFO,\n"                                                                                                            \
  "                             DEBUG, DEBUG1, DEBUG2.\n"                                                              \
  "  -n, --name <screen-name> use screen-name instead the hostname to "                                                \
  "identify\n"                                                                                                         \
  "                             this screen in the configuration.\n"                                                   \
  "  -1, --no-restart         do not try to restart on failure.\n"                                                     \
  "*     --restart            restart the server automatically if it fails.\n"                                         \
  "  -l  --log <file>         write log messages to file.\n"                                                           \
  "      --no-tray            disable the system tray icon.\n"                                                         \
  "      --enable-drag-drop   enable file drag & drop.\n"                                                              \
  "      --enable-crypto      enable TLS encryption.\n"                                                                \
  "      --tls-cert           specify the path to the TLS certificate file.\n"

#define HELP_COMMON_INFO_2                                                                                             \
  "  -h, --help               display this help and exit.\n"                                                           \
  "      --version            display version information and exit.\n"

#define HELP_COMMON_ARGS                                                                                               \
  " [--name <screen-name>]"                                                                                            \
  " [--restart|--no-restart]"                                                                                          \
  " [--debug <level>]"

// system args (windows/unix)
#if SYSAPI_UNIX

// unix daemon mode args
#define HELP_SYS_ARGS " [--daemon|--no-daemon]"
#define HELP_SYS_INFO                                                                                                  \
  "  -f, --no-daemon          run in the foreground.\n"                                                                \
  "*     --daemon             run as a daemon.\n"

#elif SYSAPI_WIN32

// windows args
#define HELP_SYS_ARGS " [--service <action>] [--relaunch] [--exit-pause]"
#define HELP_SYS_INFO                                                                                                  \
  "      --service <action>   manage the windows service, valid options "                                              \
  "are:\n"                                                                                                             \
  "                             install/uninstall/start/stop\n"                                                        \
  "      --relaunch           persistently relaunches process in current "                                             \
  "user \n"                                                                                                            \
  "                             session (useful for vista and upward).\n"                                              \
  "      --exit-pause         wait for key press on exit, can be useful for\n"                                         \
  "                             reading error messages that occur on exit.\n"
#endif
