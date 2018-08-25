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

#include "uuid/uuid.h"

#include "control_plane_services.hpp"
#include "../util/logging_util.hpp"


#include <iostream>


using namespace std;

/**
 * Setup the CntrPlaneServices object.
 * 
 * @param conf Configuration for nmeta2 DPAE.
 * @param sinks spdlog sinks for creating a combined logger.
 * @param if_name Name of NIC to sniff network traffic from for classification.
 * @param dp DataPlaneServices object to assist in controller handshake.
 * @param nmeta2dpae_version String representing the current DPAE version.
 * @param api_base Completed URL for controller API endpoint.
 * @param hostname_dpae Hostname of the machine running the DPAE application.
 */
CntrPlaneServices::CntrPlaneServices(Config& conf, vector<spdlog::sink_ptr> sinks,
                                     string if_name, DataPlaneServices& dp,
                                     string nmeta2dpae_version, string api_base,
                                     string hostname_dpae)
  : conf_(conf), dp_(dp), cp_api_mgmt_(api_base) {
  cps_log_ = make_shared<spdlog::logger>("nmeta2dpae - control_plane_services",
                                         begin(sinks), end(sinks));

  /* Set the log level on the logger. */
  string log_level = conf_.getValue("dp_logging_level");
  loggingUtilSetLogLevel(&cps_log_, "Control Plane Services", log_level);

  /* Get configuration parameters for the 'keep alive' heartbeat. */
  keepalive_interval_ = stof(conf_.getValue("keepalive_interval"));
  keepalive_retries_ = stoi(conf_.getValue("keepalive_retries"));

  /* Get configuration parameters for sniff discover timings. */
  phase3_sniff_wait_time_ = stoi(conf_.getValue("phase3_sniff_wait_time"));
  phase3_queue_reads_ = stoi(conf_.getValue("phase3_queue_reads"));
  phase3_sniff_dc_timeout_ = stoi(conf_.getValue("phase3_sniff_dc_timeout"));

  /* Initialise other members from parameters. */
  if_name_ = if_name;
  nm2dpae_ver_ = nmeta2dpae_version;

  /* Get the machine's hostname and generate a UUID. */
  hostname_dpae_ = hostname_dpae;
  uuid_t uuid_generated;
  uuid_generate(uuid_generated);
  uuid_unparse(uuid_generated, uuid_dpae_);
  cps_log_->debug("Hostname: {0}\tUUID: {1}", hostname_dpae_, uuid_dpae_);
}

/**
 * Perform phase 1 of the control plane handshake with the nmeta2 controller.
 * 
 * @param if_name Name of the interface being used for classification..
 * @param response Used to write the JSON response from the controller.
 * @return true if phase 1 completed successfully, false otherwise.
 */
bool CntrPlaneServices::cpHandshakePhase1(string if_name,
                                          rapidjson::Document* response) {
  cps_log_->info("Phase 1 disconnected {0}", hostname_dpae_);
  string err_msg;
  /* Connect to the controller API via an HTTP/1.1 session. */
  /* TODO: write util code for making API requests. It'll wrap around libcurl
   * for now but I might switch it out later. Will need to build unit tests for
   * the wrapper as well.. */
  if (!cp_api_mgmt_.initConnection(&err_msg)) {
    cps_log_->error("Connection initialisation failure: {0}", err_msg);
    return false;
  }
  cps_log_->debug("cURL connection initialisation completed successfully.");

  /* TODO: My cURL wrapper will need to use curl_easy_send and curl_easy_recv
   * as Matt's protocol will need a bit of rework for it to handle a callback
   * approach. Need to research how to get this to work, https://curl.haxx.se/libcurl/c/sendrecv.html
   * might be of help. Note that it has a function for waiting on a socket
   * before sending and receiving data.
   * 
   * I can use the callback approach for the handshake part of nmeta2dpae as
   * curl_easy_perform blocks and waits for a response (just need to set a
   * timeout...). In this case however, I will need a separate class for
   * handling classification results as I intend to use the non-block libcurl
   * MULTI API. Time to rename things! */


  return true;
}

 /**
  * Setup the program environment for the HTTP request library. This is
  * libcurl in this case. This function needs to be called at least once.
  * 
  * @return true if the program environment could be successfully set up,
  *         false otherwise.
  */
 bool CntrPlaneServices::initHttpLib() {
   cps_log_->info("Initialising the program environment for libcurl...");
   if (curl_global_init(CURL_GLOBAL_DEFAULT))
    return false;
  return true;
 }
  