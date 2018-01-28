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

#include "tc_policy.hpp"

using namespace std;

/**
 * Setup the TcPolicy object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 */
TcPolicy::TcPolicy(Config conf, std::vector<spdlog::sink_ptr> sinks) : conf_(conf) {
  tc_pol_log_ = make_shared<spdlog::logger>("nmeta2dpae - tc_policy",
                                            begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("tc_policy_dpae_logging_level");
  tc_pol_log_->info("Traffic Classification Policy object created. Minimum "
                    "logging level will be: {0}", log_level);
  if (!log_level.compare("CRITICAL"))
    tc_pol_log_->set_level(spdlog::level::critical);
  else if (!log_level.compare("ERROR"))
    tc_pol_log_->set_level(spdlog::level::err);
  else if (!log_level.compare("WARNING"))
    tc_pol_log_->set_level(spdlog::level::warn);
  else if (!log_level.compare("INFO"))
    tc_pol_log_->set_level(spdlog::level::info);
  else /* Treat anything else as debug level. */
    tc_pol_log_->set_level(spdlog::level::debug);

  tc_pol_log_->debug("Another debug");
  tc_pol_log_->info("More information");
  tc_pol_log_->warn("One more warning");
  tc_pol_log_->error("Error, of course");
  tc_pol_log_->critical("Still critical!");
}