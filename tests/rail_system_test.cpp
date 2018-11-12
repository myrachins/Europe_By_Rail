/*
 * File:   RailSystemTest.cpp
 * Author: anton
 *
 * Created on May 16, 2015, 2:12:19 PM
 * 
 * Edited: Pavel Pertsukhov, 2018
 */
 
#include <climits>
#include "gtest/gtest.h"
#include "rail_system.h"

std::string servicesPath = "../../res/services.txt";



TEST(RailSystemTest, testRomeWarsaw) {
    RailSystem rs(servicesPath);
    Route actual = rs.getCheapestRoute("Rome", "Warsaw");
    EXPECT_EQ(actual.fee, 175);
    EXPECT_EQ(actual.distance, 5200);
    // hope there is only one route with this fee
    EXPECT_EQ(actual.cities, vector<string>({"Rome", "Bern", "Sarajevo", "Budapest", "Bucharest", "Warsaw"}));
}

TEST(RailSystemTest, testWarsawParis) {
    RailSystem rs(servicesPath);
    Route actual = rs.getCheapestRoute("Warsaw", "Paris");
    EXPECT_EQ(actual.fee, 105);
    EXPECT_EQ(actual.distance, 4900);
    EXPECT_EQ(actual.cities, vector<string>({"Warsaw", "Bucharest", "Budapest", "Sarajevo", "Bern", "Paris"}));
}

TEST(RailSystemTest, testLisbonAthens) {
    RailSystem rs(servicesPath);
    Route actual = rs.getCheapestRoute("Lisbon", "Athens");
    EXPECT_EQ(actual.fee, 200);
    EXPECT_EQ(actual.distance, 4450);
    EXPECT_EQ(actual.cities, vector<string>({"Lisbon", "Madrid", "Bern", "Sarajevo", "Skopja", "Tirane", "Athens"}));
}

TEST(RailSystemTest, testNoRoute) {
    RailSystem rs(servicesPath);
    Route actual = rs.getCheapestRoute("Paris", "Belfast");
    EXPECT_EQ(actual.fee, INT_MAX);
    EXPECT_EQ(actual.distance, INT_MAX);
    EXPECT_EQ(actual.cities, vector<string>());

}

TEST(RailSystemTest, testInvalidCity) {
    RailSystem rs(servicesPath);
    EXPECT_THROW(rs.getCheapestRoute("Unknown", "Rome"), std::invalid_argument);
    EXPECT_THROW(rs.getCheapestRoute("Rome", "Unknown"), std::invalid_argument);
    EXPECT_THROW(rs.getCheapestRoute("Unknown", "Unknown"), std::invalid_argument);
}

TEST(RailSystemTest, testIvalidFile) {
    EXPECT_THROW(RailSystem rs("unknown_file");, std::logic_error);
}

