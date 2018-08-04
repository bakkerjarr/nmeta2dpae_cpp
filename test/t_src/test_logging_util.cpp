/** Copyright 2017 Jarrod N. Bakker
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "gtest/gtest.h"

#include "../../src/ext/spdlog/sinks/null_sink.h"
#include "../../src/util/logging_util.hpp"

using namespace std;

TEST(loggingUtilSetLogLevel, set_level_CRITICAL) {
  auto null_sink = make_shared<spdlog::sinks::null_sink_st> ();
  auto t_log = make_shared<spdlog::logger>("test_logger", null_sink);
  loggingUtilSetLogLevel(&t_log, "unit_test", "CRITICAL");
  EXPECT_EQ(spdlog::level::critical, t_log->level());
}

TEST(loggingUtilSetLogLevel, set_level_ERROR) {
  auto null_sink = make_shared<spdlog::sinks::null_sink_st> ();
  auto t_log = make_shared<spdlog::logger>("test_logger", null_sink);
  loggingUtilSetLogLevel(&t_log, "unit_test", "ERROR");
  EXPECT_EQ(spdlog::level::err, t_log->level());
}

TEST(loggingUtilSetLogLevel, set_level_WARNING) {
  auto null_sink = make_shared<spdlog::sinks::null_sink_st> ();
  auto t_log = make_shared<spdlog::logger>("test_logger", null_sink);
  loggingUtilSetLogLevel(&t_log, "unit_test", "WARNING");
  EXPECT_EQ(spdlog::level::warn, t_log->level());
}

TEST(loggingUtilSetLogLevel, set_level_INFO) {
  auto null_sink = make_shared<spdlog::sinks::null_sink_st> ();
  auto t_log = make_shared<spdlog::logger>("test_logger", null_sink);
  loggingUtilSetLogLevel(&t_log, "unit_test", "INFO");
  EXPECT_EQ(spdlog::level::info, t_log->level());
}

TEST(loggingUtilSetLogLevel, set_level_DEBUG) {
  auto null_sink = make_shared<spdlog::sinks::null_sink_st> ();
  auto t_log = make_shared<spdlog::logger>("test_logger", null_sink);
  loggingUtilSetLogLevel(&t_log, "unit_test", "DEBUG");
  EXPECT_EQ(spdlog::level::debug, t_log->level());
}
