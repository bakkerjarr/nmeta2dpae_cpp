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

#ifndef CONFIG_TC_POLICY_HPP_
#define CONFIG_TC_POLICY_HPP_

#include "../ext/spdlog/spdlog.h"

#include "config.hpp"

/**
 * Provides methods to ingest the traffic classification policy file
 * (main_policy.yaml) from the nmeta2 controller. Note: This object is stored
 * on the heap.
 */
class TcPolicy {
  public:
    TcPolicy(Config conf, std::vector<spdlog::sink_ptr> sinks);

  private:
    Config conf_;
    std::shared_ptr<spdlog::logger> tc_pol_log_;
};

#endif // CONFIG_TC_POLICY_HPP_