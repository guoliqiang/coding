//  Created by Derek Smith on 10/6/09.
//  Copyright 2009 SimpleGeo. All rights reserved.

#include <string.h>
#include <stdio.h>
#include "base/public/logging.h"
#include "third_part/geohash/public/geohash.h"

using namespace geohash;  // NOLINT

void check_coords(GeoCoord coord, GeoCoord expected) {
  const char* valueTitle = NULL;
  double expectedValue;
  double value;
  if (coord.latitude != expected.latitude) {
    valueTitle = "latitude";
    expectedValue = expected.latitude;
    value = coord.latitude;
  } else if (coord.longitude != expected.longitude) {
    valueTitle = "longitude";
    expectedValue = expected.longitude;
    value = coord.longitude;
  } else if (coord.north != expected.north) {
    valueTitle = "north";
    expectedValue = expected.north;
    value = coord.north;
  } else if (expected.south != coord.south) {
    valueTitle = "south";
    expectedValue = expected.south;
    value = coord.south;
  } else if (coord.east != expected.east) {
    valueTitle = "east";
    expectedValue = expected.east;
    value = coord.east;
  } else if (expected.west != coord.west) {
    valueTitle = "west";
    expectedValue = expected.west;
    value = coord.west;
  }
  if (valueTitle) {
    CHECK(false) << "Expected " << expectedValue << " but was"
                 << value << " for " << valueTitle;
  }
}

void checkHashes(const char* hash, const char* expected) {
  if (strcmp(hash, expected) != 0) {
    CHECK(false) << "Expected hash = " << expected
                 << ", but was " << hash;
  }
}

void checkNeighbors(const std::vector<std::string> & neighbors,
                    const char** expectedNeighbors) {
  for (int i = 0; i < 8; i++) {
    if (strcmp(neighbors[i].c_str(), expectedNeighbors[i]) != 0) {
      CHECK(false) << "Expected hash = " << expectedNeighbors[i]
                   << " at index = " << i << ", but was " << neighbors[i];
    }
  }
}

int main() {
  // Decoder
  GeoCoord coord = GeohashTool::GeohashDecode("ezs42");
  GeoCoord expectedCoord = {42.60498046875, -5.60302734375,
                            42.626953125, -5.5810546875,
                            42.5830078125, -5.625};
  check_coords(coord, expectedCoord);
  coord = GeohashTool::GeohashDecode("ezs42gx");
  expectedCoord = (GeoCoord) {42.602920532226562, -5.5817413330078125,
                              42.603607177734375, -5.581054687500000,
                              42.60223388671875, -5.582427978515625};
  check_coords(coord, expectedCoord);
  coord = GeohashTool::GeohashDecode("9xj5smj4w40");
  expectedCoord = (GeoCoord) {40.018140748143196, -105.27485780417919,
                              40.01814141869545, -105.27485713362694,
                              40.018140077590942, -105.27485847473145};
  check_coords(coord, expectedCoord);
  // Encoder
  std::string hash = GeohashTool::GeohashEncode(42.60498046875,
                                                -5.60302734375, 5);
  checkHashes(hash.c_str(), "ezs42");
  hash = GeohashTool::GeohashEncode(40.018141, -105.274858, 12);
  checkHashes(hash.c_str(), "9xj5smj4w40m");
  hash = GeohashTool::GeohashEncode(40.018141, -105.274858, 2);
  checkHashes(hash.c_str(), "9x");
  hash = GeohashTool::GeohashEncode(40.018141, -105.274858, 0);
  checkHashes(hash.c_str(), "9xj5sm");
  // Neighbors
  std::vector<std::string> neighbors;
  GeohashTool::GeohashNeighbors("ezs42", &neighbors);
  const char* expectedNeighbors[8] = {"ezs48", "ezs49", "ezs43", "ezs41",
                                      "ezs40", "ezefp", "ezefr", "ezefx"};
  checkNeighbors(neighbors, expectedNeighbors);
  expectedNeighbors[0] = "9xj5smj4w40q";
  expectedNeighbors[1] = "9xj5smj4w40w";
  expectedNeighbors[2] = "9xj5smj4w40t";
  expectedNeighbors[3] = "9xj5smj4w40s";
  expectedNeighbors[4] = "9xj5smj4w40k";
  expectedNeighbors[5] = "9xj5smj4w40h";
  expectedNeighbors[6] = "9xj5smj4w40j";
  expectedNeighbors[7] = "9xj5smj4w40n";
  GeohashTool::GeohashNeighbors("9xj5smj4w40m", &neighbors);
  checkNeighbors(neighbors, expectedNeighbors);
  return 0;
}
