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

#include "nmeta2dpaeBuildSettings.hpp"
#include "config/config.hpp"
#include "config/tc_policy.hpp"
#include "data_plane/data_plane_services.hpp"
#include "control_plane/control_plane_services.hpp"
#include "util/logging_util.hpp"

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
    Nmeta2Dpae(string config_path) : conf_(config_path){}

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

      /* Fork a process for each network interfaces that is being used for
       * classifying traffic. */
      stringstream sniff_ifnames_raw(conf_.getValue("sniff_if_names"));
      string buf;
      while(getline(sniff_ifnames_raw, buf, ',')) {
        nm2_log_->debug("Process forked for network interface: {0}", buf);
        /* TODO: We'll introduce multiple processes later on... */
        if (!startInterface(buf, sinks))
          return false;
        /* TODO: sleep for 1 second after forking a process */
      }

      /* Instantiate TC Policy class */
      TcPolicy tc_pol(conf_, sinks); // TODO: Leave here for now, move into my equivalent of cp_run later on.

    }

  private:
    Config conf_;
    shared_ptr<spdlog::logger> nm2_log_;

    /**
     * Initiate the 4 phase handshake with the controller then start the
     * classification services.
     * 
     * @param if_name Name of an interface being used for classification.
     * @param sinks spdlog sinks for creating a combined logger.
     * @return true if the interface started successfully, false otherwise.
     */    
    bool startInterface(string if_name, std::vector<spdlog::sink_ptr> sinks) {
      /* Instantiate the DataPlaneServices class. */
      DataPlaneServices dp = DataPlaneServices(conf_, sinks);

      /* Instantiate the ControlPlaneServices class. */
      string api_url = conf_.getValue("nmeta_controller_address");
      string api_port = conf_.getValue("nmeta_controller_port");
      string api_path = conf_.getValue("nmeta_api_path");
      string api_base = api_url + ":" + api_port + "/" + api_path;
      nm2_log_->info("nmeta2 controller API endpoint set to: {0}", api_base);
      CntrPlaneServices cp = CntrPlaneServices(conf_, sinks, if_name, dp,
                                               nmeta2dpae_VERSION, api_base);
      if (!cp.initHttpLib()) {
        nm2_log_->critical("Unable to initialise libcurl, this is needed for "
                           "the DPAE to make API requests to the controller.");
        return false;
      }
      
      /* A while true loop is used in case the handshake process fails and
       * needs to restart. */
      while(1) {
        /* Start phase 1 to connect to the controller. */
        nm2_log_->info("Controller handshake phase 1 initiated for interface {0}", if_name);
        while(1) {
          if (!cp.cpHandshakePhase1(if_name, NULL)) {
            nm2_log_->error("Phase 1 to join controller failed, will retry "
                            "for interface: {0}", if_name);
          }
          break;
/* PYTHON IMPLEMENTATION TO FOLLOW
                result = self.controlchannel.phase1(self.api_base, if_name)
                if not isinstance(result, dict):
                    self.logger.error("Phase 1 join to controller failed, "
                                        "will retry, "
                                    "interface=%s result=%s", if_name, result)
                    time.sleep(3)
                    phase1_connected = 0 // in C++ just use continue
                    continue

                if not 'dpae2ctrl_mac' in result:
                    self.logger.error("Phase 1 join to controller failed, "
                                        "will retry, "
                                    "interface=%s", if_name)
                    time.sleep(3)
                    phase1_connected = 0 // in C++ just use continue
                    continue
                else:
                    dpae2ctrl_mac = result['dpae2ctrl_mac']
                    ctrl2dpae_mac = result['ctrl2dpae_mac']
                    dpae_ethertype = result['dpae_ethertype']
                    location = result['location']
                    phase1_connected = 1 // in C++ use break
*/
        }
        nm2_log_->info("Controller handshake phase 1 active for interface {0}", if_name);
        break;
      }
      return true;
    }

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
        nm2_log_->info("Logging will be enabled for: {0}.", names);
      }
      string log_level = conf_.getValue("nmeta_dpae_logging_level");
      loggingUtilSetLogLevel(&nm2_log_, "nmeta2 DPAE Core", log_level);      
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
  /* Perform a sanity check to ensure that the correct number of
   * command-line arguments have been provided. */
  if (argc != 2) {
      cerr << "[CRITICAL] Path to nmeta2 DPAE configuration file was not "
            "provided." << endl;
      return 1;
  }
  cout << "[INFO] Starting nmeta2dape version: " << nmeta2dpae_VERSION << endl;

  /* Start nmeta2 DPAE */
  string arg_config_path(argv[1]);
  Nmeta2Dpae dpae(arg_config_path);
  if (!dpae.run())
    return 1;

  return 0;
}
