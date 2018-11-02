/** @author Yao-Wang Li
  * @date 2018-10-26
  * @email: liyaowang2911@gmail.com
  * @version 0.1
  * @brief functions for image data processing 
 */

#include "functions.h"


void slope(const vector<vector<float>> &in, vector<float> & out)
{	
      int n=in.size() -1 ;
      vector<float> tmp (n,0);

	for (int i=0; i<n; i++)
	{
		tmp[i] = (in[i+1][1] - in[i][1])/double(in[i+1][0]-in[i][0]); 
                cout << tmp[i]<<endl;
	}
        out = tmp;
}



