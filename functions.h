/** @author Yao-Wang Li
  * @date 2018-10-14
  * @email: liyaowang2911@gmail.com
  * @version 0.1
  * @brief header file for functions which do image processing. 
 */


#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <vector>
#include <fstream>
#include <tiffio.h>
#include <algorithm>


using namespace std;

void calculateSlope(const vector<vector<float>> & in, vector<float> & out);
//int peakEnds(vector<float> &in, vector<vector<int>> &out);
//int findPeaks(vector<vector<float>> & in, vector<int> & out, float stdLimit);
int findPeaks(vector<vector<float>> & in, vector<int> & out, float beamY, int gapStart, float stdLimit);
int findPeaksRange(vector<float> & in, vector<int> & peaks, vector<vector<int>> & peaksRange);
int integralForPeaks(vector<vector<float>> & in, vector<vector<int>> &peaksRange, vector<float>& peaksArea);
string detectTiffType(string filein);
float calculateDValue(int &numOfpixels, float & pixelSize, float & distance, float & wavelength);
void vectorOfCharToInt(vector<char> & in, vector<int> & out);
template <class T>
int writeVec2File( vector<T> &vec1, const char * filename)
{
	ofstream fout(filename);
	if(!fout)
	{
		cout<<"File cannot be found"<<endl;
		return -1;
	}
	for (int i=0; i<vec1.size(); i++)
	{
		fout<<vec1[i]<<endl;
	}
	fout.close();
	return 0;
}

template <class T>
int writePoints2File( vector<vector<T>> &vec2, const char * filename)
{
	ofstream fout(filename);
	if(!fout)
	{
		cout<<"File cannot be open!"<<endl;
	}
	for (int i=0 ;i<vec2.size(); i++)
	{
		fout<<vec2[i][0]<< "\t"<<vec2[i][1]<<endl;
	}
	fout.close();
	return 0;

}
template <class T>
int writeMatrix2File( vector<vector<T>> &matrix, const char * filename)
{
	ofstream fout(filename);
	if(!fout)
	{
		cout<<"File cannot be open!"<<endl;
	}
	for (int i=0 ;i<matrix.size(); i++)
    {
	    for (int j=0 ;j<matrix[i].size(); j++)
	    {
		    fout<<matrix[i][j]<< "\t";
    	}
        fout<<endl;
    }
	fout.close();
	return 0;

}


template <class T> vector<T> ReadTiff(string fileIn, uint32 &w, uint32 & h)
{
	vector<T> data;
	TIFF * tif = TIFFOpen(fileIn.c_str(), "r");
	
	if(tif) 
	{
		size_t npixels;
		uint32 * raster;
		
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		npixels = w * h;
	
		raster = (uint32 *) _TIFFmalloc(npixels * sizeof(uint32));
		if(raster != NULL)
		{
			if(TIFFReadRGBAImageOriented(tif, w, h, raster, ORIENTATION_TOPLEFT, 0))
			{
				for (size_t i=0; i<npixels; i++)
				{
					data.push_back(raster[i]);
				}
			}
			_TIFFfree(raster);
		}
		TIFFClose(tif);
	}
	return data;
}
template<class T>
void writeTifImage(vector<T> & data, string &outfile, uint32 & w, uint32 & h)
{
	TIFF * tif = TIFFOpen(outfile.c_str(), "w");
	if (!tif)
	{
		fprintf(stderr, "cannot open a tif file.");
	}
	
    TIFFSetField(tif,TIFFTAG_IMAGEWIDTH,w);
    TIFFSetField(tif,TIFFTAG_IMAGELENGTH,h);
    TIFFSetField(tif,TIFFTAG_SAMPLESPERPIXEL,1);
    TIFFSetField(tif,TIFFTAG_BITSPERSAMPLE,32);
    TIFFSetField(tif,TIFFTAG_ORIENTATION,ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    TIFFWriteEncodedStrip(tif, 0, &data[0], w*h*4);             
    TIFFClose(tif);

}


template<class T> 
void maskBeamAndGap(vector<T> &data, vector<int> &mask, uint32 w, uint32 h, float beamX , float beamY)
{
    //the horizental gap of th two detectors (hypix 3000)
	for (int i=384; i<416; i++)
	for (int j=0; j<w; j++)
	{
		mask[i*w+j] = 1;
	}

    //the beam stop
	for (int i=0; i<beamY; i++)
	for (int j=380; j<392; j++)
	{
		mask[i*w+j] = 1;
	}

	int beamRadius = ceil(beamY - 384);
	int yUp = beamY-beamRadius+1;
	int yDown = beamY+beamRadius+1;
	int xLeft = beamX - beamRadius+1;
	int xRight = beamX + beamRadius+1;

	for(int i=yUp; i<yDown; i++)
	for(int j=xLeft; j<xRight; j++)
	{
		float r=sqrt((j-beamX)*(j-beamX)+(i-beamY)*(i-beamY));
		if (r <= beamRadius)
		{
			mask[i*w+j] = 1;
		}
	}
	


}

template<class T>
vector<vector<float>> convert2DTo1D(vector<T> & data, float & beamX, float & beamY, vector<int> & mask, uint32 & w, uint32 & h, float & pixelSize, float & distance, float & wavelength)
{
//	int maxRadius = round(sqrt(beamX*beamX+beamY*beamY));
	int maxRadius = 0;
	beamX > beamY ? maxRadius = ceil(beamY): maxRadius = ceil(beamX);
	int r = 0;
	vector<vector<float>> oneDimData(maxRadius+1,vector<float>(3,0));
//	vector<vector<float>> medianData(maxRadius+1,vector<float>());

	for (int i=0; i<h; i++)
	for (int j=0; j<w; j++)
	{
		r = round(sqrt((j-beamX)*(j-beamX) + (i-beamY) * (i-beamY)));
		if (r <= maxRadius && mask[i*w+j]==0)
		{
			oneDimData[r][1] += data[i*w+j];
			oneDimData[r][2] += 1;
  //          medianData[r].push_back(data[i*w+j]);
		}
	
	}

//    for (int i=0; i<oneDimData.size(); i++)
  //  {
    //    oneDimData[i][1] /= oneDimData[i][2];
   // }

//    for (int i=0; i<(maxRadius+1); i++)
  //  {
    //    vector<float> tmp = medianData[i];
      //  std::sort(tmp.begin(), tmp.end());
        //int middle = ceil(tmp.size()/2);
      //  float sum = 0.0;
       // for (int i=middle; i<tmp.size(); i++)
        //{
         //   sum +=tmp[i];
       // }
    //    oneDimData[i][0] = 1/calculateDValue(i, pixelSize, distance, wavelength);
      //  oneDimData[i][1]=sum;//- oneDimData[i][1];
    }
	
	for (int i=0; i<oneDimData.size(); i++)
	{
            //   if(oneDimData[i][1]>0)
            //    {
	          //  oneDimData[i][1] /= oneDimData[i][2];
               // }
                oneDimData[i][0] = 1/calculateDValue(i, pixelSize, distance, wavelength);

	} 
	
        return oneDimData;
}

template<class T>
void findMaxMin(vector<T> & data, T & max, T & min)
{
	 min = data[0];
	 max = data[1];
	for (int i=0; i<data.size(); i++)
	{
		if (data[i] < min)
		{
			min = data[i];
		}
		else if (data[i] > max)
		{
			max = data[i];
		}
		else
		{
			continue;
		}
	}
}

template<class T>
void intensityInvert(vector<T> &data)
{
	T min = 0;
	T max = 0;
	findMaxMin<T>(data, max, min);
    cout<<"max value: "<<max<<" min value: "<<min<<endl;
    for (int i=0;i<data.size(); i++)
    {
    	data[i] =max -data[i];
       // data[i] = abs(data[i]);
    }

}	


 

#endif
