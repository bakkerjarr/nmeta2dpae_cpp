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

#include <iostream>
#include <list>
#include <map>

#include "config.hpp"

using namespace std;

/* A template used to validate configuration files and assign default values */
/* if attributes are missing. */
static const map<string, string> CONFIG_TEMPLATE = {
  {"nmeta_controller_address", "10.1.0.3"},
  {"nmeta_controller_port", "8080"},
  {"nmeta_api_path", "nmeta/api"},
  {"nmeta_dpae_logging_level_c", "INFO"},
  {"sniff_logging_level_c", "INFO"},
  {"controlplane_logging_level_c", "INFO"},
  {"dp_logging_level_c", "INFO"},
  {"tc_logging_level_c", "INFO"},
  {"tc_policy_dpae_logging_level_c", "DEBUG"},
  {"requests_logging_level", "INFO"},
  {"nmeta_dpae_logging_level_s", "INFO"},
  {"sniff_logging_level_s", "INFO"},
  {"controlplane_logging_level_s", "INFO"},
  {"dp_logging_level_s", "INFO"},
  {"tc_logging_level_s", "INFO"},
  {"tc_policy_dpae_logging_level_s", "DEBUG"},
  {"syslog_enabled", "0"},
  {"loghost", "localhost"},
  {"logport", "514"},
  {"logfacility", "19"},
  {"syslog_format", "sev=%(levelname)s module=%(name)s func=%(funcName)s %(message)s"},
  {"console_log_enabled", "1"},
  {"coloredlogs_enabled", "1"},
  {"console_format", "%(levelname)s, %(name)s %(funcName)s, %(message)s"},
  {"sniff_if_names", "eth1"},
  {"phase3_sniff_wait_time", "1"},
  {"phase3_queue_reads", "5"},
  {"phase3_sniff_dc_timeout", "5"},
  {"keepalive_interval", "1"},
  {"keepalive_retries", "3"},
  {"mongo_addr", "localhost"},
  {"mongo_port", "27017"},
  {"suppress_flow_pkt_count_initial", "8"},
  {"suppress_flow_pkt_count_backoff", "500"}
};

/**
 * Prepare a Config object for parsing a configuation file.
 * 
 * @param config_path Path to the nmeta2 DPAE configuration file.
 */
Config::Config(string config_path) {
  cout << "[INFO] Initialising configuration parser...\n";
  this->config_path_ = config_path;
}

/**
 * Parse the configuration file and check for correctness. The configuration
 * data is stored if the file if legal.
 * 
 * @return true if successful, false otherwise.
 */
bool Config::readConfig() {
  cout << "[INFO] Reading nmeta2 DPAE configuration from: " << config_path_
      << endl;
    
  /* Attempt to open and read the entire file. */
  try {
    config_yaml_ = YAML::LoadFile(config_path_);
  } catch (const YAML::BadFile &ex) {
    cerr << "[CRITICAL] Exception raised: cannot find or unable to read file: "
        << config_path_ << endl;
        return false;
  } catch (const exception &ex) {
    cerr << "[CRITICAL] Exception raised: " << ex.what() << endl;
    return false;
  }

  /* Verify that the YAML file contains legal keys. */
  cleanseParsedConfig(&config_yaml_);

  /* Assign default values if attributes are missing from the parsed config. */
  // TODO: Implement a method for the above comment.

  cout << "[INFO] nmeta2 DPAE configuration loaded successfully." << endl;
  return true;
}

/**
 * Iterate over the parsed YAML configuration and remove attribute keys that
 * are not valid.
 * 
 * @param parsed_config Address of YAML object that contains the parsed
 *    configuation.
 */
void Config::cleanseParsedConfig(YAML::Node *parsed_config) {
  /* Determine what keys are not valid and need to be removed. */
  list <string> for_deletion;
  YAML::const_iterator it_y;
  for (it_y = (*parsed_config).begin(); it_y != (*parsed_config).end(); ++it_y) {
    string key = it_y->first.as<std::string>();
    if (!CONFIG_TEMPLATE.count(key)) {
      /* An invalid key was found, take note of it. */
      cerr << "[ERROR] Invalid attribute found in parsed config: " << key << endl;
      for_deletion.push_back(key);
    }
  }

  /* Remove invalid keys from the parsed config. */
  list<string>::iterator it_l;
  for (it_l = for_deletion.begin(); it_l != for_deletion.end(); ++it_l) {
    cout << "[INFO] Omitting invalid attribute \'" << *it_l << "\' from "
        "parsed configuration." << endl;
    (*parsed_config).remove(*it_l);    
  }
}