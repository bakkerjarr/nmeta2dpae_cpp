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

#ifndef DATA_PLANE_SNIFF_HPP_
#define DATA_PLANE_SNIFF_HPP_

#include "../ext/spdlog/spdlog.h"

#include "../config/config.hpp"
#include "traffic_classification.hpp"

/**
 * Provides methods to sniff and process packets on a given interface.
 */
class Sniff {
  public:
    Sniff(Config& conf, std::vector<spdlog::sink_ptr> sinks, TraffClass& tc);

  private:
    Config& conf_;
    TraffClass& tc_;
    std::shared_ptr<spdlog::logger> sniff_log_; 
};

#endif // DATA_PLANE_SNIFF_HPP_