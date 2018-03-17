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

#ifndef UTIL_LOGGING_UTIL_HPP_
#define UTIL_LOGGING_UTIL_HPP_

#include "../ext/spdlog/spdlog.h"

void loggingUtilSetLogLevel(std::shared_ptr<spdlog::logger>* new_logger,
                 std::string component_name, std::string log_level);


#endif // UTIL_LOGGING_UTIL_HPP_