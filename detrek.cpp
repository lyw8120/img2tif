/** @author Yao-Wang Li
 *  @date 2018-10-14
 *  @email: liyaowang2911@gmail.com
 *  @version 0.1
 *  @brief implemnt the functions in the class detrek
 */

#include "detrek.h"
#include <fstream>
#include <cctype>
#include <cmath>
using namespace std;

detrek::detrek()
{
}

detrek::detrek(string in_file)
{
	detrek::file=in_file;
	readHeader();

	
}

detrek::~detrek()
{
}


void detrek::readHeader()
{
	string line;
	ifstream myfile(file.c_str(), ios::in|ios::binary);
	if (myfile.is_open())
	{
		if (!myfile.eof())
		{
			while (getline(myfile,line))
			{
				std::size_t equalSign=line.find("=");
				std::size_t semiconPos=line.find(";");
				int len=semiconPos - equalSign - 1;
				if (equalSign && semiconPos)
				{
					string name=line.substr(0,equalSign);
					string value=line.substr(equalSign+1,len);
					if(name=="HEADER_BYTES")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::headerBytes=stoi(value);
	//					cout<<detrek::headerBytes<<endl;
					}
					else if(name=="DIM")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::imageDims=stoi(value);
	//					cout<<detrek::imageDims<<endl;
					}
					else if (name=="Data_type")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::dataType=value;
	//					cout<<detrek::dataType<<endl;
					}
					else if (name=="SIZE1")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::size1=stoi(value);
	//					cout<<detrek::size1<<endl;
					}
					else if (name=="SIZE2")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::size2=stoi(value);
	//					cout<<detrek::size2<<endl;
					}
					else if (name=="SCAN_WAVELENGTH")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::scanWaveLength=stof(value);
	//					cout<<detrek::scanWaveLength<<endl;
					}
					else if (name=="PILT_SPATIAL_BEAM_POSITION")
					{
						std::size_t space=value.find_last_of(" ");
						string beamX=value.substr(0,space);
						string beamY=value.substr(space+1);
				//		cout<<"value: "<<value<<endl;
				//		cout<<"beamX: "<<beamX<<endl;
				//		cout<<"beamY: "<<beamY<<endl;
				//		cout<<"line: "<<line<<endl;
						float beamXTmp=stof(beamX);
						float beamYTmp=stof(beamY);
						detrek::beamX=round(beamXTmp);
						detrek::beamY=round(beamYTmp);
	//					cout<<"beam position: "<<detrek::beamX<<" "<<detrek::beamY<<endl;
					}
					else if (name=="SATURATED_VALUE")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::saturatedValue=stoi(value);
	//					cout<<detrek::saturatedValue<<endl;
					}
					else if (name=="COMPRESSION")
					{
				//		cout<<value<<endl;
				//		cout<<line<<endl;
						detrek::compression=value;
	//					cout<<detrek::compression<<endl;
					}
					else
					{
						continue;
					}	
				
				}
			
			}
		}	
		myfile.close();
	}
	else
	{
		cout<<"unable to open file"<<endl;
	}

}

void detrek::printHeaderInfo()
{
cout<<"Header size: "<<detrek::headerBytes<<" bytes"<<endl;
cout<<"Image dimensions: "<<detrek::imageDims<<endl;
cout<<"The number of pixels in the first dimension: "<<detrek::size1<<endl; 
cout<<"The number of pixels in the second dimension: "<<detrek::size2<<endl; 
cout<<"Image data type: "<<detrek::dataType<<endl;
cout<<"The beam position (in pixel): "<<detrek::beamX<<" "<<detrek::beamY<<endl;
cout<<"Was data compressed? "<<detrek::compression<<endl;
cout<<"The maximum value that a pixel can have: "<<detrek::saturatedValue<<endl;
cout<<"The scan wave length: "<<detrek::scanWaveLength<<endl;
}

void detrek::readImage()
{
}

void detrek::writeTifImage(string outfile)
{
}


