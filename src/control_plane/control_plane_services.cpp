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

#include "control_plane_services.hpp"

using namespace std;

/**
 * Setup the CntrPlaneServices object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 */
CntrPlaneServices::CntrPlaneServices(Config& conf, std::vector<spdlog::sink_ptr> sinks,
                                     std::string if_name, DataPlaneServices& dp)
  : conf_(conf), dp_(dp) {
  cps_log_ = make_shared<spdlog::logger>("nmeta2dpae - control_plane_services",
                                         begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("dp_logging_level");
  cps_log_->info("Control Plane Services object created. Minimum logging level "
                 "will be: {0}", log_level);
  if (!log_level.compare("CRITICAL"))
    cps_log_->set_level(spdlog::level::critical);
  else if (!log_level.compare("ERROR"))
    cps_log_->set_level(spdlog::level::err);
  else if (!log_level.compare("WARNING"))
    cps_log_->set_level(spdlog::level::warn);
  else if (!log_level.compare("INFO"))
    cps_log_->set_level(spdlog::level::info);
  else /* Treat anything else as debug level. */
    cps_log_->set_level(spdlog::level::debug);

  if_name_ = if_name;
}
