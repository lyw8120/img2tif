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

int findPeaks(vector<vector<float>> & in, vector<int> & out)
{
	//start from 25
	for (int i=25; i<in.size()-5; i++)
	{
		float mean = (in[i][1] +  in[i+1][1] + in[i+2][1] + in[i+3][1] + in[i+4][1])/5;
		float std = sqrt(pow((in[i][1]-mean),2) +  pow((in[i+1][1]-mean),2) + pow((in[i+2][1]-mean),2) + pow((in[i+3][1]-mean),2) + pow((in[i+4][1]-mean),2)/4);
		//if std > 1 and the middle element is the maximum.
		if ( std> 1 && in[i+2][1]>in[i+1][1] && in[i+2][1]>in[i+3][1] && in[i+1][1]>in[i][1] && in[i+3][1]>in[i+4][1])
		{
			out.push_back(i+2);
			i += 5;
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
		int tmp = peaks[i];
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
