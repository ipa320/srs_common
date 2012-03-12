/*
 * cpc_point.h
 *
 *  Created on: 26.08.2010
 *      Author: goa
 */
#include "srs_object_database/point_types.h"

#ifndef SRS_POINT_TYPES_HPP_
#define SRS_POINT_TYPES_HPP_

//using namespace pcl;

struct PointSURF
{
	PCL_ADD_POINT4D;
	union
	{
		struct
		{
		        uint32_t rgbr;
		        uint32_t rgbg;
		        uint32_t rgbb;
		        uint32_t id;
		        float confidence;
			float descriptor[64];
		};
		float data_c[4];
	};
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;

struct PointPLY
{
        PCL_ADD_POINT4D;
        union
        {
                struct
                {
                        uint32_t rgbr;
                        uint32_t rgbg;
                        uint32_t rgbb;
                };
                float data_c[4];
        };
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;
#endif /* SRS_POINT_TYPES_HPP_ */
