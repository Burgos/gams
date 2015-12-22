/**
 * Copyright (c) 2015 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following acknowledgments and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The names "Carnegie Mellon University," "SEI" and/or "Software
 *    Engineering Institute" shall not be used to endorse or promote products
 *    derived from this software without prior written permission. For written
 *    permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *    appear in their names without prior written permission of
 *    permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 * 
 *      This material is based upon work funded and supported by the Department
 *      of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *      University for the operation of the Software Engineering Institute, a
 *      federally funded research and development center. Any opinions,
 *      findings and conclusions or recommendations expressed in this material
 *      are those of the author(s) and do not necessarily reflect the views of
 *      the United States Department of Defense.
 * 
 *      NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *      INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 *      UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *      IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *      FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *      OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *      NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *      TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 *      This material has been approved for public release and unlimited
 *      distribution.
 **/

/**
 * @file UTMFrame.cpp
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the UTM reference frame class
 **/

#ifdef GAMS_UTM

#include "UTMFrame.h"
#include "GPSFrame.h"

#include <GeographicLib/UTMUPS.hpp>

using namespace gams::utility;

using namespace GeographicLib;

const double UTMFrame::ZONE_WIDTH = 1000000;
const double UTMFrame::SOUTH_OFFSET = 10000000;
const double UTMFrame::MAX_Y = 9600000;
const double UTMFrame::MIN_Y = -9600000;

void UTMFrame::transform_location_to_origin(
                  double &x, double &y, double &z) const
{
  GAMS_WITH_FRAME_TYPE(origin(), GPSFrame, frame)
  {
    UTMUPS::Reverse(to_zone(x), to_hemi(y),
                    to_easting(x), to_northing(y), y, x);
    return;
  }
  throw undefined_transform(*this, origin().frame(), true);
}

void UTMFrame::transform_location_from_origin(
                  double &x, double &y, double &z) const
{
  GAMS_WITH_FRAME_TYPE(origin(), GPSFrame, frame)
  {
    int zone;
    bool hemi;
    double eas, nor;
    UTMUPS::Forward(y, x, zone, hemi, eas, nor, zone_);
    x = from_easting(eas, zone);
    y = from_northing(nor, hemi);
    return;
  }
  throw undefined_transform(*this, origin().frame(), true);
}

double UTMFrame::calc_distance(
                  double x1, double y1, double z1,
                  double x2, double y2, double z2) const
{
  if(to_zone(x1) == to_zone(x2))
  {
    double x_dist = x2 - x1;
    double y_dist = y2 - y1;
    double z_dist = z2 - z2;

    return sqrt(x_dist * x_dist + y_dist * y_dist + z_dist * z_dist);
  }
  GAMS_WITH_FRAME_TYPE(origin(), GPSFrame, frame)
  {
    Location l1(*this, x1, y1, z1);
    Location l2(*this, x2, y2, z2);

    l1.transform_this_to(*frame);
    l2.transform_this_to(*frame);

    return l1.distance_to(l2);
  }
}

void UTMFrame::do_normalize_location(
                  double &x, double &y, double &z) const
{ }

#endif
