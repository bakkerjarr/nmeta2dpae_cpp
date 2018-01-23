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

#include "nmeta2dpaeBuildSettings.hpp"
#include "Config/config.hpp"

using namespace std;

int main (int argc, char* argv[]) {
  cout << "[INFO] Running nmeta2dape version: " << nmeta2dpae_VERSION << endl;

  if (argc != 2) {
      cerr << "[CRITICAL] Path to nmeta2 DPAE configuration file was not "
            "provided." << endl;
      return 1;
  }

  string config_path(argv[1]);

  Config conf(config_path);
  if (!conf.readConfig()) {
    cerr << "[CRITICAL] Unable to load nmeta2 configuration. Exiting..."
        << endl;
    return 1;
  }

  /*
  Only use logging once config has been checked. This is so logging
  configuration can be read in from the config file and put into use.

  Until this point, just print to stdout. Once config is completed, then
  the main DPAE object can start the logging facility.
  */

  return 0;
}
