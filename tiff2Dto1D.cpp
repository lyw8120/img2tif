/** @author Yao-Wang Li
 * @date 2018-11-10
 * @email: liyaowang2911@gmail.com
 * @version: 0.1
 * @brief convert the 2d powder diffraction (tiff format) to conversional one dimensional power diffraction data.
 */

#include <cstdlib>
#include "functions.h"

int main (int argc, char ** argv)
{
	if (argc != 8)
	{
		std::cout<<"I need the tiff file name, pixelsize, camera length, wavelength, beamX, beamY and std for peak searching. thank you."<<std::endl;
		exit(1);
	}

	string fileName=string(argv[1]);
	int position=fileName.find_last_of(".");
	if (fileName.substr(position+1)!="tif" && fileName.substr(position+1)!="tiff")
	{
		std::cout<<"it is not an image with the extension 'tif' or 'tiff'."<<std::endl;
		exit(2);
	}
	float pixelSize = atof(argv[2]);
	float distance = atof(argv[3]);
	float wavelength = atof(argv[4]);
	float beamX = atof(argv[5]);
	float beamY = atof(argv[6]);
	float std = atof(argv[7]);

	
	string baseName=fileName.substr(0, position);
	string outFileName=baseName + ".1d";
	string outMaskName=baseName + "_mask.tif";
	string outInvName=baseName + "_inv.tif";
	string outpeaksRange=baseName + "_peaksRange.txt";
    string outpeaksArea=baseName + "_peaksArea.txt";
	
	std::cout<<"This program can convert the tiff format two dimensional diffraction image to 1D data."<<std::endl;
	std::cout<<"Have fun with this program, good luck to you. from Yao-Wang. "<<std::endl;

	string tiffType=detectTiffType(fileName);
	
	//vector<int> data;
	vector<uint32> data;
	vector<vector<float>> oneDimPowder;
	uint32 w = 0;
	uint32 h = 0;

	if ( tiffType == "char")
	{
		cout<<"the tif data type: "<<tiffType<<endl;
		//vector<char> tmp;
		//tmp = ReadTiff<char>(fileName, w, h);
	//	data = ReadTiff<uint32>(fileName, w, h);
		//vectorOfCharToInt(tmp, data);
	}
	else if(tiffType== "short" || tiffType == "int")
	{
		cout<<"the tif data type: "<<tiffType<<endl;
	//	data = ReadTiff<uint32>(fileName, w, h);

	}
	else
	{
		cout<<"I cannot process this image"<<endl;
		exit(1);
	}

	data = ReadTiff<uint32>(fileName, w, h);
    cout<<data[311*w+314]<<endl;
	intensityInvert<uint32>(data);
	vector<int> mask(data.size(), 0);
	maskBeamAndGap<uint32>(data,mask, w, h, beamX, beamY);
	oneDimPowder = convert2DTo1D<uint32>(data, beamX, beamY, mask, w, h, pixelSize, distance, wavelength);
	

    vector<float> slopeValues;
  //  powderData is contains the d value and corresponding intensity value.
    calculateSlope(oneDimPowder, slopeValues);

	vector<int> peaksPosition;
	vector<vector<float>> peaksRes;
    //gap from 384 to 416
	findPeaks(oneDimPowder, peaksPosition, beamY, 384,  std);

    vector<vector<int>> peakTwoEnds;
    findPeaksRange(slopeValues, peaksPosition, peakTwoEnds);

    vector<float> area;
    integralForPeaks(oneDimPowder, peakTwoEnds, area);

	
	for (int i=0; i<peaksPosition.size(); i++)
	{
        vector<float> tmp(4,0);
		float d = calculateDValue(peaksPosition[i], pixelSize, distance, wavelength);
        tmp[0] = peaksPosition[i];
        tmp[1] = d;
        tmp[2] = 1/d;
        tmp[3] = oneDimPowder[peaksPosition[i]][1];
		peaksRes.push_back(tmp);
	}
	string outSlope = baseName + "_slope.txt";
	writeVec2File<float>(slopeValues, outSlope.c_str());
		
	string outPeaks = baseName + "_peaks.txt";
	writeMatrix2File<float>(peaksRes, outPeaks.c_str());

	writePoints2File<float>(oneDimPowder, outFileName.c_str());
	writePoints2File<int>(peakTwoEnds, outpeaksRange.c_str());
	writeVec2File<float>(area, outpeaksArea.c_str());

	writeTifImage<uint32>(data, outInvName, w, h);
	writeTifImage<int>(mask, outMaskName, w, h);

	return 0;
}
