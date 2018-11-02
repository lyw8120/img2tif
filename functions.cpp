/** @author Yao-Wang Li
  * @date 2018-10-26
  * @email: liyaowang2911@gmail.com
  * @version 0.1
  * @brief functions for image data processing 
 */

#include "functions.h"


void slope(vector<float,2> in, vector<float> out)
{
	
	for (int i=0; i<in.size()-1; i++)
	{
		out[i] = (in[i+1][1] - in[i][1])/(in[i+1][0]-in[i][0]); 
	}
}



