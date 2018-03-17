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

#include "data_plane_services.hpp"
#include "../util/logging_util.hpp"

using namespace std;

void setLogLevel(std::shared_ptr<spdlog::logger>* new_logger, string component_name, string log_level);

/**
 * Setup the DataPlaneServices object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 */
DataPlaneServices::DataPlaneServices(Config& conf, std::vector<spdlog::sink_ptr> sinks)
  : conf_(conf), tc_(conf, sinks), sniff_(conf, sinks, tc_) {
  dps_log_ = make_shared<spdlog::logger>("nmeta2dpae - data_plane_services",
                                         begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("dp_logging_level");
  loggingUtilSetLogLevel(&dps_log_, "Data Plane Services", log_level);
}

/**
 * Copy constructor for DataPlaneServices object.
 * 
 * @param other Another DataPlaneServices object to copy members from.
 */
DataPlaneServices::DataPlaneServices(const DataPlaneServices& other)
  : conf_(other.conf_), tc_(other.tc_), sniff_(other.sniff_) {
  dps_log_ = other.dps_log_;
}