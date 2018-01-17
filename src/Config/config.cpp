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
#include <libgen.h>

#include "config.hpp"

using namespace std;

/**
 * Default constructor. Configure the path 
 * 
 * @param config_path Path to the nmeta2 DPAE configuration file.
 */
Config::Config(string config_path)
{
  cout << "[INFO] Initialising configuration parser...\n";
  config_path_ = config_path;
}

/**
 * Parse the configuration file and check for correctness. The configuration
 * data is stored if the file if legal.
 * 
 * @return true if successful, false otherwise.
 */
bool Config::readConfig(){
  cout << "[INFO] Reading nmeta2 DPAE configuration from: " << config_path_
      << endl;

  config_yaml_ = YAML::LoadFile(config_path_);

  cout << "[INFO] nmeta2 DPAE configuration loaded successfully." << endl;
  return true;
}


/*
  Only use logging once config has been checked. This is so logging
  configuration can be read in from the config file and put into use.

  Until this point, just print to stdout. Once config is completed, then
  the main DPAE object can start the logging facility.
*/


/*
        std::string config_directory = "";
        static std::string config_filename = "config.yaml";
        //config_yaml;
        std::string working_directory;
*/