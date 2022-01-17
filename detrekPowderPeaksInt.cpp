/** @author Yao-Wang Li
 * @date 2018-10-26
 * @email: liyaowang2911@gmail.com
 * @version: 0.1
 * @brief convert the rigaku image to conversional one dimensional power diffraction data.
 * @the center blank nad beam stop will be excluded during process. 
 */

#include <cstdlib>
#include "detrek.h"
#include "functions.h"

int main (int argc, char ** argv)
{
	if (argc != 3)
	{
		std::cout<<"I need the detrek image file name and std for peak determination, thank you."<<std::endl;
		exit(1);
	}

	string fileName=string(argv[1]);
	int position=fileName.find_last_of(".");
	if (fileName.substr(position+1)!="img")
	{
		std::cout<<"it is not an image with the extension 'img'."<<std::endl;
		exit(2);
	}

	string baseName=fileName.substr(0, position);
	string outFileName=baseName + ".tif";
	float std = atof(argv[2]);

	std::cout<<"This program can convert the rigaku two dimensional diffraction image to tiff format image."<<std::endl;
	std::cout<<"Have fun with this program, good luck to you. from Yao-Wang. "<<std::endl;

	detrek rigakuImg(fileName);
	//rigakuImg.printHeaderInfo();
	rigakuImg.maskBeamAndGap();
	rigakuImg.convert();
       // rigakuImg.printPowderData();
        
        vector<float> slopeValues;
	//powderData is contains the d value and corresponding intensity value.
        calculateSlope(rigakuImg.powderData, slopeValues);

	vector<int> peaksPosition;
    vector<vector<float>> peaksRes;
    //the gap of detector start from 195 and end at 211.
    int gapStart=195;
    float beamY = rigakuImg.getBeamY();
	findPeaks(rigakuImg.powderData, peaksPosition, beamY, gapStart, std);
    
    float pixelsize = rigakuImg.getPixelSize();
    float distance = rigakuImg.getDistance();
    float wavelength = rigakuImg.getWaveLength();

    for (int i=0; i<peaksPosition.size(); i++)
	{
        vector<float> tmp(4,0);
		float d = calculateDValue(peaksPosition[i], pixelsize, distance, wavelength);
        tmp[0] = peaksPosition[i];
        tmp[1] = d;
        tmp[2] = 1/d;
        tmp[3] = 2*asin(wavelength/(2*d))*180/3.1415926;
		peaksRes.push_back(tmp);
	}

	vector<vector<int>> peakTwoEnds;
	findPeaksRange(slopeValues, peaksPosition, peakTwoEnds);

	vector<float> area;
	integralForPeaks(rigakuImg.powderData, peakTwoEnds, area);
	
	string outSlope = baseName + "_slope.txt";
	writeVec2File<float>(slopeValues, outSlope.c_str());
		
	string outPeaks = baseName + "_peaks.txt";
//	writeVec2File<int>(peaksPosition, outPeaks.c_str());
    writeMatrix2File<float>(peaksRes, outPeaks.c_str());

	string outPowderData = baseName + "_powderData.txt";
	writePoints2File<float>(rigakuImg.powderData, outPowderData.c_str());
	
	string outPeaksRange = baseName + "_peaksRange.txt";
	writePoints2File<int>(peakTwoEnds, outPeaksRange.c_str());

	string outArea = baseName + "_area.txt";
	writeVec2File<float>(area, outArea.c_str());
	
	


	return 0;
}
