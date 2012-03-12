/*
 * utility
 *
 *  Created on: Oct 24, 2011
 *      Author: srs-db-dev
 */
#include "srs_object_database/utility.h"
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
#include "srs_object_database/point_types.h"
#include "srs_object_database/impl/point_types.cpp"
#include "srs_object_database_msgs/surf.h"
#include <geometry_msgs/Pose.h>

using namespace std;



void utWriteToFile(char * data, std::string path, long size)
{

  ofstream outfile(path.c_str(), ofstream::binary | ofstream::out);
  // write to outfile
  outfile.write(data, size);
  outfile.close();
  //ROS_INFO("Finish writing file \n");
}

void utDataWrite(vector<uint8_t> data, string path)
{
  // write a generic data varible to file as specified in path input variable
  //ROS_INFO("Writing data");
  if (data.size() <= 0)
  {
    ROS_INFO("Warning data supplied by service is not valid!");
    return;
  }
  size_t first_obj_size;
  vector<uint8_t> c;
  c.operator =(data);
  first_obj_size = c.size() * sizeof(char);
  //ROS_INFO("Object Data %i \n", first_obj_size);
  std::stringstream mod_id;
  uint8_t dataArray[first_obj_size];
  //ROS_INFO("Loading...\n");
  for (size_t t = 0; t < first_obj_size; t++)
  {
    dataArray[t] = (uint8_t)data.at(t);
  }
  char * buffer;
  size_t size;
  size = first_obj_size;
  buffer = new char[first_obj_size];
  //ROS_INFO("Writing data ... \n");
  for (uint i = 0; i < first_obj_size; i++)
  {
    buffer[i] = (char)dataArray[i];
  }
  utWriteToFile(buffer, path, size);
  delete[] buffer;
}

char * utRetrieveDataFile(std::string path, int * siz)
{

  int size;
  //ROS_INFO("Image processing for %s \n", path.c_str());
  ifstream file(path.c_str(), ios::in | ios::binary | ios::ate);
  //ROS_INFO("Image processing begin \n");
  char * memblock;
  if (file.is_open())
  {
    //ROS_INFO(" Processing File \n");
    file.seekg(0, ios::end);
    size = file.tellg();
    file.seekg(0, ios::beg);
    //ROS_INFO("File data size %i", size);
    memblock = new char[size];
    *siz = size;
    file.read(memblock, size);
    //ROS_INFO("Total block read %i \n", size);
    file.close();
    return memblock;
  } else {
    ROS_INFO("Could not retrieve file");
  }

  return NULL;

}

std::string fromCharPt(char * ch){
  if(ch == NULL){
    return "";
  } else {
    string tmp (ch);
    return tmp;
  }
}

input utGetInputParameter(std::string path){
  ROS_INFO("Data from file %s",path.c_str());
  string line = "";

  input input;
  char* pch;

  ifstream source(path.c_str());
  if (source.is_open())
  {
    ROS_INFO("Loading input for test client");
    char * tokenString;
    string str;
    while (getline(source, line))
    {
      pch = (char*)malloc(sizeof(char) * (line.length() + 1));
      strcpy(pch, line.c_str());
      tokenString = strtok(pch, " ");
      std::string param=tokenString;
      string tmp;
      if (param=="id"){
        tokenString = strtok(NULL, " ");
        ROS_INFO("Line id ");
        input.id=fromCharPt(tokenString);

      }
      if (param=="grasp"){
        ROS_INFO("Line grasp ");
        tokenString = strtok(NULL, " ");
        input.grasp=fromCharPt(tokenString);
      }
      if (param=="mesh"){
        ROS_INFO("Line Mesh ");
        tokenString = strtok(NULL, " ");
        input.mesh=fromCharPt(tokenString);
      }
      if (param=="image"){
        ROS_INFO("Line Image ");
        tokenString = strtok(NULL, " ");
        input.image=fromCharPt(tokenString);
      }
      if (param=="image-type"){
        ROS_INFO("Line Image Type ");
        tokenString = strtok(NULL, " ");
        input.image_type=fromCharPt(tokenString);
      }
      if (param=="pcl"){
        ROS_INFO("Line pcl ");
        tokenString = strtok(NULL, " ");
        input.pcl=fromCharPt(tokenString);
      }
      if (param=="surf"){
        ROS_INFO("Line Surf ");
        tokenString = strtok(NULL, " ");
        input.surf=fromCharPt(tokenString);

      }
      if (param=="typeMesh"){
        ROS_INFO("Line TypeMesh  ");
        tokenString = strtok(NULL, " ");
        input.mType=fromCharPt(tokenString);
      }
      if (param=="name"){
        ROS_INFO("Line Name ");
        tokenString = strtok(NULL, " ");
        input.name=fromCharPt(tokenString);
            //line.substr(param.size()+1,(line.length()-param.size())-1);
      }
      if (param=="category"){
        ROS_INFO("Line category ");
        tokenString = strtok(NULL, " ");
        input.category=fromCharPt(tokenString);
        //input.category=line.substr(param.size()+1,(line.length()-param.size())-1);
      }
      if (param=="description"){
        ROS_INFO("Line Description ");
        tokenString = strtok(NULL, " ");
        input.description=fromCharPt(tokenString);
            //line.substr(param.size()+1,(line.length()-param.size())-1);
      }
      if (param=="x-size"){
        ROS_INFO("Line x ");
        tokenString = strtok(NULL, " ");
        input.x=line.substr(param.size()+1,(line.length()-param.size())-1);
        ROS_INFO("Line x size %f",atof(input.x.c_str()));
      }
      if (param=="y-size"){
        ROS_INFO("Line y size ");
        tokenString = strtok(NULL, " ");
        input.y=fromCharPt(tokenString);
            //line.substr(param.size()+1,(line.length()-param.size())-1);
        ROS_INFO("Line y size %f",atof(input.y.c_str()));
      }
      if (param=="z-size"){
        ROS_INFO("Line z size ");
        tokenString = strtok(NULL, " ");
        input.z=fromCharPt(tokenString);
        //line.substr(param.size()+1,(line.length()-param.size())-1);
        ROS_INFO("Line z size %f",atof(input.z.c_str()));
      }
      if (param=="basic-shape"){
        ROS_INFO("Line Basic Shape ");
        tokenString = strtok(NULL, " ");
        input.basic_shape=fromCharPt(tokenString);
        //line.substr(param.size()+1,(line.length()-param.size())-1);
      }
      if (param=="end"){
        ROS_INFO("data loaded ");
        return input;
      }
      if (param=="graspable"){
        ROS_INFO("graspable loaded ");
        tokenString = strtok(NULL, " ");
        input.graspable=fromCharPt(tokenString);
        //line.substr(param.size()+1,(line.length()-param.size())-1);;
      }
      if (param=="urdf"){
        ROS_INFO("urdf loaded ");
        tokenString = strtok(NULL, " ");
        input.urdf=fromCharPt(tokenString);
        ROS_INFO("urdf %s ",input.urdf.c_str());
        //line.substr(param.size()+1,(line.length()-param.size())-1);;
      }
    }

    return input;
  }else{
    ROS_INFO("Cannot load Input File");
    return input;
  }

}

database_interface::PostgresqlDatabase utLoadSettings(std::string dbInstance, std::string dbPort, std::string dbUser,
                                                      std::string dbPwd, std::string dbName)
{

  database_interface::PostgresqlDatabase database(dbInstance, dbPort, dbUser, dbPwd, dbName);
  return database;

}

vector<char> utConvertToVector(char* inputData)
{
  int size = sizeof(inputData) / sizeof(char);
  vector<char> vChar;
  for (int i = 0; i < size; i++)
  {

    vChar.push_back(inputData[i]);
  }
  return vChar;
}

char* utConvertToChar(vector<char> inputData)
{
  int size = inputData.size();
  char* vChar = new char[size];
  for (int i = 0; i < size; i++)
  {
    vChar[i] = inputData.at(i);
  }
  return vChar;
}

void utLoadFeaturePointFromFile(std::string path,srs_object_database_msgs::surf::Ptr & surf)
{
  ROS_INFO("Data from file %s",path.c_str());
  pcl::PointCloud<PointSURF>::Ptr cloud(new pcl::PointCloud<PointSURF>);
  pcl::PointCloud<PointXYZ>::Ptr bBox(new pcl::PointCloud<PointXYZ>);
  pcl::PointCloud<PointSURF> pcl;
  PointSURF pSurf;
  pcl::PointCloud<PointXYZ> boundingBox;

  pcl.height = 0;
  pcl.width = 1;
  boundingBox.height=2;
  boundingBox.width=1;
  boundingBox.points.resize(2);
  // second point line
  uint32_t rRGB = 0;
  uint32_t gRGB = 0;
  uint32_t bRGB = 0;
  string line = "";
  long totalLine = 0;
  long totalPoints = 0;
  long numFeatures = 0;
  long points = 0;
  bool testLine = false;
  char* pch;
  // the first four lines represent the general settings of the points
  int offset = 4;

  ifstream source(path.c_str());
  if (source.is_open())
  {

    ROS_INFO("Loading file data to pcl Begin");
    char * tokenString;
    while (getline(source, line))
    {
      if (line.length() == 2)
      {
        string cut;
        //ROS_INFO("Testing lenght ");

        if (line.substr(0, 2) == "-1")
        {
          testLine = true;
          //ROS_INFO("found -1");
        }
        else
        {
          testLine = false;
          //ROS_INFO("found short line but not -1 instead %s",line.substr(0,2).c_str());
        }
      }
      else
      {
        testLine = false;
      }
      if (!(testLine))
      {
        totalLine++;
        //ROS_INFO("Points %i",totalPoints);
        if (totalLine > offset)
        {
          // New Point processing after the first settings lines
          if (points + 1 <= numFeatures)
          {
            totalPoints++;
            int mod;
            mod = (totalPoints) % 3;
            if (mod == 1)
            {
              // this line contains id and confidence
              // first token is id, while second is confidence
              pch = (char*)malloc(sizeof(char) * (line.length() + 1));
              strcpy(pch, line.c_str());
              tokenString = strtok(pch, " ");
              pSurf.id=atol(tokenString);
              //pcl.points[points+1].id = atol(tokenString);
              tokenString = strtok(NULL, " ");
              pSurf.confidence=atof(tokenString);
              //pcl.points[points+1].confidence = atof(tokenString);

            }
            if (mod == 2)
            {
              pch = (char*)malloc(sizeof(char) * (line.length() + 1));
              strcpy(pch, line.c_str());

              tokenString = strtok(pch, " ");
              pSurf.x=atof(tokenString);
              //pcl.points[points+1].x = atof(tokenString);
              tokenString = strtok(NULL, " ");
              pSurf.y=atof(tokenString);
              //pcl.points[points+1].y = atof(tokenString);
              tokenString = strtok(NULL, " ");
              pSurf.z=atof(tokenString);
              pSurf.rgbr=rRGB;
              pSurf.rgbg=gRGB;
              pSurf.rgbb=bRGB;
            }
            if (mod == 0)
            {
              pch = (char*)malloc(sizeof(char) * (line.length() + 1));
              strcpy(pch, line.c_str());
              tokenString = strtok(pch, " ");
              int i = 0;
              pSurf.descriptor[0]=atof(tokenString);
              tokenString = strtok(NULL, " ");
              while ((i < 63) & (tokenString != NULL))
              {
                pSurf.descriptor[i+1]=atof(tokenString);
                tokenString = strtok(NULL, " ");
                i++;
              }
              points++;
              pcl.points.push_back(pSurf);
            }
          }else{

            pch = (char*)malloc(sizeof(char) * (line.length() + 1));
            strcpy(pch, line.c_str());
            float temp[6];
            tokenString = strtok(pch, " ");
            int i = 1;
            temp[0]=atof(tokenString);
            tokenString = strtok(NULL, " ");
            // this line contains descriptions
            while ((tokenString != NULL))
            {
              temp[i]=atof(tokenString);
              tokenString = strtok(NULL, " ");
              i++;
            }
            if(points + 1 == numFeatures+1){

              // this is the first line after the points -> bounding box
              boundingBox.points[0].x=temp[0];
              boundingBox.points[0].y=temp[1];
              boundingBox.points[0].z=temp[2];
              boundingBox.points[1].x=temp[3];
              boundingBox.points[1].y=temp[4];
              boundingBox.points[1].z=temp[5];

            } else if (points + 1 == numFeatures+2){
              // this is the second line after the points -> coord frame
              surf->coord_frame.position.x=temp[0];
              surf->coord_frame.position.y=temp[1];
              surf->coord_frame.position.z=temp[2];
              surf->coord_frame.orientation.x=temp[3];
              surf->coord_frame.orientation.y=temp[4];
              surf->coord_frame.orientation.z=temp[5];
            }else {
              ROS_INFO("Wrong information in the viewpoint");
            }
            points++;
          }

        }
        else
        {
          // We are processing generic information for all points
          if (totalLine == 1)
          {
            // first four lines contains general information about PCL
            // tokenize the string using the separator
            pch = (char*)malloc(sizeof(char) * (line.length() + 1));
            strcpy(pch, line.c_str());
            tokenString = strtok(pch, " ");
            // first line contains number of features
            numFeatures = atol(tokenString);
            //ROS_INFO("Total feature in file... %ld", numFeatures);
            pcl.height = numFeatures;
            ROS_INFO("Declared features in file %i",numFeatures);
            //pcl.points.resize(numFeatures);

          }
          if (totalLine == 2)
          {
            // first line is red
            pch = (char*)malloc(sizeof(char) * (line.length() + 1));
            strcpy(pch, line.c_str());
            tokenString = strtok(pch, " ");
            rRGB = atoi(tokenString);
            //ROS_INFO("RGB red...%i", rRGB);
            // second line is green
            tokenString = strtok(NULL, " ");
            gRGB = atoi(tokenString);
            //ROS_INFO("RGB green...%i", gRGB);
            // third line is blue
            tokenString = strtok(NULL, " ");
            bRGB = atoi(tokenString);
            //ROS_INFO("RGB blue...%i", bRGB);

          }
          if (totalLine == 3)
          {
            // number of value in description should be always 64
            pch = (char*)malloc(sizeof(char) * (line.length() + 1));
            strcpy(pch, line.c_str());
            tokenString = strtok(pch, " ");
            //int description = atoi(tokenString);
            //ROS_INFO("Description...%i", description);
          }
          if (totalLine == 4)
          {
            // number of connection dimension
            pch = (char*)malloc(sizeof(char) * (line.length() + 1));
            strcpy(pch, line.c_str());
            tokenString = strtok(pch, " ");
            //int connection = atoi(tokenString);
            //ROS_INFO("Connection...%i", connection);
            //ROS_INFO("################################################################");
          }
        }
      }
    }
  }
  else
  {
    ROS_INFO("Cannot open file data to load FPt");
  }

  ROS_INFO("PCL loaded with: %i size", pcl.size());
  ROS_INFO("PCL loaded with: %i features", pcl.points.size());

  *cloud=pcl;
  *bBox=boundingBox;
  sensor_msgs::PointCloud2 fpt;
  sensor_msgs::PointCloud2 bounding_box;
  pcl::toROSMsg(*cloud,fpt);
  pcl::toROSMsg(*bBox,bounding_box);
  surf->surf=fpt;
  surf->bounding_box=bounding_box;
  ROS_INFO("Retrieval executed");

}

void utLoadCloudPointFromPLYFile(std::string path, srs_object_database_msgs::pcl::Ptr & pcl){
  ROS_INFO("Data from file %s",path.c_str());
  //srs_object_database_msgs::pcl::Ptr pcl(new srs_object_database_msgs::pcl);
  pcl::PointCloud<PointPLY>::Ptr plyPt(new pcl::PointCloud<PointPLY>);
  pcl::PointCloud<PointPLY> ply;
  ply.width=1;
  ply.height = 0;

  // second point line
  string line = "";
  long totalLine = 0;
  //long totalPoints = 0;
  //long numFeatures = 0;
  long points = 0;
  long numP=0;
  //bool testLine = false;
  char* pch;
  // the first four lines represent the general settings of the points
  int offset = 10;
  ifstream source(path.c_str());
  if (source.is_open())
  {
    ROS_INFO("Loading file data to pcl Begin");
    char * tokenString;
    while (getline(source, line))
    {
        totalLine++;
        //ROS_INFO("Points %i",totalPoints);
        if (totalLine > offset)
        {
          points++;
          float temp[6];
          pch = (char*)malloc(sizeof(char) * (line.length() + 1));
          strcpy(pch, line.c_str());
          tokenString = strtok(pch, " ");
          int i = 1;
          temp[0]=atof(tokenString);
          tokenString = strtok(NULL, " ");
          while ((tokenString != NULL))
          {
            temp[i]=atof(tokenString);
            tokenString = strtok(NULL, " ");
            i++;
          }
          ply.points[i].x=temp[0];
          ply.points[i].y=temp[1];
          ply.points[i].z=temp[2];
          ply.points[i].rgbr=temp[3];
          ply.points[i].rgbb=temp[4];
          ply.points[i].rgbg=temp[5];

         }else{

             if(totalLine == 3){
               pch = (char*)malloc(sizeof(char) * (line.length() + 1));
               strcpy(pch, line.c_str());
               tokenString = strtok(pch, " ");
               tokenString = strtok(NULL, " ");
               tokenString = strtok(NULL, " ");
               //the third element is the number of points
               ROS_INFO("Resized with points %ld",atol(tokenString));
               ply.points.resize(atol(tokenString));
               numP=atol(tokenString);
               ply.height = numP;
             }
          }
        }
      }
  else
  {
    ROS_INFO("Cannot open file data to load Pcl");
  }
  ROS_INFO("PCL loaded with: %i size", ply.points.size());
  *plyPt=ply;
  sensor_msgs::PointCloud2 pcl2Msg;
  pcl::toROSMsg(*plyPt,pcl2Msg);
  pcl->pcl=pcl2Msg;
  ROS_INFO("Retrieval executed");

}

arm_navigation_msgs::Shape utLoadMeshFromFile(std::string path){
  arm_navigation_msgs::Shape mesh;
  arm_navigation_msgs::Shape::Ptr meshPt (new arm_navigation_msgs::Shape );
  geometry_msgs::Point pt;
  std::cerr << path.c_str() << endl;

    // 3 type means that this shape is a mesh
  mesh.type = 3;
  bool startCoord=false;
  bool startIndex=false;
  char * pch;
  string line;
  ifstream source(path.c_str());
  if(source.is_open()){
    char * tokenString;
    while (getline(source, line))
    {


      if ((line.find("]")!=string::npos) && (startCoord) ){

        startCoord=false;
      } else if ((line.find("]")==string::npos) && (startCoord)){
          pch = (char*)malloc(sizeof(char) * (line.length() + 1));
          strcpy(pch, line.c_str());
          tokenString = strtok(pch, " ");
          pt.x = strtod(tokenString,NULL);
          tokenString = strtok(NULL, " ");
          pt.y = strtod(tokenString,NULL);
          tokenString = strtok(NULL, " ");
          pt.z = strtod(tokenString,NULL);
          mesh.vertices.push_back(pt);
      }


      if ((line.find("]")!=string::npos) && (startIndex) ){

        startIndex=false;
      }else if ((line.find("]")==string::npos) && (startIndex)){

        pch = (char*)malloc(sizeof(char) * (line.length() + 1));
        strcpy(pch, line.c_str());
        tokenString = strtok(pch, ",");

        mesh.triangles.push_back(atoi(tokenString));

        tokenString = strtok(NULL, ",");

        mesh.triangles.push_back(atoi(tokenString));

        tokenString = strtok(NULL, ",");

        mesh.triangles.push_back(atoi(tokenString));

      }
      if (line.find("Coordinate3")!=string::npos){

        getline(source, line);
        if (line.find("point")!=string::npos){
          startCoord=true;
        }
      }

      if (line.find("IndexedFaceSet")!=string::npos){
        getline(source, line);
         if (line.find("coordIndex")!=string::npos){

          startIndex=true;
         }
       }
    }


  } else {
    ROS_INFO("Cannot load file mesh");
  }

  return mesh;
}

