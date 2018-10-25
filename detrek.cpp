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
cout<<"Was data compressed? "<<detrek::compression<<endl;
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
	int oriImageSize = detrek::size1 * detrek::size2;
	int imageSize = detrek::size1 * (detrek::size2 - 17);
	int blankSize = 17 * detrek::size1;
	int imageBlock = imageSize/2;
	int32_t * dataWithoutStrip = new int32_t [imageSize];	
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


