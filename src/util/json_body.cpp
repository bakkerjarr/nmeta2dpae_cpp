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

/**
 * Provides functions for handling JSON documents from HTTP requests.
 */

#include "json_body.hpp"

using namespace std;

/**
 * Take a string-encoded JSON object and decode into objects that can be
 * utilised in C++.
 * 
 * @param req_body A JSON object encoded as a string.
 * @param doc The decoded JSON document.
 * @param err_msg Contains are error message if this function returns false.
 * @return true if JSON could be successfully decoded, false otherwise.
 */
bool jsonBodyDecode(string req_body, rapidjson::Document* doc,
                    string* err_msg) {
  /* Attempt to parse the document. */
  doc->Parse(req_body.c_str());
  /* Catch errors and form an error message. I've added one to the offset
   * provided by RapidJSON because the values it gives are off by one. */
  if (doc->HasParseError()) {
    *err_msg = "Error at character " +
                to_string((unsigned)doc->GetErrorOffset() + 1) + ": " +
                rapidjson::GetParseError_En(doc->GetParseError()) +
                " JSON doc: " + req_body;
    return false;
  }
  return true;
}

/**
 * Check that a JSON document contains the keys within the key_list. Return
 * false and record the missing keys.
 * 
 * @param key_list A list of JSON keys that must be within the JSON document.
 * @param doc A JSON document.
 * @param err_msg Contains are error message if this function returns false.
 * @return true if the JSON was successfully validated, false otherwise.
 */
bool jsonBodyValidate(vector<string> key_list, rapidjson::Document doc,
                      string* err_msg) {
  return false;
}