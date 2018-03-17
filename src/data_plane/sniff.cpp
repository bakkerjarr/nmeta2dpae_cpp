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

#include "sniff.hpp"
#include "../util/logging_util.hpp"

using namespace std;

/**
 * Setup the Sniff object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 * @param tc TraffClass object used for classifying sniffed packets.
 */
Sniff::Sniff(Config& conf, std::vector<spdlog::sink_ptr> sinks, TraffClass& tc)
  : conf_(conf), tc_(tc) {
  sniff_log_ = make_shared<spdlog::logger>("nmeta2dpae - sniff",
                                         begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("sniff_logging_level");
  loggingUtilSetLogLevel(&sniff_log_, "Sniff", log_level);
}