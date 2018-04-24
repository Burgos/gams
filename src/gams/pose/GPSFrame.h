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
 * @file GPSFrame.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the base reference Frame class
 **/

#ifndef _GAMS_POSE_GPS_FRAME_H_
#define _GAMS_POSE_GPS_FRAME_H_

#include "ReferenceFrame.h"

namespace gams
{
  namespace pose
  {
#if 0
    /**
     * Positions represented as GPS coordinates, and an altitude,
     * assuming a spherical planet (by default, Earth):
     *    x is Latitude
     *    y is Longitude
     *    z is Altitude (above assumed perfectly spherical surface)
     * Orientations represented in Axis Angle notation
     *    Axis rx points towards north pole
     *    Axis ry points west at current linear
     *    Axis rz points upwards (i.e, normal vector)
     *
     * Note that under this scheme, change in x and/or y linear, while
     *   maintaining the same angular angles, implies a angular relative
     *   to the planet.
     *
     * Distances at same altitude calculated as distance along great circle of
     * sphere of radius planet_radius plus altitude. If altitude differs,
     * great circle distance first calculated using lower of two altitudes,
     * then distance calculated as follows:
     *    distance = sqrt(circle_distance ^ 2 + altitude_difference ^ 2)
     *
     * This frame can have cartesian frames embedded within it, but cannot
     * be embedded within any other frames at this time.
     **/
    class GAMSExport GPSFrame : public SimpleRotateFrame
    {
    public:
      static const double EARTH_RADIUS; /// Mean radius of the Earth
      static const double MOON_RADIUS; /// Mean radius of the Moon
      static const double MARS_RADIUS; /// Mean radius of Mars

      /**
       * Construct the GPSFrame, by default, for Earth
       *
       * @param planet_radius the radius of the planet this frame will
       *    represent. By default, EARTH_RADIUS.
       **/
      GPSFrame(double planet_radius = EARTH_RADIUS);

      /**
       * Construct the GPSFrame, by default, for Earth, embedded within
       * another frame. Note: does not support embedding within other frames.
       *
       * @param origin the origin of this frame within another frame
       * @param planet_radius the radius of the planet this frame will
       *    represent. By default, EARTH_RADIUS.
       **/
      explicit GPSFrame(
            const Pose &origin, double planet_radius = EARTH_RADIUS);

      /**
       * Construct the GPSFrame, by default, for Earth, embedded within
       * another frame, bound to a Pose object.
       *
       * @param origin the Pose object to bind to as origin of this frame
       * @param planet_radius the radius of the planet this frame will
       *    represent. By default, EARTH_RADIUS.
       **/
      explicit GPSFrame(Pose *origin, double planet_radius = EARTH_RADIUS);

      /**
       * Get the radius of the planet this frame represents
       *
       * @return radius in meters
       **/
      double radius() const;

      /**
       * Get the circumference of the planet this frame represents
       *
       * @return new circumference in meters
       **/
      double circ() const;

      /**
       * Set the radius of the planet this frame represents
       *
       * @param new_radius the new radius, in meters
       * @return radius in meters
       **/
      double radius(double new_radius);

      /**
       * Set the circumference of the planet this frame represents
       *
       * @param new_circ the new circumference, in meters
       * @return new circumference in meters
       **/
      double circ(double new_circ);

    protected:
      /**
       * Returns the name of this type of reference frame.
       *
       * @return the string "GPS"
       **/
      virtual std::string get_name() const;

      /**
      * Transforms a linear to origin
      * @param x   the x coordinate
      * @param y   the y coordinate
      * @param z   the z coordinate
      **/
      virtual void transform_linear_to_origin(
                      double &x, double &y, double &z) const;

      /**
      * Transforms a linear from origin
      * @param x   the x coordinate
      * @param y   the y coordinate
      * @param z   the z coordinate
      **/
      virtual void transform_linear_from_origin(
                      double &x, double &y, double &z) const;

      /**
      * Calculates distance from one point to another
      * @param x1   the x coordinate of the first point
      * @param y1   the y coordinate of the first point
      * @param z1   the z coordinate of the first point
      * @param x2   the x coordinate of the second point
      * @param y2   the y coordinate of the second point
      * @param z2   the z coordinate of the second point
      **/
      virtual double calc_distance(
                      double x1, double y1, double z1,
                      double x2, double y2, double z2) const;

      /**
      * Normalizes a linear
      * @param x   the x coordinate
      * @param y   the y coordinate
      * @param z   the z coordinate
      **/
      virtual void do_normalize_linear(
                      double &x, double &y, double &z) const;

    private:
      double planet_radius_;

#ifdef GAMS_NO_RTTI
    public:
      static type_ids::Flags get_stypes() {
        using namespace type_ids;
        return flags(GPS, SimpleRotate);
      }

      type_ids::Flags get_types() const override { return get_stypes(); }
#endif
    };
#endif

    constexpr double EARTH_RADIUS = 6371000.0;
    constexpr double EARTH_CIRC = EARTH_RADIUS * 2 * M_PI;

    extern const ReferenceFrameType *GPS;

    GAMSExport const ReferenceFrame &gps_frame (void);
  }
}

#include "GPSFrame.inl"
#endif

