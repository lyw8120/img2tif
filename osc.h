/**
 * author: Dr. Yaowang Li
 * date: Jan. 11, 2022
 * @brief osc.h defines the data structure for reading rigaku file format "osc".
 * osc image file has 1400 chars length. 
 *
 */


#ifndef OSC_H_
#define OSC_H_

#include <iostream>

struct osc {
    char rigakuName[10];        //what kind of instrument
    char version[10];           //which version 
    char crystalName[20];       //crystal name
    char crystalSystem[12];     //crystal system
    float alength;              //unit cell a length
    float blength;              //unit cell b length
    float clength;              //unit cell c length
    float alpha;                //unit cell alpha angle
    float beta;                 //unit cell beta angle
    float gamma;                //unit cell gamma angle
    char spaceGroupSymbol[12];  //space group symbol
    float mosaic;               //mosaic spread
    float memoComments[80];     //memo comments
    char reservedSpace[84];     //reserved space
    char date[12];              // date of measurement
    char userName[20];          //username when collecting image
    char xraySource[4];         // like, Cu, Mo 
    float waveLength;           // wave length
    char typeMonochromator[20]; //type of monochromator
    float m2theta;              //monochromator two theta
    char collimator[20];        //collimator size and type
    char filter[4];             //filter type, like Ni
    float cameraLength;         //camera length
    float current;              //generator current
    float voltage;              //generator voltage
    char focusInfo[12];         //focus information
    char xrayMemo[80];          //xray memo
    long shapeType;             //IP shape, 0=flat, 1=cylinder
    float weissenbergOsci;      //weissenberg oscillation 1
    char reservedSpace2[56];    //reserved space 2
    char mAxisCloseSpindleAxis; //crystal mount axis close to spindle axis
    char mAxisCloseBeamAxis;    //crystal mount axis close to beam axis
    float phi0;                 //datum phi angle(degree)
    float phiStart;             //phi oscillation start angle (degree)
    float phiEnd;               //phi oscillation end angle (degree)
    long frameNum;              //frame number
    float exposureTime;         //exposure time
    float beamX;                //direct beam X position
    float beamY;                //direct beam Y position
    float omga;                 //goniostat angle omega
    float chiAngle;             //goniostat angle chi
    float gonio2theta;          //goniostat angle two theta
    float spindleAngle;         //spindle inclination angle
    char scanSpace[204];        //reserved space for future use, is used for storing the scan template information.
    long pixelsX;               //number of pixel in X direction
    long pixelsZ;               //number of pixel in Z direction
    float pixelSizeX;           //pixel size in X direction
    float pixelSizeZ;           //pixel size in Z direction
    long recordLength;          //record length(bytes)
    long recordNum;             //number of records in image
    long lineNum;               //starting line number
    long ipNum;                 //IP number
    float hiloRatio;            //photomultiplier output hi/lo ratio

};
#endif

