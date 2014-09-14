/*
 *  geohash.h libgeohash
 *  Created by Derek Smith on 10/6/09.
 *  Copyright (c) 2010, SimpleGeo
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  Redistributions of source code must retain the above copyright notice, this list
 *  of conditions and the following disclaimer. Redistributions in binary form must
 *  reproduce the above copyright notice, this list of conditions and the following
 *  disclaimer in the documentation and/or other materials provided with the distribution.
 *  Neither the name of the SimpleGeo nor the names of its contributors may be used
 *  to endorse or promote products derived from this software without specific prior
 *  written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 *  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef  __GEOHASH_H_
#define  __GEOHASH_H_

#include <string>
#include <vector>

namespace geohash {

// https://github.com/simplegeo/libgeohash
// Metric in meters
struct GeoBoxDimension {
  double height;
  double width;
};

struct GeoCoord {
  double latitude;
  double longitude;
  double north;
  double east;
  double south;
  double west;
  GeoBoxDimension dimension;
};

class GeohashTool {
 public:
  // Creates a the hash at the specified precision. If precision is set to 0.
  // or less than it defaults to 12.
  static std::string GeohashEncode(double lat, double lng, int precision);

  // Returns the latitude and longitude used to create the hash along with
  // the bounding box for the encoded coordinate.
  static GeoCoord GeohashDecode(const std::string & hash);

  // Return an array of geohashes that represent the neighbors of the passed
  // in value. The neighbors are indexed as followed:
  // N, NE, E, SE, S, SW, W, NW
  // 0, 1,  2,  3, 4,  5, 6, 7
  static void GeohashNeighbors(const std::string & hash,
                               std::vector<std::string> * rs);

  // Returns the width and height of a precision value.
  static GeoBoxDimension GeohashDimensionsForPrecision(int precision);
};

}  // namespace geohash
#endif  // __GEOHASH_H_
