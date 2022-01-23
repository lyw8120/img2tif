/**
 * author: Dr. Yaowang Li
 * date: Jan. 20, 2022
 * @brief osc.cpp implements the functions of osc.h has defined.
 */

#include "osc.h"

osc::osc(string filename, bool all)
{
    _filename = filename;
    ifstream is(_filename.c_str(), std::ios::in|std::ios::binary);

    if (is.bad() || is.fail())
    {
        std::cerr << "cannot open the file: "<<filename << std::endl;
        if(is.is_open())
        {
            is.close();
        }
        exit(1);
    }

    readHeader(is);

    if (all)
    {
        readData(is);
    }
    
}

int osc::readHeader(std::ifstream &in)
{
    in.read(reinterpret_cast<char *> (&_header), headerLength);
    
    if (in.bad() || in.fail())
    {
        return 1;
    }

    unsigned short one = 1;
    char *p = reinterpret_cast<char *> (&one);
    if (1 == static_cast<int> (*p))
    {
        swapHeader(_header);
    }

    long size = _header.recordLength - headerLength;
    if (size > 0)
    {
        p = new char [size];
        in.read(p, size);
        delete p;
    }

    return 0;
    
}

int osc::readData(std::ifstream &in)
{
    
    return 0;
}


int osc::printHeaderInfo()
{
    fprintf(stdout, "Header Information:\n");
    fprintf(stdout, "instrument: %s\n", _header.rigakuName);
    fprintf(stdout, "version: %s\n", _header.version);
    fprintf(stdout, "unit cell a: %f\n", _header.alength);
    fprintf(stdout, "unit cell b: %f\n", _header.blength);
    fprintf(stdout, "unit cell c: %f\n", _header.clength);
    fprintf(stdout, "unit cell alpha: %f\n", _header.alpha);
    fprintf(stdout, "unit cell beta: %f\n", _header.beta);
    fprintf(stdout, "unit cell gamma: %f\n", _header.gamma);
    fprintf(stdout, "space group: %s\n", _header.spaceGroupSymbol);
    fprintf(stdout, "date: %s\n", _header.date);
    fprintf(stdout, "user: %s\n", _header.userName);
    fprintf(stdout, "x-ray source: %s\n", _header.xraySource);
    fprintf(stdout, "wave length: %f\n", _header.waveLength);
    fprintf(stdout, "monochromator: %s\n", _header.typeMonochromator);
    fprintf(stdout, "monochromator two theta: %f\n", _header.m2theta);
    fprintf(stdout, "collimator: %s\n", _header.collimator);
    fprintf(stdout, "filter: %s\n", _header.filter);
    fprintf(stdout, "camera length: %f\n", _header.cameraLength);
    fprintf(stdout, "current: %f\n", _header.current);
    fprintf(stdout, "voltage: %f\n", _header.voltage);
    fprintf(stdout, "focus: %s\n", _header.focusInfo);
    fprintf(stdout, "memo: %s\n", _header.xrayMemo);
    if (_header.shapeType == 0)
    {
        fprintf(stdout, "shape: flat\n");
    }
    else if (_header.shapeType == 1)
    {
        fprintf(stdout, "shape: cylinder\n");
    }

    fprintf(stdout, "shape: %d\n", _header.shapeType);

    if (_header.weissenbergOsci == 1)
    {
        fprintf(stdout, "Oscillation type: weissenberg\n");
    }
    else
    {
        fprintf(stdout, "Oscillation type: regular\n");
    }

    fprintf(stdout, "crystal mount (spindle axis): %s\n", _header.mAxisCloseSpindleAxis);
    fprintf(stdout, "crystal mount (beam axis): %s\n", _header.mAxisCloseBeamAxis);
    fprintf(stdout, "datum phi angle: %f\n", _header.phi0);
    fprintf(stdout, "start phi angle: %f\n", _header.phiStart);
    fprintf(stdout, "end phi angle: %f\n", _header.phiEnd);
    fprintf(stdout, "frame number: %d\n", _header.frameNum);
    fprintf(stdout, "exposure time: %f\n", _header.exposureTime);
    fprintf(stdout, "beam X: %f\n", _header.beamX);
    fprintf(stdout, "beam Y: %f\n", _header.beamY);
    fprintf(stdout, "goniostat omga angle: %f\n", _header.omga);
    fprintf(stdout, "goniostat chi angle: %f\n", _header.chiAngle);
    fprintf(stdout, "goniostat two theta angle: %f\n", _header.gonio2theta);
    fprintf(stdout, "spindle inclination angle: %f\n", _header.spindleAngle);
    fprintf(stdout, "template information: %s\n", _header.scanSpace);
    fprintf(stdout, "pixel number in X: %ld\n", _header.pixelsX);
    fprintf(stdout, "pixel number in Z: %ld\n", _header.pixelsZ);
    fprintf(stdout, "pixel size in X: %f\n", _header.pixelSizeX);
    fprintf(stdout, "pixel size in Y: %f\n", _header.pixelSizeZ);
    fprintf(stdout, "record length(bytes): %ld\n", _header.recordLength);
    fprintf(stdout, "number of records: %ld\n", _header.recordNum);
    fprintf(stdout, "starting: %ld\n", _header.lineNum);
    fprintf(stdout, "IP number: %ld\n", _header.ipNum);
    fprintf(stdout, "photomultiplier output hi/lo ratio: %f\n", _header.hiloRatio);
    fprintf(stdout, "fade time (to start of read): %f\n", _header.fadingStart);
    fprintf(stdout, "fade time (to end of read): %f\n", _header.fadingEnd);
    fprintf(stdout, "host type/endian: %s\n", _header.host);
    fprintf(stdout, "IP type: %s\n", _header.ip);
    if (_header.directionX == 0)
    {
        fprintf(stdout, "horizontal scanning code: from left to right.\n");
    }
    else
    {
        fprintf(stdout, "horizontal scanning code: from right to left.\n");
    }


    if (_header.directionY ==  0)
    {
        fprintf(stdout, "vertical scanning code: from down to up.\n");
    }
    else
    {
        fprintf(stdout, "vertical scanning code: from up to down.\n");
    }

    if (_header.directionZ == 0)
    {
        fprintf(stdout, "front/back scanning code: front.\n");
    }
    else
    {
        fprintf(stdout, "front/back scanning code: back.\n");
    }

    fprintf(stdout, "pixel shift: %f\n", _header.shift);
    fprintf(stdout, "even/odd intensity ratio (RAXIS V): %f\n", _header.ineo);

    axis Axis[5];
    std::string s = _header.rigakuName;
    bool isRapid = std::string::npos != s.find("R-AXIS-CS");
    std::string ScanAxis = "Phi";

    if (_header.magicNum != 0)
    {
        std::istringstream is(_header.gonioAxisName);
        for (int i=0; i< _header.numOfgonioAxis; i++)
        {
           is >> Axis[i].name;
           Axis[i].start = _header.gonioStartAngle[i];
           Axis[i].end = _header.gonioEndAngle[i];
           Axis[i].offset = _header.gonioShift[i];
        }
        ScanAxis = Axis[_header.scanAxis].name;
    }
    else
    {
        Axis[0].name = "Omega";
        Axis[2].name = "Phi";
        if(!isRapid)
        {
            Axis[0].start = _header.omga;
            Axis[0].end = _header.omga;
            Axis[0].offset = 0.0;
            Axis[2].start = _header.phiStart;
            Axis[2].end = _header.phiEnd;
            Axis[2].offset = _header.phi0;
            ScanAxis = Axis[2].name;
        }
        else
        {
            Axis[2].start = _header.omga;
            Axis[2].end = _header.omga;
            Axis[2].offset = 0.0;
            Axis[0].start = _header.phiStart;
            Axis[0].end = _header.phiEnd;
            Axis[0].offset = _header.phi0;
            ScanAxis = Axis[0].name;
        }
    }


    if (_header.magicNum != 0)
    {
        fprintf(stdout, "Instrument axis (using extended section of header)\n");
    }
    else
    {
        fprintf(stdout, "Instrument axis (using conventional section of header)\n");
    }

    fprintf(stdout, "scan axis: %s\n", ScanAxis.c_str());

    for (int i = 0; i< 5 && Axis[i].name != ""; i++)
    {
        fprintf(stdout, "Axis name: %s\n", Axis[i].name.c_str());
        fprintf(stdout, "Axis start: %f\n", Axis[i].start);
        fprintf(stdout, "Axis end: %f\n", Axis[i].end);
        fprintf(stdout, "Axis offset: %f\n", Axis[i].offset);
    }

    axis theta, dist;
    theta.name = "2theta";
    theta.start = _header.gonio2theta;
    theta.end = _header.gonio2theta;
    theta.offset = 0.0;
    dist.name = "Distance";
    dist.start = _header.cameraLength;
    dist.end = _header.cameraLength;
    dist.offset = 0.0;
    
    fprintf(stdout, "theta Axis name: %s; \tstart angle: %f;\t end angle: %f;\t offset: %f;\n", theta.name.c_str(), theta.start, theta.end, theta.offset);
    fprintf(stdout, "dist Axis name: %s; \tstart angle: %f;\t end angle: %f;\t offset: %f;\n", dist.name.c_str(), dist.start, dist.end, dist.offset);

    return 0;
}

unsigned short osc::swapv(const unsigned short v)
{
    unsigned short sv = v;
    char *p = reinterpret_cast<char *> (&sv);

    char tmp = p[0];
    p[0] = p[1];
    p[1] = tmp;

    return sv;
}

long osc::swapv(const long v)
{
    long sv = v;
    char *p = reinterpret_cast<char *> (&sv);

    char tmp = p[0];
    p[0] = p[3];
    p[3] = tmp;

    tmp = p[1];
    p[1] = p[2];
    p[2] = tmp;

    return sv;
}

float swapv(const float v)
{
    float sv = v;
    char *p = reinterpret_cast<char *> (&sv);

    char tmp = p[0];
    p[0] = p[3];
    p[3] = tmp;

    tmp = p[1];
    p[1] = p[2];
    p[2] = tmp;

    return sv;
}

int swapHeader(oscHeader & header)
{
    //swap the bytes in long elements
    header.shapeType = swapv(header.shapeType);
    header.frameNum = swapv(header.frameNum);
    header.pixelsX = swapv(header.pixelsX);
    header.pixelsZ = swapv(header.pixelsZ);
    header.recordLength = swapv(header.recordLength);
    header.recordNum = swapv(header.recordNum);
    header.lineNum = swapv(header.lineNum);
    header.ipNum = swapv(header.ipNum);
    header.directionX = swapv(header.directionX);
    header.directionY = swapv(header.directionY);
    header.directionZ = swapv(header.directionZ);
    //swap the bytes in the float elements
    header.alength = swapv(header.alength);
    header.blength = swapv(header.blength);
    header.clength = swapv(header.clength);
    header.alpha = swapv(header.alpha);
    header.beta = swapv(header.beta);
    header.gamma = swapv(header.gamma);
    header.mosaic = swapv(header.mosaic);
    header.waveLength = swapv(header.waveLength);
    header.m2theta = swapv(header.m2theta);
    header.cameraLength = swapv(header.cameraLength);
    header.current = swapv(header.current);
    header.voltage = swapv(header.voltage);
    header.weissenbergOsci = swapv(header.weissenbergOsci);
    header.phi0 = swapv(header.phi0);
    header.phiStart = swapv(header.phiStart);
    header.phiEnd = swapv(header.phiEnd);
    header.exposureTime = swapv(header.exposureTime);
    header.beamX = swapv(header.beamX);
    header.beamY = swapv(header.beamY);
    header.omga = swapv(header.omga);
    header.chiAngle = swapv(header.chiAngle);
    header.gonio2theta = swapv(header.gonio2theta);
    header.spindleAngle = swapv(header.spindleAngle);
    header.pixelSizeX = swapv(header.pixelSizeX);
    header.pixelSizeZ = swapv(header.pixelSizeZ);
    header.hiloRatio = swapv(header.hiloRatio);
    header.fadingStart = swapv(header.fadingStart);
    header.fadingEnd = swapv(header.fadingEnd);

    //new variables for RAPID image headers
    header.shift = swapv(header.shift);
    header.ineo = swapv(header.ineo);
    header.magicNum = swapv(header.magicNum);
    header.numOfgonioAxis = swapv(header.numOfgonioAxis);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            header.gonioVec[i][j] = swapv(header.gonioVec[i][j]);
        }
        header.gonioStartAngle[i] = swapv(header.gonioStartAngle[i]);
        header.gonioEndAngle[i] = swapv(header.gonioEndAngle[i]);
        header.goioShift[i] = swapv(header.goioShift[i]);
    }

    header.scanAxis = swapv(header.scanAxis);

    return 0;
}


