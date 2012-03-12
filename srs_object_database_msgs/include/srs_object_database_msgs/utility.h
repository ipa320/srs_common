#ifndef UTILITY_H_
#define UTILITY_H_

#include <string.h>
#include <ros/ros.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <algorithm>
#include <iterator>
#include <database_interface/postgresql_database.h>
#include <database_interface/db_class.h>
#include <pcl/ros/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/pcl_base.h>
#include <pcl/io/io.h>
#include <srs_object_database_msgs/surf.h>
#include <srs_object_database_msgs/pcl.h>
#include "srs_object_database/point_types.h"
#include "srs_object_database/impl/point_types.cpp"
#include <arm_navigation_msgs/Shape.h>

struct input {
  std::string id;
  std::string category;
  std::string name;
  std::string description;
  std::string image;
  std::string image_type;
  std::string grasp;
  std::string mesh;
  std::string mType;
  std::string pcl;
  std::string surf;
  std::string x;
  std::string y;
  std::string z;
  std::string basic_shape;
  std::string graspable;
  std::string urdf;
} ;

// FILE OPEN-READ-WRITE UTILITY
void utWriteToFile(char * data,std::string,long);
void utDataWrite(std::vector<uint8_t>,std::string);
char* utConvertToChar(std::vector<char>);
std::vector<char> utConvertToVector(char* inputData);

char * utRetrieveDataFile(std::string,  int *);
input utGetInputParameter(std::string);
// Data Loading utility
void utLoadFeaturePointFromFile(std::string,srs_object_database_msgs::surf::Ptr &);

void utLoadCloudPointFromPLYFile(std::string);

arm_navigation_msgs::Shape utLoadMeshFromFile(std::string , srs_object_database_msgs::pcl::Ptr &);

// utility

std::string fromCharPt(char *);

// DATABASE UTILITY
database_interface::PostgresqlDatabase utLoadSettings(std::string ,std::string ,std::string ,std::string ,std::string );


#endif /* UTILITY_H_ */
