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


