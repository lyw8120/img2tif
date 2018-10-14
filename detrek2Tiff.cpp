/** @author Yao-Wang Li
 * @date 2018-10-14
 * @email: liyaowang2911@gmail.com
 * @version: 0.1
 * @brief convert the rigaku image to tiff format image 
 */

#include <cstdlib>
#include "detrek.h"

int main (int argc, char ** argv)
{
	if (argc != 2)
	{
		std::cout<<"I need the detrek image file name, thank you."<<std::endl;
		exit(1);
	}

	string fileName=string(argv[1]);
	int position=fileName.find_last_of(".");
	if (fileName.substr(position+1)!="img")
	{
		std::cout<<"it is not an image with the extension 'img'."<<std::endl;
		exit(2);
	}

	string baseName=fileName.substr(0, position);
	string outFileName=baseName + ".tif";
	
	std::cout<<"This program can convert the rigaku two dimensional diffraction image to tiff format image."<<std::endl;
	std::cout<<"Have fun with this program, good luck to you. from Yao-Wang. "<<std::endl;

	detrek rigakuImg(fileName);
	rigakuImg.writeTifImage(outFileName);


	return 0}
