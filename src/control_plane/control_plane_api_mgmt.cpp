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

#include "control_plane_api_mgmt.hpp"

using namespace std;

/**
 * Initialise a CntrPlaneApiMgmt object for managing API requests for the
 * handshake and keep-alive protocols used to connect to the controller.
 * 
 * @param api_url API URL of the target nmeta2 controller.
 */
CntrPlaneApiMgmt::CntrPlaneApiMgmt(string api_url) {
  api_url_ = api_url;
  conn_ = NULL;
}

/**
 * Destructor for CntrPlaneApiMgmt class. Needed to close connections that
 * are managed by libcurl.
 */
CntrPlaneApiMgmt::~CntrPlaneApiMgmt() {
  if (conn_ != NULL)
    curl_easy_cleanup(conn_);
}

/**
 * Initialise a CntrPlaneApiMgmt object for managing API requests for the
 * handshake and keep-alive protocols used to connect to the controller.
 * 
 * @param err_msg Contains an error message if this function returns false.
 * @return true if the libcurl connection was be established, false otherwise.
 */
bool CntrPlaneApiMgmt::initConnection(string* err_msg) {
  CURLcode code;
 
  conn_ = curl_easy_init();
 
  if(conn_ == NULL) {
    *err_msg = "Unable to create cURL connection.";
    return false;
  }

  code = curl_easy_setopt(conn_, CURLOPT_ERRORBUFFER, error_buffer_);
  if(code != CURLE_OK) {
    *err_msg = "Unable to set cURL error buffer, libcurl error: ";
    err_msg->append(curl_easy_strerror(code)); // Turn error code into string
    return false;
  }
  
  /* The type of the URL string must be cast to char*. It cannot be string or
   * const char*. */
  code = curl_easy_setopt(conn_, CURLOPT_URL, (char*) api_url_.c_str());
  if(code != CURLE_OK) {
    *err_msg = "Unable to set URL for cURL connection: ";
    err_msg->append(error_buffer_);
    return false;
  }

  code = curl_easy_setopt(conn_, CURLOPT_TCP_KEEPALIVE, 1L);
  if(code != CURLE_OK) {
    *err_msg = "Unable to set TCP keep-alive for cURL connection: ";
    err_msg->append(error_buffer_);
    return false;
  }
  /* Interval time between keep-alive probes while the connection is idle:
   * 60 seconds */
  code = curl_easy_setopt(conn_, CURLOPT_TCP_KEEPIDLE, 120L);
  if(code != CURLE_OK) {
    *err_msg = "Unable to set TCP keep-alive idle time for cURL connection: ";
    err_msg->append(error_buffer_);
    return false;
  }
  /* Interval time between keep-alive probes: 60 seconds */
  code = curl_easy_setopt(conn_, CURLOPT_TCP_KEEPINTVL, 60L);
  if(code != CURLE_OK) {
    *err_msg = "Unable to set TCP keep-alive probe interval time for cURL "
               "connection: ";
    err_msg->append(error_buffer_);
    return false;
  }

  code = curl_easy_setopt(conn_, CURLOPT_CONNECT_ONLY, 1L);
  if(code != CURLE_OK) {
    *err_msg = "Unable to set CONNECT_ONLY flag cURL connection.";
    return false;
  }

  /* Now test the connection. */
  code = curl_easy_perform(conn_);
  if(code != CURLE_OK) {
    *err_msg = "Unable to establish connection with the nmeta2 controller: ";
    err_msg->append(curl_easy_strerror(code));
    return false;
  }

  return true;
}
