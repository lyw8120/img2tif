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


using namespace std;

void calculateSlope(const vector<vector<float>> & in, vector<float> & out);
//int peakEnds(vector<float> &in, vector<vector<int>> &out);
int findPeaks(vector<vector<float>> & in, vector<int> & );
//int writeVec2File( vector<float> &vec1, const char * filename);
//int writePoints2File( vector<vector<float>> &vec2, const char * filename);
int findPeaksRange(vector<float> & in, vector<int> & peaks, vector<vector<int>> & peaksRange);
int integralForPeaks(vector<vector<float>> & in, vector<vector<int>> &peaksRange, vector<float>& peaksArea);

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
#endif
