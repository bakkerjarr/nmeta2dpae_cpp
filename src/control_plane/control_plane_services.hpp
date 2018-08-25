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

#ifndef CONTROL_PLANE_CONTROL_PLANE_SERVICES_HPP_
#define CONTROL_PLANE_CONTROL_PLANE_SERVICES_HPP_

#include "../ext/spdlog/spdlog.h"
#include "../util/json_body.hpp"

#include "../config/config.hpp"
#include "control_plane_api_mgmt.hpp"
#include "../data_plane/data_plane_services.hpp"

/**
 * Provides methods to run the services on the data-plane.
 */
class CntrPlaneServices {
  public:
    CntrPlaneServices(Config& conf, std::vector<spdlog::sink_ptr> sinks,
                      std::string if_name, DataPlaneServices& dp,
                      std::string nmeta2dpae_version, std::string api_base,
                      std::string hostname_dpae);
    bool cpHandshakePhase1(std::string if_name, rapidjson::Document* response);
    bool initHttpLib(); 

  private:
    Config& conf_;
    std::shared_ptr<spdlog::logger> cps_log_;
    CntrPlaneApiMgmt cp_api_mgmt_;
    DataPlaneServices dp_;
    std::string if_name_;
    std::string nm2dpae_ver_;
    std::string hostname_dpae_;
    char uuid_dpae_[37];
    float keepalive_interval_;
    int keepalive_retries_;
    int phase3_sniff_wait_time_;
    int phase3_queue_reads_;
    int phase3_sniff_dc_timeout_;
};

#endif // CONTROL_PLANE_CONTROL_PLANE_SERVICES_HPP_