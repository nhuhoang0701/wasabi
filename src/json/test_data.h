#pragma once

#include <string>


static const std::string Json_test_reader0 =
	"{   "
	"    \"key\": {   "
	"        \"array\":   "
	"        [   "
	"            \"\",   "
	"            \"string\",   "
	"            -1.1,   "
	"            -1,   "
	"            true,   "
	"            false,   "
	"            null,   "
	"            {},   "
	"            [],   "
	"            [0]   "
	"        ],   "
	"        \"map0\":   "
	"        {   "
	"            \"\": \"\",   "
	"            \"string\": \"string\",   "
	"            \"0.0\": 0.0,   "
	"            \"0\": 0,   "
	"            \"true\": true,   "
	"            \"false\": false,   "
	"            \"null\": null,   "
	"            \"{}\":{},   "
	"            \"[]\":[],   "
	"            \"[0]\":[0]   "
	"        },   "
	"        \"map1\":   "
	"        {   "
	"        }   "
	"    }   "
	"}   ";

///Declare writer
extern  void Test_writer();
