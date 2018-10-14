/** @author Yao-Wang Li
  * @date 2018-10-14
  * @email: liyaowang2911@gmail.com
  * @version 0.1
  * @brief header file for class detrek
 */


#ifndef _DETREK_H_
#define _DETREK_H_

#include <iostream>
#include <string>

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

		//the type of data
		string dataType;
		
		//the beam position
		int beamX;
		int beamY;

		/**
 		  * the type of compression used for the entire image data, 
 		  * currently is not supprot image data compression
 		  */
		string compression;

		//the maximum value that a pixel an have 
		int saturatedValue;

		//the scan wavelength 
		float scanWaveLength;

		//input file name
		string file;
	
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

		//write image data using the tiff format
		void writeTifImage(string outfile);

};

#endif

