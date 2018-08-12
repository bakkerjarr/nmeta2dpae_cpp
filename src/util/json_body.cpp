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

#include "../ext/rapidjson/error/en.h"
#include "../ext/rapidjson/stringbuffer.h"

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
 * At this point, parameters are pass-by-reference to reduce the amount of
 * object data that is copied around.
 * 
 * @param validator RapidJSON SchemaValidator for validating the doc against.
 * @param doc A decoded JSON document.
 * @param err_msg Contains are error message if this function returns false.
 * @return true if the JSON was successfully validated, false otherwise.
 */
bool jsonBodyValidate(rapidjson::SchemaValidator* validator,
                      rapidjson::Document* doc, string* err_msg) {
  if (!doc->Accept(*validator)) {
    /* The JSON document doc does not match the schema in validator. */
    if (!strcmp(validator->GetInvalidSchemaKeyword(), "additionalProperties")) {
      rapidjson::StringBuffer sb;
      validator->GetInvalidDocumentPointer().StringifyUriFragment(sb);
      *err_msg = "JSON document has an unexpected key: ";
      err_msg->append(sb.GetString());
    } else if (!strcmp(validator->GetInvalidSchemaKeyword(), "required")) {
      *err_msg = "JSON document is missing a required key.";
    } else if (!strcmp(validator->GetInvalidSchemaKeyword(), "type")) {
      *err_msg = "JSON document has a value of an incorrect type.";
    } else {
      *err_msg = "JSON document does not match the schema (unknown error).";
    }
    return false;
  }
  return true;
}
