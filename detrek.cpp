/** @author Yao-Wang Li
 *  @date 2018-10-14
 *  @email: liyaowang2911@gmail.com
 *  @version 0.1
 *  @brief implemnt the functions in the class detrek
 */

#include "detrek.h"

detrek::detrek()
{
}

detrek::detrek(string in_file)
{
	detrek::file=in_file;
	detrek::readHeader();
	detrek::readImage();
	
}

detrek::~detrek()
{
	delete data;
}


void detrek::readHeader()
{
	string line;
	ifstream myfile(detrek::file.c_str(), ios::in|ios::binary);
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
						detrek::headerBytes=stoi(value);
					}
					else if(name=="DIM")
					{
						detrek::imageDims=stoi(value);
					}
					else if (name=="Data_type")
					{
						detrek::dataType=value;
					}
					else if (name=="SIZE1")
					{
						detrek::size1=stoi(value);
					}
					else if (name=="SIZE2")
					{
						detrek::size2=stoi(value);
					}
					else if (name=="SCAN_WAVELENGTH")
					{
						detrek::scanWaveLength=stof(value);
					}
					else if (name=="PILT_SPATIAL_BEAM_POSITION")
					{
						std::size_t space=value.find_last_of(" ");
						string beamX=value.substr(0,space);
						string beamY=value.substr(space+1);
						float beamXTmp=stof(beamX);
						float beamYTmp=stof(beamY);
						detrek::beamX=round(beamXTmp);
						detrek::beamY=round(beamYTmp);
					}
					else if (name=="SATURATED_VALUE")
					{
						detrek::saturatedValue=stoi(value);
					}
					else if (name=="COMPRESSION")
					{
						detrek::compression=value;
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
	ifstream myfile(detrek::file.c_str(), ios::in|ios::binary);
	myfile.seekg(detrek::headerBytes);
	int imgSize = detrek::beamX * detrek::beamY;

	if (detrek::dataType=="long int" )
	{
		int byteSize = imgSize * 4 ;
		int charSize = byteSize/8;
		char * tmp = new char [charSize];
		if(!tmp)
		{
			cout<<"memory error, cannot read image data into buffer!"<<endl;
			exit(1);
		}
		myfile.read(tmp, charSize);	
		
		data = new int32_t [imgSize];
		if (!data)
		{
			cout<<"cannot allocate memory for image."<<endl;
			exit(1);
		}
			memcpy(data, tmp, byteSize);
		delete tmp;
	}
//	else if (detrek::dataType=="unsigned long int")
//	{
//		unsigned long int * tmp = new unsigned long int[size];
//		data = tmp;
//		myfile.read((unsigned long int *)data, size);	
//	}
//	else if(detrek::dataType=="short int" )
//	{
//		short int * tmp = new short int [size];
//		data = tmp;
//		myfile.read((short int *)data, size);	
//	}
//	else if ( detrek::dataType=="unsigned short int")
//	{
//		unsigned short int * tmp = new unsigned short int [size];
//		data = tmp;
//		myfile.read((unsigned short int *)data, size);	
//	}
//	else if (detrek::dataType == "signed char")
//	{
//		signed char * tmp = new signed char [size];
//		data = tmp;
//		myfile.read((signed char *)data, size);	
//	}
//	else if (detrek::dataType == "unsigned char")
//	{
//		unsigned char * tmp = new unsigned char [size];
//		data = tmp;
//		myfile.read((unsigned char *)data, size);	
//	}
	else
	{
		cout <<"the data type is not supported!"<<endl;	
	}
	
	myfile.close();	 
}

void detrek::printData()
{
	for (int i=0; i<detrek::beamX; i++)
	{
		for (int j=0; j<detrek::beamY; j++)
		{
			cout << data[i*detrek::beamX+j]<<" ";
		}
		cout <<endl;
	}
}

void detrek::writeTifImage(string outfile)
{
}


