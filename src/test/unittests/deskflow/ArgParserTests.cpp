/*
 * Deskflow -- mouse and keyboard sharing utility
 * Copyright (C) 2014-2016 Symless Ltd.
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

#include <array>
#include <functional>

#include "deskflow/ArgParser.h"
#include "deskflow/ArgsBase.h"
#include "deskflow/ClientArgs.h"
#include "deskflow/ServerArgs.h"
#include "deskflow/ToolArgs.h"

#include <gtest/gtest.h>

TEST(ArgParserTests, isArg_abbreviationsArg_returnTrue)
{
  int i = 1;
  const int argc = 2;
  const char *argv[argc] = {"stub", "-t"};
  bool result = ArgParser::isArg(i, argc, argv, "-t", NULL);

  EXPECT_EQ(true, result);
}

TEST(ArgParserTests, isArg_fullArg_returnTrue)
{
  int i = 1;
  const int argc = 2;
  const char *argv[argc] = {"stub", "--test"};
  bool result = ArgParser::isArg(i, argc, argv, NULL, "--test");

  EXPECT_EQ(true, result);
}

TEST(ArgParserTests, isArg_missingArgs_returnFalse)
{
  int i = 1;
  const int argc = 2;
  const char *argv[argc] = {"stub", "-t"};
  static deskflow::ArgsBase argsBase;
  ArgParser argParser(NULL);
  argParser.setArgsBase(argsBase);

  bool result = ArgParser::isArg(i, argc, argv, "-t", NULL, 1);

  EXPECT_FALSE(result);
  EXPECT_EQ(true, argsBase.m_shouldExitFail);
}

TEST(ArgParserTests, searchDoubleQuotes_doubleQuotedArg_returnTrue)
{
  String command("\"stub\"");
  size_t left = 0;
  size_t right = 0;

  bool result = ArgParser::searchDoubleQuotes(command, left, right);

  EXPECT_EQ(true, result);
  EXPECT_EQ(0, left);
  EXPECT_EQ(5, right);
}

TEST(ArgParserTests, searchDoubleQuotes_noDoubleQuotedArg_returnfalse)
{
  String command("stub");
  size_t left = 0;
  size_t right = 0;

  bool result = ArgParser::searchDoubleQuotes(command, left, right);

  EXPECT_FALSE(result);
  EXPECT_EQ(0, left);
  EXPECT_EQ(0, right);
}

TEST(ArgParserTests, searchDoubleQuotes_oneDoubleQuoteArg_returnfalse)
{
  String command("\"stub");
  size_t left = 0;
  size_t right = 0;

  bool result = ArgParser::searchDoubleQuotes(command, left, right);

  EXPECT_FALSE(result);
  EXPECT_EQ(0, left);
  EXPECT_EQ(0, right);
}

TEST(ArgParserTests, splitCommandString_oneArg_returnArgv)
{
  String command("stub");
  std::vector<String> argv;

  ArgParser::splitCommandString(command, argv);

  EXPECT_EQ(1, argv.size());
  EXPECT_EQ("stub", argv.at(0));
}

TEST(ArgParserTests, splitCommandString_twoArgs_returnArgv)
{
  String command("stub1 stub2");
  std::vector<String> argv;

  ArgParser::splitCommandString(command, argv);

  EXPECT_EQ(2, argv.size());
  EXPECT_EQ("stub1", argv.at(0));
  EXPECT_EQ("stub2", argv.at(1));
}

TEST(ArgParserTests, splitCommandString_doubleQuotedArgs_returnArgv)
{
  String command("\"stub1\" stub2 \"stub3\"");
  std::vector<String> argv;

  ArgParser::splitCommandString(command, argv);

  EXPECT_EQ(3, argv.size());
  EXPECT_EQ("stub1", argv.at(0));
  EXPECT_EQ("stub2", argv.at(1));
  EXPECT_EQ("stub3", argv.at(2));
}

TEST(ArgParserTests, splitCommandString_spaceDoubleQuotedArgs_returnArgv)
{
  String command("\"stub1\" stub2 \"stub3 space\"");
  std::vector<String> argv;

  ArgParser::splitCommandString(command, argv);

  EXPECT_EQ(3, argv.size());
  EXPECT_EQ("stub1", argv.at(0));
  EXPECT_EQ("stub2", argv.at(1));
  EXPECT_EQ("stub3 space", argv.at(2));
}

TEST(ArgParserTests, getArgv_stringArray_return2DArray)
{
  std::vector<String> argArray;
  argArray.push_back("stub1");
  argArray.push_back("stub2");
  argArray.push_back("stub3 space");
  const char **argv = ArgParser::getArgv(argArray);

  String row1(argv[0]);
  String row2(argv[1]);
  String row3(argv[2]);

  EXPECT_EQ("stub1", row1);
  EXPECT_EQ("stub2", row2);
  EXPECT_EQ("stub3 space", row3);

  delete[] argv;
}

TEST(ArgParserTests, assembleCommand_stringArray_returnCommand)
{
  std::vector<String> argArray;
  argArray.push_back("stub1");
  argArray.push_back("stub2");
  String command = ArgParser::assembleCommand(argArray);

  EXPECT_EQ("stub1 stub2", command);
}

TEST(ArgParserTests, assembleCommand_ignoreSecondArg_returnCommand)
{
  std::vector<String> argArray;
  argArray.push_back("stub1");
  argArray.push_back("stub2");
  String command = ArgParser::assembleCommand(argArray, "stub2");

  EXPECT_EQ("stub1", command);
}

TEST(ArgParserTests, assembleCommand_ignoreSecondArgWithOneParameter_returnCommand)
{
  std::vector<String> argArray;
  argArray.push_back("stub1");
  argArray.push_back("stub2");
  argArray.push_back("stub3");
  argArray.push_back("stub4");
  String command = ArgParser::assembleCommand(argArray, "stub2", 1);

  EXPECT_EQ("stub1 stub4", command);
}

TEST(ArgParserTests, assembleCommand_stringArrayWithSpace_returnCommand)
{
  std::vector<String> argArray;
  argArray.push_back("stub1 space");
  argArray.push_back("stub2");
  argArray.push_back("stub3 space");
  String command = ArgParser::assembleCommand(argArray);

  EXPECT_EQ("\"stub1 space\" stub2 \"stub3 space\"", command);
}

TEST(ArgParserTests, parseToolArgs_matches_correspondingly)
{
  ArgParser parser(nullptr);
  std::map<const char *, std::function<bool(ToolArgs const &)>> tests = {
      {"--get-installed-dir", [](ToolArgs const &a) { return a.m_getInstalledDir; }},
      {"--get-profile-dir", [](ToolArgs const &a) { return a.m_getProfileDir; }},
      {"--get-arch", [](ToolArgs const &a) { return a.m_getArch; }}
  };
  for (auto const &test : tests) {
    ToolArgs toolArgs;
    EXPECT_FALSE(test.second(toolArgs));
    std::array<const char *, 2> twoArgs{LEGACY_BINARY_NAME, test.first};
    EXPECT_TRUE(parser.parseToolArgs(toolArgs, 2, twoArgs.data()));
    EXPECT_TRUE(test.second(toolArgs));
  }
  ToolArgs toolArgs;
  std::array<const char *, 2> twoArgs{LEGACY_BINARY_NAME, "--garbage"};
  EXPECT_FALSE(parser.parseToolArgs(toolArgs, 2, twoArgs.data()));
}

TEST(ArgParserTests, parseServerArgs_parses_each_category)
{
  ArgParser parser(nullptr);
  deskflow::ServerArgs args;
  args.m_daemon = false;
  char const *argv[] = {
      "deskflow", "--help"
#if WINAPI_MSWINDOWS
      ,
      "--exit-pause"
#elif WINAPI_XWINDOWS
      ,
      "--no-xinitthreads"
#endif
      ,
      "--res-w", "888"
  };
  EXPECT_TRUE(parser.parseServerArgs(args, sizeof(argv) / sizeof(argv[0]), argv));
  EXPECT_EQ(args.m_shouldExitOk, true);
}

TEST(ArgParserTests, parseClientArgs_parses_single_help)
{
  ArgParser parser(nullptr);
  deskflow::ClientArgs args;
  args.m_daemon = false;
  char const *argv[] = {
      "deskflow",
      "--help"
#if WINAPI_MSWINDOWS
      ,
      "--exit-pause"
#elif WINAPI_XWINDOWS
      ,
      "--no-xinitthreads"
#endif
      ,
      "--res-w",
      "888",
      "127.0.0.1"
  };
  EXPECT_TRUE(parser.parseClientArgs(args, sizeof(argv) / sizeof(argv[0]), argv));
  EXPECT_EQ(args.m_shouldExitOk, true);
}
