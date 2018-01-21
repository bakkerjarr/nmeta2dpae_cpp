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
#ifndef CONFIG_CONFIG_HPP_
#define CONFIG_CONFIG_HPP_

#include <string>
#include <yaml-cpp/yaml.h>

/**
 * Ingests the configuration file for the DPAE and provides access to the
 * keys/values that it contains.
 */
class Config {
  public:
    Config(std::string config_path);
    bool readConfig();
    void getValue(std::string configKey);

  private:
    std::string config_path_;
    YAML::Node config_yaml_;
    void cleanseParsedConfig(YAML::Node *parsedConfig);
};

#endif // CONFIG_CONFIG_HPP_
