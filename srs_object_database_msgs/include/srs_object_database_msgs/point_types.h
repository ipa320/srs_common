/*
 * cpc_point.h
 *
 *  Created on: 26.08.2010
 *      Author: goa
 */

#ifndef SRS_POINT_TYPES_H_
#define SRS_POINT_TYPES_H_

#include <pcl/ros/register_point_struct.h>
#include <pcl/point_types.h>
//#include <ros/ros.h>

using namespace pcl;

struct PointSURF;


#include "srs_object_database_msgs/impl/point_types.cpp"

//Point obtained from Swissranger devices, confidence only used for SR2
POINT_CLOUD_REGISTER_POINT_STRUCT(
	PointSURF,
  (float, x, x)
  (float, y, y)
  (float, z, z)
  (uint32_t, rgbr, rgbr)
  (uint32_t, rgbg, rgbg)
  (uint32_t, rgbb, rgbb)
  (uint32_t , id, id)
  (float , confidence,confidence)
  (float[64], descriptor, descriptor));

POINT_CLOUD_REGISTER_POINT_STRUCT(
        PointPLY,
  (float, x, x)
  (float, y, y)
  (float, z, z)
  (uint32_t, rgbr, rgbr)
  (uint32_t, rgbg, rgbg)
  (uint32_t, rgbb, rgbb));

#endif /* SRS_POINT_TYPES_H_ */
