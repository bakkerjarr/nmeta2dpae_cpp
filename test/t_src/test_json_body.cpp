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

#include "gtest/gtest.h"

#include "../../src/util/json_body.hpp"

using namespace std;

/* Resources */

const string req_body_1 = "{\"attr_1\": 123,"
                          "\"attr_2\": \"qwerty\"}";
const string req_body_2 = "{\"attr_1\": 123,"
                          "\"attr_2\": wef}";
const string req_body_3 = "{\"attr_1\": [1,2,3],"
                          "\"attr_2\": {\"attr_3\": \"hello_world\"}}";
const string req_body_4 = "{\"attr_1\": [1,,3],"
                          "\"attr_2\": \"attr_3\": \"hello_world\"}}";
const string req_body_5 = "{\"hostname_dpae\": \"localhost\","
                          "\"uuid_dpae\":\"1234567890\","
                          "\"uuid_controller\":\"9876543210\","
                          "\"if_name\":\"eth0\"}";
const string req_body_6 = "{\"hostname_dpae\": \"example.com\","
                          "\"uuid_dpae\":\"1234567890\","
                          "\"uuid_controller\":\"6547891023\","
                          "\"if_name\":}";

/* Test cases */

TEST(jsonBodyDecode, simple_json_payload_pass) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_TRUE(jsonBodyDecode(req_body_1, &doc, &err_msg));
}

TEST(jsonBodyDecode, simple_json_payload_fail) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_FALSE(jsonBodyDecode(req_body_2, &doc, &err_msg));
}

TEST(jsonBodyDecode, nested_json_payload_pass) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_TRUE(jsonBodyDecode(req_body_3, &doc, &err_msg));
}

TEST(jsonBodyDecode, nested_json_payload_fail) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_FALSE(jsonBodyDecode(req_body_4, &doc, &err_msg));
}

TEST(jsonBodyDecode, nmeta2_payload_pass) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_TRUE(jsonBodyDecode(req_body_5, &doc, &err_msg));
}

TEST(jsonBodyDecode, nmeta2_payload_fail) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_FALSE(jsonBodyDecode(req_body_6, &doc, &err_msg));
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

TEST(jsonBodyValidate, DISABLED_as) {
  ASSERT_EQ(1, 3);
  /* TODO: I need a JSON doc to pass through... */
  //jsonBodyValidate(vector<string> key_list, rapidjson::Document doc,
  //                    string* err_msg
}
