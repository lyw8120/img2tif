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
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct axis {
    axis():start(0.0), end(0.0), offset(0.0),name(""){}
    double start, end, offset;
    string name;
};

struct oscHeader {
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
    float fadingStart;          //fading time, end of exposure to start of read
    float fadingEnd;            //fading time, end of exposure to end of read
    char host[10];              //type of computer, (IRIS, VAX) ==> endian
    char ip[10];                //type of ip
    long directionX;            //horizontal scanning code; 0: left -> right, 1: right -> left
    long directionY;            //vertical scanning code; 0: down -> up; 1: up -> down
    long directionZ;            //front/back scanning code; 0: front; 1: back
    float shift;                //pixel shift, R-AXIS V
    float ineo;                 //intensity ratio E/O R-AXIS v
    long magicNum;              //magic number to indicate next values are legit
    long numOfgonioAxis;        //number of goniometer axes
    float gonioVec[5][3];       //goniometer axis vectors
    float gonioStartAngle[5];   //start angles for each of 5 axis
    float gonioEndAngle[5];     //end angles for each of 5 axis
    float gonioShift[5];         //offset values for each of 5 axis
    long scanAxis;              //which axis is the scan axis
    char gonioAxisName[40];     //Names of the axes (space or comma separated?)

    //the following is program dependent.
    char file[16];
    char cmnt[20];
    char smpl[20];
    long iext;
    long reso;
    long save;
    long dint;
    long byte;
    long init;
    long ipus;
    long dexp;
    long expn;
    long posx[20];
    long posy[20];
    int xray;
    char res5[768];             //reserved space

};

//constant variable
const int headerLength = 1400;

class osc 
{
    private:
        string _filename;
        oscHeader _header;
    public:
        osc(string filename, bool all = true);
        int readHeader(ifstream & in);
        int readData(ifstream & in);
        int printHeaderInfo();
        unsigned short swapv(const unsigned short v);
        long swapv(const long v);
        float swapv(const float v);
        int swapHeader(oscHeader & header);

        ~osc();


};

#endif

