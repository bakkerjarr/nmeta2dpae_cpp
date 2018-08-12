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
#include<iostream>
#include "../../src/util/json_body.hpp"

using namespace std;

/* Resources */
/* Odd numbered request bodies are legal JSON, even numbered ones are not. */
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
const string req_body_8 = "{\"hostname_dpae\": \"example.com\","
                          "\"uuid_dpae\":\"this one is missing keys\"}";
const string req_body_10 = "{\"hostname_dpae\": \"example.com\","
                           "\"extra_key_1\": \"4\","
                           "\"uuid_dpae\":\"1234567890\","
                           "\"uuid_controller\":\"6547891023\","
                           "\"if_name\": \"something\","
                           "\"extra_key_2\": \"mooooore data!\"}";
const string req_body_12 = "{\"hostname_dpae\": \"example.com\","
                           "\"uuid_dpae\":\"this one is missing a key\","
                           "\"extra_key\":\"and also has an extra key\"}";
const string req_body_14 = "{\"hostname_dpae\": \"localhost\","
                           "\"uuid_dpae\": 1234567890,"
                           "\"uuid_controller\":\"9876543210\","
                           "\"if_name\":\"eth0\"}";

const char* nmeta2_json_schema = \
"{"
	"\"$schema\": \"http://json-schema.org/draft-04/schema#\","
	"\"title\": \"DPAE-Controller_Connection\","
	"\"description\": \"Information for the DPAE to establish a session with the controller.\","
	"\"type\": \"object\","
	"\"properties\": {"
		"\"hostname_dpae\": {"
			"\"description\": \"The hostname of the machine that this DPAE will execute on.\","
			"\"type\": \"string\""
		"},"
		"\"uuid_dpae\": {"
			"\"description\": \"UUID of this DPAE instance.\","
			"\"type\": \"string\""
		"},"
		"\"uuid_controller\": {"
			"\"description\": \"UUID of the controller we are connecting to.\","
			"\"type\": \"string\""
		"},"
		"\"if_name\": {"
			"\"description\": \"The name of the network interface that will be used to sniff traffic from.\","
			"\"type\": \"string\""
		"}"
	"},"
	"\"required\": [\"hostname_dpae\", \"uuid_dpae\", \"uuid_controller\", \"if_name\"],"
	"\"additionalProperties\": false"
"}";


/* Test cases: Decoding JSON documents */

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

TEST(jsonBodyDecode, nmeta2_json_payload_pass) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_TRUE(jsonBodyDecode(req_body_5, &doc, &err_msg));
}

TEST(jsonBodyDecode, nmeta2_json_payload_fail) {
  rapidjson::Document doc;
  string err_msg;
  ASSERT_FALSE(jsonBodyDecode(req_body_6, &doc, &err_msg));
}

/* Test cases: Validating the keys of decoded JSON documents */

/**
 * Successfully validate a JSON document used by the nmeta2 system.
 */
TEST(jsonBodyValidate, nmeta2_json_key_check_pass) {
  /* Prepare the function arguments */
  rapidjson::Document doc;
  doc.Parse(req_body_5.c_str());
  string err_msg;
  /* Parse the schema  */
  rapidjson::Document schema_json_doc;
  if (schema_json_doc.Parse(nmeta2_json_schema).HasParseError()) {
    string err_msg = "FATAL ERROR: unable to parse JSON schema: ";
    err_msg.append(nmeta2_json_schema);
    FAIL() << err_msg;
  }
  rapidjson::SchemaDocument schema(schema_json_doc);
  rapidjson::SchemaValidator validator(schema);
  /* Test the jsonBodyValidate function */
  ASSERT_TRUE(jsonBodyValidate(&validator, &doc, &err_msg));
}

/**
 * Fail to validate a JSON document because the doc is missing keys expected by
 * the nmeta2 system.
 */
TEST(jsonBodyValidate, nmeta2_json_missing_keys_fail) {
  /* Prepare the function arguments */
  rapidjson::Document doc;
  doc.Parse(req_body_8.c_str());
  string err_msg;
  /* Parse the schema  */
  rapidjson::Document schema_json_doc;
  if (schema_json_doc.Parse(nmeta2_json_schema).HasParseError()) {
    string err_msg = "FATAL ERROR: unable to parse JSON schema: ";
    err_msg.append(nmeta2_json_schema);
    FAIL() << err_msg;
  }
  rapidjson::SchemaDocument schema(schema_json_doc);
  rapidjson::SchemaValidator validator(schema);
  /* Test the jsonBodyValidate function */
  ASSERT_FALSE(jsonBodyValidate(&validator, &doc, &err_msg));
}

/**
 * Fail to validate a JSON document because the doc is has extra keys that are
 * not expected by the nmeta2 system.
 */
TEST(jsonBodyValidate, nmeta2_json_extra_keys_fail) {
  /* Prepare the function arguments */
  rapidjson::Document doc;
  doc.Parse(req_body_10.c_str());
  string err_msg;
  /* Parse the schema  */
  rapidjson::Document schema_json_doc;
  if (schema_json_doc.Parse(nmeta2_json_schema).HasParseError()) {
    string err_msg = "FATAL ERROR: unable to parse JSON schema: ";
    err_msg.append(nmeta2_json_schema);
    FAIL() << err_msg;
  }
  rapidjson::SchemaDocument schema(schema_json_doc);
  rapidjson::SchemaValidator validator(schema);
  /* Test the jsonBodyValidate function */
  ASSERT_FALSE(jsonBodyValidate(&validator, &doc, &err_msg));
}

/**
 * Fail to validate a JSON document because the doc is missing a key expected
 * by the nmeta2 system but also has a key that is not expected by the nmeta2
 * system.
 */
TEST(jsonBodyValidate, nmeta2_json_missing_and_extra_keys_fail) {
  /* Prepare the function arguments */
  rapidjson::Document doc;
  doc.Parse(req_body_12.c_str());
  string err_msg;
  /* Parse the schema  */
  rapidjson::Document schema_json_doc;
  if (schema_json_doc.Parse(nmeta2_json_schema).HasParseError()) {
    string err_msg = "FATAL ERROR: unable to parse JSON schema: ";
    err_msg.append(nmeta2_json_schema);
    FAIL() << err_msg;
  }
  rapidjson::SchemaDocument schema(schema_json_doc);
  rapidjson::SchemaValidator validator(schema);
  /* Test the jsonBodyValidate function */
  ASSERT_FALSE(jsonBodyValidate(&validator, &doc, &err_msg));
  cout << err_msg <<endl;
}

/**
 * Fail to validate a JSON document because a data type in the doc that is
 * expected by the nmeta2 system is incorrect.
 */
TEST(jsonBodyValidate, nmeta2_json_incorrect_type_fail) {
  /* Prepare the function arguments */
  rapidjson::Document doc;
  doc.Parse(req_body_14.c_str());
  string err_msg;
  /* Parse the schema  */
  rapidjson::Document schema_json_doc;
  if (schema_json_doc.Parse(nmeta2_json_schema).HasParseError()) {
    string err_msg = "FATAL ERROR: unable to parse JSON schema: ";
    err_msg.append(nmeta2_json_schema);
    FAIL() << err_msg;
  }
  rapidjson::SchemaDocument schema(schema_json_doc);
  rapidjson::SchemaValidator validator(schema);
  /* Test the jsonBodyValidate function */
  ASSERT_FALSE(jsonBodyValidate(&validator, &doc, &err_msg));
  cout << err_msg <<endl;
}
