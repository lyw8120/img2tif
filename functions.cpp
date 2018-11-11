/** @author Yao-Wang Li
  * @date 2018-10-26
  * @email: liyaowang2911@gmail.com
  * @version 0.1
  * @brief functions for image data processing 
 */

#include "functions.h"


void calculateSlope(const vector<vector<float>> &in, vector<float> & out)
{	
      int n=in.size() -1 ;
      vector<float> tmp (n,0);

	for (int i=0; i<n; i++)
	{
		tmp[i] = (in[i+1][1] - in[i][1])/double(in[i+1][0]-in[i][0]); 
	}
        out = tmp;
}

/* @brief
 * find the peaks using the standard error in the local range (5).
 */

int findPeaks(vector<vector<float>> & in, vector<int> & out, float stdLimit)
{
	//start from 25
	for (int i=25; i<in.size()-3; i++)
	{
		//float mean = (in[i][1] +  in[i+1][1] + in[i+2][1] + in[i+3][1] + in[i+4][1])/5;
		float mean = (in[i][1] +  in[i+1][1] + in[i+2][1])/3;
	//	float std = sqrt(pow((in[i][1]-mean),2) +  pow((in[i+1][1]-mean),2) + pow((in[i+2][1]-mean),2) + pow((in[i+3][1]-mean),2) + pow((in[i+4][1]-mean),2)/4);
		float std = sqrt(pow((in[i][1]-mean),2) +  pow((in[i+1][1]-mean),2) + pow((in[i+2][1]-mean),2)/2);
		//if std > 1 and the middle element is the maximum.
	//	if ( std> stdLimit && in[i+2][1]>in[i+1][1] && in[i+2][1]>in[i+3][1] && in[i+1][1]>in[i][1] && in[i+3][1]>in[i+4][1])
		if ( std> stdLimit && in[i+1][1]>in[i][1] && in[i+1][1]>in[i+2][1])
		{
			//out.push_back(i+2);
			out.push_back(i+1);
			//cout<<i+2<<endl;
			cout<<i+1<<endl;
			//cout << in[i][1]<<" "<<in[i+1][1]<<" "<<in[i+2][1]<<" "<<in[i+3][1]<<" "<<in[i+4][1]<<endl;
			//i +=4;
			i += 2;
		}
	}
		
	return 0;
}

//int peakEnds(vector<float> & in, vector<vector<int>> &out)
//{
//	int n=in.size();
//	vector<int> tmp(2,0);
//	for (int i=0; i<n-3; i++)
//	{
//		if(in[i]<=0 && in[i+1]>=0 && in[i+2]>in[i+1])
//		{	
//			tmp[0] = i+1;
//		}
//		else if(in[i]<in[i+1] && in[i+1]<=0 && in[i+2]>=0)
//		{
//			tmp[1] = i+2;
//		}
//		cout<<tmp[0]<< " "<<tmp[1]<<endl;
//		if (tmp[0] != 0 && tmp[1] != 0 && tmp[1]>tmp[0])
//		{
//			out.push_back(tmp);
//			tmp[0] = 0;
//			tmp[1] = 0;
//		}

//	}
//	return 0;
//}	

/* @brief based on the slope data, searching the given peak edge in two direction.
 *
 */
int findPeaksRange(vector<float> & in, vector<int> & peaks, vector<vector<int>> & peaksRange)
{
	for (int i=0; i<peaks.size(); i++)
	{
		//since the number of element in slope data is less 1 than the number of powder data.
		int tmp = peaks[i]+1;
	
		vector<int> point(2,0);
		for (int j=tmp; j>0; j--)
		{
			if (in[j-1]<in[j])
			{
				point[0] = j -1;
				break;
			}
		}
		for (int j=tmp; j<in.size(); j++)
		{
			if(in[j+1]>in[j])
			{
				point[1] = j+1;
				break;
			}
		}
		peaksRange.push_back(point);
	}
	return 0;
}


int integralForPeaks(vector<vector<float>> & in, vector<vector<int>> &peaksRange, vector<float>& peaksArea)
{
	for (int i=0; i<peaksRange.size(); i++)
	{
		float area = 0;
		for (int j=peaksRange[i][0]; j<=peaksRange[i][1]; j++)
		{
			area += in[j][1];
		}
		peaksArea.push_back(area);
	}

	
	return 0;

}

string detectTiffType(string filein)
{
	TIFF * tif = TIFFOpen(filein.c_str(), "r");
	int b = 8;
	string tifftype;
	if (tif)
	{
		TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &b);
	}
	if (b == 8)
	{
		tifftype="char";
	}
	else if(b == 16)
	{
		tifftype = "short";
	}
	else if(b== 32)
	{
		tifftype ="int";
	}
	else
	{
		cout<<"I have no idea about the tif image bitsample"<<endl;
	}
	TIFFClose(tif);
	return tifftype;
}



float calculateDValue(int &numOfpixels, float & pixelSize, float & distance, float & wavelength)
{
	float d=0.0;
	double theta = 0.0;
	theta = atan(numOfpixels * pixelSize / distance)/2;
	d = wavelength / (2 * sin(theta));
	return d;
}

void vectorOfCharToInt(vector<char> & in, vector<int> & out)
{
	vector<int> tmp(in.begin(), in.end());
	for (int i=0; i<out.size(); i++)
	{
		tmp[i] -= '0';
	}
	out = tmp;
}
