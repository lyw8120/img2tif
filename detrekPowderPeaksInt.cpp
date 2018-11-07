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
	if (argc != 2)
	{
		std::cout<<"I need the detrek image file name, thank you."<<std::endl;
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
	findPeaks(rigakuImg.powderData, peaksPosition);
	
	vector<vector<int>> peakTwoEnds;
	findPeaksRange(slopeValues, peaksPosition, peakTwoEnds);

	vector<float> area;
	integralForPeaks(rigakuImg.powderData, peakTwoEnds, area);
	
	string outSlope = baseName + "_slope.txt";
	writeVec2File<float>(slopeValues, outSlope.c_str());
		
	string outPeaks = baseName + "_peaks.txt";
	writeVec2File<int>(peaksPosition, outPeaks.c_str());

	string outPowderData = baseName + "_powderData.txt";
	writePoints2File<float>(rigakuImg.powderData, outPowderData.c_str());
	
	string outPeaksRange = baseName + "_peaksRange.txt";
	writePoints2File<int>(peakTwoEnds, outPeaksRange.c_str());

	string outArea = baseName + "_area.txt";
	writeVec2File<float>(area, outArea.c_str());
	
	


	return 0;
}
