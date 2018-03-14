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

#include "traffic_classification.hpp"

using namespace std;

/**
 * Setup the TraffClass object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 */
TraffClass::TraffClass(Config& conf, std::vector<spdlog::sink_ptr> sinks) : conf_(conf) {
  tc_log_ = make_shared<spdlog::logger>("nmeta2dpae - traffic_classification",
                                         begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("tc_logging_level");
  tc_log_->info("Traffic Classification object created. Minimum logging level "
                 "will be: {0}", log_level);
  if (!log_level.compare("CRITICAL"))
    tc_log_->set_level(spdlog::level::critical);
  else if (!log_level.compare("ERROR"))
    tc_log_->set_level(spdlog::level::err);
  else if (!log_level.compare("WARNING"))
    tc_log_->set_level(spdlog::level::warn);
  else if (!log_level.compare("INFO"))
    tc_log_->set_level(spdlog::level::info);
  else /* Treat anything else as debug level. */
    tc_log_->set_level(spdlog::level::debug);
  
  /* Retrieve config values for elephant flow suppression. */
  suppress_flow_pkt_count_initial_ = 
                       stoi(conf_.getValue("suppress_flow_pkt_count_initial"));
  suppress_flow_pkt_count_backoff_ = 
                       stoi(conf_.getValue("suppress_flow_pkt_count_backoff"));
  
  /* TODO: Instantiate a flow object for classifiers to work with. */
  string mongo_addr = conf_.getValue("mongo_addr");
  int mongo_port = stoi(conf_.getValue("mongo_port"));
}

/**
 * Copy constructor for TraffClass object.
 * 
 * @param other Another TraffClass object to copy members from.
 */
TraffClass::TraffClass(const TraffClass& other) : conf_(other.conf_) {
  tc_log_ = other.tc_log_;
  id_arp_ = other.id_arp_;
  id_lldp_ = other.id_lldp_;
  id_dns_ = other.id_dns_;
  id_dhcp_ = other.id_dhcp_;
  suppress_flow_pkt_count_initial_ = other.suppress_flow_pkt_count_initial_;
  suppress_flow_pkt_count_backoff_ = other.suppress_flow_pkt_count_backoff_; 
}
