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
	detrek::imageSize = detrek::size1 * detrek::size2;
}

detrek::~detrek()
{
	delete data;
	delete mask;
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
		//			else if (name=="PILT_SPATIAL_BEAM_POSITION")
		//			{
		//				std::size_t space=value.find_last_of(" ");
		//				string beamX=value.substr(0,space);
		//				string beamY=value.substr(space+1);
		//				float beamXTmp=stof(beamX);
		//				float beamYTmp=stof(beamY);
		//				detrek::beamX=beamXTmp;
		//				detrek::beamY=beamYTmp;
		//			}
					else if (name =="PILT_SPATIAL_DISTORTION_INFO")
					{
						stringstream ss;
						ss.str(value);
						string s1, s2,s3,s4;
						ss>>s1>>s2>>s3>>s4;
						detrek::beamX = stof(s1);
						detrek::beamY = stof(s2);
						detrek::pixelSizeX = stof(s3);
						detrek::pixelSizeY = stof(s4);
						
					}
					else if (name == "PILT_GONIO_VALUES")
					{
						stringstream ss;
						ss.str(value);
						string s1, s2, s3, s4,s5,s6;
						ss>>s1>>s2>>s3>>s4>>s5>>s6;
						detrek::distance = stof(s6);
					}
					else if (name=="SATURATED_VALUE")
					{
						detrek::saturatedValue=stoi(value);
					}
					else if (name=="COMPRESSION")
					{
						detrek::compression=value;
					}
					else if (name=="BYTE_ORDER")
					{
						detrek::byteOrder=value;
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
cout<<"The pixel size in X and Y directions (mm/pixel): "<<detrek::pixelSizeX<<" "<<detrek::pixelSizeY<<endl;
cout<<"Was data compressed? "<<detrek::compression<<endl;
cout<<"Distance between sample and detector (in mm): "<<detrek::distance<<endl;
cout<<"The maximum value that a pixel can have: "<<detrek::saturatedValue<<endl;
cout<<"The scan wave length: "<<detrek::scanWaveLength<<endl;
cout<<"The byte order is : "<<detrek::byteOrder<<endl;
}

void detrek::readImage()
{
	FILE * myfile=fopen(detrek::file.c_str(), "rb");
	if (!myfile)
	{
		fprintf(stderr, "cannot open the file to read data.\n");
		fclose(myfile);
		exit(1);
	}

	int imgSize = detrek::size1 * detrek::size2;
	if (detrek::dataType=="long int" )
	{
		data = new int32_t [imgSize];
		if (!data)
		{
			fprintf(stderr, "memory error when allocating memory for image data.\n");
			fclose(myfile);
			exit(1);
		}
		
		fseek(myfile, detrek::headerBytes, SEEK_CUR);

		long intnum = fread(data, sizeof(int32_t), imgSize, myfile);
	
		if(intnum != imgSize)
		{
			fprintf(stderr, "cannot read data into buffer!\n the actual image size is not consistent with the information in image header.\n");
			exit(1);
		}
	
		fclose(myfile);
	}

	else
	{
		fprintf(stderr, "the data type is not supported!\n");	
		exit(1);
		fclose(myfile);	 
	}
	
}

void detrek::printData()
{
	cout << detrek::size1 << " "<<detrek::size2<<endl;
	for (int i=0; i<detrek::size1; i++)
	{
		for (int j=0; j<detrek::size2; j++)
		{
			cout << data[i*detrek::size1+j]<<" ";
		}
		cout <<endl;
	}
}

void detrek::cutCenterBlank()
{
	int imageSizeWithoutBlank = detrek::size1 * (detrek::size2 - 17);
	int blankSize = 17 * detrek::size1;
	int imageBlock = imageSizeWithoutBlank/2;
	int32_t * dataWithoutStrip = new int32_t [imageSizeWithoutBlank];	
	for ( int i=0; i<imageBlock; i++)
	{
		dataWithoutStrip[i] = data[i];
		dataWithoutStrip[i+imageBlock] = data[i+imageBlock+blankSize];
	}

//	memcpy(dataWithoutStrip, data, imageBlock*4);
//	memcpy(dataWithoutStrip[imageBlock], data[imageBlock+17*detrek::size1], imageBlock*4);
	delete data;
	data = dataWithoutStrip;
	detrek::size2 -= 17;
}


void detrek::maskBeamAndGap()
{
	mask = new bool [detrek::imageSize];
	for (int i=0 ;i<detrek::imageSize; i++)
	{
		mask[i] = false;
	}

	for (int i=0; i<detrek::size2; i++)
	for (int j=0; j<detrek::size1; j++)
	{
		if(data[i*detrek::size1+j] <3)
		{
			mask[i*detrek::size1+j] = true;
		}
	}
//	for (int i=0; i<detrek::size2; i++)
//	{
//		for (int j=0; j<detrek::size1; j++)
//		{
//		cout<<	mask[i*detrek::size1+j] <<" ";
//		}
//		cout<<endl;
//	}

//	int startRow = (detrek::size2 - 17)/2 - 1;
//	int endRow = startRow + 17;
	
//	for (int i=startRow; i<endRow; i++)
//	for (int j=0; j<detrek::size2; j++)
//	{
//		mask[i*detrek::size1+j] = true;
//	}	
	
//	vector<vector<int>> beamStop;
//	vector<int> tmp(2,0);
//	for (int i=0; i<(startRow+1); i++)
//	{
///		float std1 = 0;
//		float std2 = 0;
//		int startPoint = 0;
//		int endPoint = 0;
//		for (int j=0; j<detrek::size1 - 5; j++)
//		{
//			int pos = i*detrek::size1+j;
//			int x0 = data[pos];
//			int x1 = data[pos+1];
//			int x2 = data[pos+2];
//			int x3 = data[pos+3];
//			int x4 = data[pos+4];
//			int x5 = data[pos+5];
//			float mean = (x0 + x1 + x2 + x3 + x4 + x5)/5;
		//	float std = ((x0-mean) + (x1-mean) + (x2-mean))/3;
			
		//	if( std>std1) 
		//	{
		//		std1=std;
		//		if( (abs(x2 -x1) > abs(x1-x0))&& (x2<5) && x3<5 && x4<5 &&x5<5)
		//		{
		//			startPoint = pos+2;
		//		}
		//		if ((abs(x2-x1) < abs(x1-x0))&&(x0<5))	
		//		{
		//		 	endPoint = pos;
		//		}
		//	}
		///	if(std1>std2)
		//	{
		//		std=std2;
		//		std2=std1;
		//		std1=std;
		//	}
				
		//	tmp[0] = startPoint;
		//	tmp[1] = endPoint;
	//	}
		
//			if ( mean<3 && x0>5 && x1<5&&x2<5&&x3<5&&x4<5&&x4<5)
//			{
//				tmp[0] = pos;
//			}
//			if (mean<3 && x0<5 && x1<5&&x2<5&&x3<5&&x4<5&&x4>5)
//			{
//				tmp[1] = pos+4;
//			}	
//		}
//		beamStop.push_back(tmp);
//		tmp[0]=tmp[1]=0;
//	}
	
//	for(int i=0; i<detrek::size1; i++)
//	{
//	//	cout<<data[i]<< " ";
//	}
//	cout<<endl;

		
//	for (int i=0; i<beamStop.size(); i++)
//	{
//		cout<<beamStop[i][0]<<" "<<beamStop[i][1]<<endl;
//	}
//	cout<<beamStop.size()<<endl;
}

float detrek::calculateDValue(int numOfpixels)
{
	float d=0.0;
	double theta = 0.0;
	theta = atan(numOfpixels * detrek::pixelSizeX / detrek::distance)/2;
	d = detrek::scanWaveLength / (2 * sin(theta));
	return d;
}

void detrek::convert()
{
	//int maxRadius = round(sqrt(beamX*beamX+beamY*beamY));
	int maxRadius = 0;
	beamX > beamY ? maxRadius = ceil(beamX): maxRadius = ceil(beamY);
	cout << maxRadius<<endl;
	int r = 0;
	vector<vector<float>> oneDimData(maxRadius+1,vector<float>(2,0));

	for (int i=0; i<detrek::size2; i++)
	for (int j=0; j<detrek::size1; j++)
	{
		r = round(sqrt((j-detrek::beamX)*(j-detrek::beamX) + (i-detrek::beamY) * (i-detrek::beamY)));
		if (r <= maxRadius && r>5 && !mask[i*detrek::size1+j])
		{
			oneDimData[r][0] += data[i*detrek::size1+j];
			oneDimData[r][1] += 1;
		}
	
	}
	
	for (int i=0; i<oneDimData.size(); i++)
	{
                if(oneDimData[i][1]>0)
                {
		    oneDimData[i][0] /= oneDimData[i][1];
                }

	} 
	
	for (int i=0; i<oneDimData.size(); i++)
	{	
		cout<<1/detrek::calculateDValue(i)<< "\t"<<oneDimData[i][0]<< endl;
	}
	cout<<endl;
}

void detrek::writeTifImage(string outfile)
{
	TIFF * tif = TIFFOpen(outfile.c_str(), "w");
	if (!tif)
	{
		fprintf(stderr, "cannot open a tif file.");
	}
	
    TIFFSetField(tif,TIFFTAG_IMAGEWIDTH,detrek::size1);
    TIFFSetField(tif,TIFFTAG_IMAGELENGTH,detrek::size2);
    TIFFSetField(tif,TIFFTAG_SAMPLESPERPIXEL,1);
    TIFFSetField(tif,TIFFTAG_BITSPERSAMPLE,32);
    TIFFSetField(tif,TIFFTAG_ORIENTATION,ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    TIFFWriteEncodedStrip(tif, 0, data, detrek::size1*detrek::size2*4);             
    TIFFClose(tif);
}


