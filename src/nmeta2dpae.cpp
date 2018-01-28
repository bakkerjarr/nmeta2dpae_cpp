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
#include <syslog.h>

#include "ext/spdlog/spdlog.h"

#include "nmeta2dpaeBuildSettings.hpp"
#include "config/config.hpp"
#include "config/tc_policy.hpp"

using namespace std;

/**
 * The core object used to setup and run the Data Path Auxiliary Engine.
 */
class Nmeta2Dpae {
  public:
    /**
     * Initialise the core object used to run the DPAE.
     * 
     * @param config_path Path to the nmeta2 DPAE configuration file.
     */
    Nmeta2Dpae(string config_path) : conf_(config_path) {
      cout << "[INFO] Starting nmeta2dape version: " << nmeta2dpae_VERSION
          << endl;
    }

    /**
     * Start the DPAE application.
     * 
     * @return true if the application exits gracefully, false otherwise.
     */
    bool run() {
      /* Parse the configuration. */
      if (!readConfig()) {
        cerr << "[CRITICAL] Unable to load nmeta2 dpae configuration. "
            << "Exiting..." << endl;
        return false;
      }

      /* Configure logging and establish a logger for this object. We store the
       * sinks so that they may be passed onto other objects. */
      vector<spdlog::sink_ptr> sinks = prepareLogger();
      
      nm2_log_->debug("Debug");
      nm2_log_->info("Information");
      nm2_log_->warn("Warning");
      nm2_log_->error("Error");
      nm2_log_->critical("Critical!");

      /* Instantiate TC Policy class */
      TcPolicy tc_pol(conf_, sinks);

    }

  private:
    Config conf_;
    shared_ptr<spdlog::logger> nm2_log_;
    
    /**
     * Setup and register the logger. There could be multiple sinks being used
     * at the same time, such as stdout and syslog.
     * 
     * @return The sinks to be used by other loggers.
     */
    vector<spdlog::sink_ptr> prepareLogger() {
      cout << "[INFO] Configuring the nmeta2dpae logger. Note that the "
          << "console log format cannot be changed." << endl;

      /* spdlog has the concept of a sink: an object that writes a log to its
       * target. This could be stdout, syslog, a file etc. The vector below is
       * used to group all sinks together to create a combined logger. This
       * allows us to make a single function call to write out to many targets.
       * Furthermore, this will be used by the multiple loggers in the nmeta2
       * DPAE application. */
      vector<spdlog::sink_ptr> sinks;
      vector<string> sink_names;

      /* Check if console logging has been enabled. */
      if (stoi(conf_.getValue("console_log_enabled"))) {
        sink_names.push_back("console");
        /* Is colourised logging being used? */
        if (stoi(conf_.getValue("coloredlogs_enabled"))) {
          /* Create a colourised sink for the logger */
          sinks.push_back(
                    make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
        } else {
          /* Create a non-colourised sink for the logger */
          sinks.push_back(make_shared<spdlog::sinks::stdout_sink_st>());
        }
      }
      /* Check if syslog logging has been enabled. */
      if (stoi(conf_.getValue("syslog_enabled"))) {
        sink_names.push_back("syslog");
        int syslog_facility = stoi(conf_.getValue("logfacility"));
        /* The three parameters for the syslog sink are: ident, option and
         * facility. See the man page for syslog and syslog.h for more
         * information. A brief explanation of the parameters:
         *    - ident: Can be used to identify the process issuing the log.
         *    - option: Flags which control the operation of syslog.
         *    - facility: Specifies the type of program that is logging the
         *                message. This number needs to be bit shifted to the
         *                left by 3. */
        sinks.push_back(make_shared<spdlog::sinks::syslog_sink>("nmeta2dpae",
                                                LOG_PID, syslog_facility<<3)); 
      }

      /* Combine the sinks into the one logger. */
      nm2_log_ = make_shared<spdlog::logger>("nmeta2dpae",
                                  begin(sinks), end(sinks));
      
      /* Set the log level on the logger. */
      string log_level = conf_.getValue("nmeta_dpae_logging_level_c");
      if (sinks.size() == 0) {
        cout << "[WARNING] Logging disabled for nmeta2 DPAE. This will be the "
            "last message." << endl;
      } else {
        string names;
        vector<string>::iterator s_name;
          for (s_name = sink_names.begin(); s_name != sink_names.end(); ++s_name) {
            if (s_name == sink_names.begin()) 
              names.append(*s_name);
            else
              names.append(", ").append(*s_name);
          }
        nm2_log_->info("nmeta2 DPAE logging enabled for: {0}. Minimum logging "
                       "level will be: {1}", names, log_level);
      }
      if (!log_level.compare("CRITICAL"))
        nm2_log_->set_level(spdlog::level::critical);
      else if (!log_level.compare("ERROR"))
        nm2_log_->set_level(spdlog::level::err);
      else if (!log_level.compare("WARNING"))
        nm2_log_->set_level(spdlog::level::warn);
      else if (!log_level.compare("INFO"))
        nm2_log_->set_level(spdlog::level::info);
      else /* Treat anything else as debug level. */
        nm2_log_->set_level(spdlog::level::debug);
      
      return sinks;
    }

    /**
     * Trigger the Config class to load the nmeta2 DPAE configuration.
     * 
     * @return true if successful, false otherwise.
     */
    bool readConfig() {
      return conf_.readConfig();
    }
};

int main (int argc, char* argv[]) {
  /* Perform a sanity check to ensure that the correct number of *
   * command-line arguments have been provided. */
  if (argc != 2) {
      cerr << "[CRITICAL] Path to nmeta2 DPAE configuration file was not "
            "provided." << endl;
      return 1;
  }

  /* Start nmeta2 DPAE */
  string arg_config_path(argv[1]);
  Nmeta2Dpae dpae(arg_config_path);
  if (!dpae.run())
    return 1;

  return 0;
}
