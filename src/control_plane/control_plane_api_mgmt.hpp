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

#ifndef CONTROL_PLANE_CONTROL_PLANE_API_MGMT_
#define CONTROL_PLANE_CONTROL_PLANE_API_MGMT_

#include <string>

#include <curl/curl.h>

/**
 * A class that wraps up API requests made to the controller for the handshake
 * and keep-alive protocols used to connect to the controller.
 */
class CntrPlaneApiMgmt {
  public:
    CntrPlaneApiMgmt(std::string api_url);
    ~CntrPlaneApiMgmt();
    bool initConnection(std::string* err_msg);

  private:
    std::string api_url_;
    CURL* conn_;
    char error_buffer_[CURL_ERROR_SIZE];
};

#endif // CONTROL_PLANE_CONTROL_PLANE_API_MGMT_