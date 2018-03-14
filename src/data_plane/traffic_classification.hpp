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

#ifndef DATA_PLANE_TRAFFIC_CLASSIFICATION_HPP_
#define DATA_PLANE_TRAFFIC_CLASSIFICATION_HPP_

#include "../ext/spdlog/spdlog.h"

#include "../config/config.hpp"

/**
 * Provides methods to ingest the classification policy, check packets against
 * policy, calling appropriate classifiers and returning actions.
 */
class TraffClass {
  public:
    TraffClass(Config& conf, std::vector<spdlog::sink_ptr> sinks);
    TraffClass(const TraffClass& other);

  private:
    Config& conf_;
    std::shared_ptr<spdlog::logger> tc_log_;
    /* Initialise Identity Harvest flags (they get set at DPAE join time) */
    /* TODO: Turn this into an unsigned char and bitshift for flags*/
    int id_arp_ = 0;
    int id_lldp_ = 0;
    int id_dns_ = 0;
    int id_dhcp_ = 0;
    /* TODO: Declare a vector for holding custom classifiers */
    /* Declare ints for config values for elephant flow suppression */
    int suppress_flow_pkt_count_initial_;
    int suppress_flow_pkt_count_backoff_;    
};

#endif // DATA_PLANE_TRAFFIC_CLASSIFICATION_HPP_