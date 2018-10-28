/** @author Yao-Wang Li
  * @date 2018-10-14
  * @email: liyaowang2911@gmail.com
  * @version 0.1
  * @brief header file for class detrek
  *
  * 		    size1
  * 		------------------------------>
  *		   |
  * size2  |
  *   	   |
  *    	   |
  *    	   |
  *    	   |
  *    	   |
  *    	   V
 */


#ifndef _DETREK_H_
#define _DETREK_H_

#include <iostream>
#include <string>
#include <cstdint>
#include <fstream>
#include <cctype>
#include <cmath>
#include <cstring>
#include <tiffio.h>
#include <vector>
#include <sstream>


using namespace std;


class detrek {
	private:
		//the length of the entire header in bytes 
		int headerBytes;

		//the number of dimensions in the image
		int imageDims;

		//the number of pixels along the first direction
		int size1;

		//the number of pixels along the second direction
		int size2;

		//imagesize
		int imageSize;
	
		//gap length 
		int gapLength = 17;

		//the type of data
		string dataType;
		
		//the beam position
		float beamX;
		float beamY;
	
		//pixel size (mm/pixel) in x and y direction
		float pixelSizeX;
		float pixelSizeY;

		//distance between sample and detector
		float distance;

		/**
 		  * the type of compression used for the entire image data, 
 		  * currently is not supprot image data compression
 		  */
		string compression;

		//the maximum value that a pixel an have 
		int saturatedValue;

		//the scan wavelength 
		float scanWaveLength;

		//byte order
		string byteOrder;

		//input file name
		string file;
	
		//image data
		int32_t * data = NULL;

		//mask for beam stop and the gap between detectors
		bool * mask = NULL;

	public:

		//defalut constructor
		detrek();
		
		//constructor
		detrek(string in_file);
		
		//deconstructor
		~detrek();
	
		//read image header
		void readHeader();
		
		//print image header inforamtion
		void printHeaderInfo();
	
		//read rigaku image 
		void readImage();
	
		//calculate the d spacing 
		float calculateDValue(int numOfpixels);

		//print rigaku image data
		void printData();

		//cut the center blank area. 17x484
		void cutCenterBlank();
	
		//mask the beam stop and gap
		void maskBeamAndGap();
	
		//convert 2D image to 1D conventional powder diffraction data
		void convert();

		//write image data using the tiff format
		void writeTifImage(string outfile);

};

#endif

