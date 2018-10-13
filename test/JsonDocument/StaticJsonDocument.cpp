// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("StaticJsonDocument") {
  StaticJsonDocument<200> doc;

  SECTION("serializeJson()") {
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = "world";

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"hello\":\"world\"}");
  }

  SECTION("Copy assignment") {
    StaticJsonDocument<200> doc2;
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    doc = doc2;

    std::string json;
    serializeJson(doc, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc.nestingLimit == 42);
  }

  SECTION("Copy constructor") {
    deserializeJson(doc, "{\"hello\":\"world\"}");
    doc.nestingLimit = 42;

    StaticJsonDocument<200> doc2 = doc;

    std::string json;
    serializeJson(doc2, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc2.nestingLimit == 42);
  }
}
