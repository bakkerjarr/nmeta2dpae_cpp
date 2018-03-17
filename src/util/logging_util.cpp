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

/**
 * Provides useful functions for handling spdlog.
 */

#include "logging_util.hpp"

using namespace std;

/**
 * Set the minimum logging level for a nmeta2 DPAE component's logger. A
 * message is printed to the logger with the name of the component and the
 * logging level being set.
 * 
 * @param new_logger Pointer to a spdlog object.
 * @param component_name Name of the nmeta2 DPAE component.
 * @param log_level The minimum logging level to be set.
 */
void loggingUtilSetLogLevel(shared_ptr<spdlog::logger>* new_logger,
                            string component_name, string log_level) {
  (*new_logger)->info("{0} object created. Minimum logging level "
                 "will be: {1}", component_name, log_level);
  if (!log_level.compare("CRITICAL"))
    (*new_logger)->set_level(spdlog::level::critical);
  else if (!log_level.compare("ERROR"))
    (*new_logger)->set_level(spdlog::level::err);
  else if (!log_level.compare("WARNING"))
    (*new_logger)->set_level(spdlog::level::warn);
  else if (!log_level.compare("INFO"))
    (*new_logger)->set_level(spdlog::level::info);
  else /* Treat anything else as debug level. */
    (*new_logger)->set_level(spdlog::level::debug);
}