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

/* A template used to validate configuration files and assign default values if
 * if attributes are missing. */
static const map<string, string> CONFIG_TEMPLATE = {
  {"nmeta_controller_address", "10.1.0.3"},
  {"nmeta_controller_port", "8080"},
  {"nmeta_api_path", "nmeta/api"},
  {"nmeta_dpae_logging_level", "INFO"},
  {"sniff_logging_level", "INFO"},
  {"controlplane_logging_level", "INFO"},
  {"dp_logging_level", "INFO"},
  {"tc_logging_level", "INFO"},
  {"tc_policy_dpae_logging_level", "DEBUG"},
  {"requests_logging_level", "INFO"},
  {"syslog_enabled", "0"},
  {"logfacility", "19"},
  {"console_log_enabled", "1"},
  {"coloredlogs_enabled", "1"},
  {"console_format", "%(levelname)s, %(name)s %(funcName)s, %(message)s"},
  {"sniff_if_names", "eth1"},
  {"phase3_sniff_wait_time", "1"},
  {"phase3_queue_reads", "5"},
  {"phase3_sniff_dc_timeout", "5"},
  {"phase2_max_retries", "3"},
  {"phase3_max_retries", "3"},
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
  this->config_loaded_ = false; // configuration has not yet been loaded
  this->config_path_ = config_path;
}

/**
 * Copy constructor for Config object.
 * 
 * @param other Another Config object to copy members from.
 */
Config::Config(const Config &other){
    config_loaded_ = other.config_loaded_;
    config_path_ = other.config_path_;
    config_yaml_ = other.config_yaml_;
}

/**
 * Return a string representation of the value of an configuration attribute.
 * 
 * @param config_key Name of the configuration attribute to return.
 * @return A string representation of a configuration value. The caller must
 *    cast this to another type if needed.
 */
string Config::getValue(string config_key) {
  return config_yaml_[config_key].as<string>();
}

/**
 * Parse the configuration file and check for correctness. The configuration
 * data is stored if the file if legal.
 * 
 * @return true if successful, false otherwise.
 */
bool Config::readConfig() {
  /* Check that the configuration has not already been loaded. This may be
   * triggered by accident (someone has mistakingly called this method) or
   * someone malicious. */
  if (config_loaded_) {
    cerr << "[CRITICAL] nmeta2 DPAE configuration has already been "
        "loaded." << endl;
    return false;
  }

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

  /* TODO: Check that the values provided for each attribute are valid. */

  /* Assign default values if attributes are missing from the parsed config. */
  provideDefaultConfig(&config_yaml_);

  cout << "[INFO] nmeta2 DPAE configuration loaded successfully." << endl;
  config_loaded_ = true;
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
      cout << "[WARNING] Invalid attribute found in parsed config: " << key
          << endl;
      for_deletion.push_back(key);
    }
  }

  /* Remove invalid keys from the parsed config. */
  list<string>::iterator key;
  for (key = for_deletion.begin(); key != for_deletion.end(); ++key) {
    cout << "[INFO] Omitting invalid attribute \'" << *key << "\' from "
        "parsed configuration." << endl;
    (*parsed_config).remove(*key);    
  }
}

/**
 * Iterate over the parsed and cleansed YAML configuration and provide default
 * values for any missing attributes.
 * 
 * @param parsed_config Address of YAML object that contains the parsed
 *    configuation.
 */
void Config::provideDefaultConfig(YAML::Node *parsed_config) {
  map<string, string>::const_iterator it_m;
  for (it_m = CONFIG_TEMPLATE.begin(); it_m != CONFIG_TEMPLATE.end(); ++it_m) {
    /* Check that this attribute exists within the parsed configuration. */
    string conf_key = it_m->first;
    if (!(*parsed_config)[conf_key]) {
      /* A missing attribute has been found, we must assign a default value. */
      cout << "[WARNING] Attribute missing from parsed configration: "
          << conf_key << endl;
      string conf_val = it_m->second;
      cout << "[INFO] Creating missing attribute \'" << conf_key << "\' with "
          "default value: " << conf_val << endl;
      (*parsed_config)[conf_key] = conf_val;
    }
  }
}
