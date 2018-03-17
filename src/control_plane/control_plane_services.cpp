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
#include "../util/logging_util.hpp"

using namespace std;

/**
 * Setup the CntrPlaneServices object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 * @param if_name Name of NIC to sniff network traffic from for classification.
 * @param dp DataPlaneServices object to assist in controller handshake.
 * @param nmeta2dpae_version String representing the current DPAE version.
 */
CntrPlaneServices::CntrPlaneServices(Config& conf, std::vector<spdlog::sink_ptr> sinks,
                                     std::string if_name, DataPlaneServices& dp,
                                     string nmeta2dpae_version)
  : conf_(conf), dp_(dp) {
  cps_log_ = make_shared<spdlog::logger>("nmeta2dpae - control_plane_services",
                                         begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("dp_logging_level");
  loggingUtilSetLogLevel(&cps_log_, "Control Plane Services", log_level);

  /* Initialise other members from parameters. */
  if_name_ = if_name;
  nm2dpae_ver_ = nmeta2dpae_version;
}
