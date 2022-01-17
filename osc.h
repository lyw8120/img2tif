/**
 * author: Dr. Yaowang Li
 * date: Jan. 11, 2022
 * @brief osc.h defines the data structure for reading rigaku file format "osc".
 * osc image file has 1400 chars length. 
 *
 */


#ifndef OSC_H_
#define OSC_H_

#include <iostream>

struct osc {
    char rigakuName[10];        //what kind of instrument
    char version[10];           //which version 
    char crystalName[20];       //crystal name
    char crystalSystem[12];     //crystal system
    float alength;              //unit cell a length
    float blength;              //unit cell b length
    float clength;              //unit cell c length
    float alpha;                //unit cell alpha angle
    float beta;                 //unit cell beta angle
    float gamma;                //unit cell gamma angle
    char spaceGroupSymbol[12];  //space group symbol
    float mosaic;               //mosaic spread
    float memoComments[80];     //memo comments
    char reservedSpace[84];     //reserved space
    char date[12];              // date of measurement
    char userName[20];          //username when collecting image
    char xraySource[4];         // like, Cu, Mo 
    float waveLength;           // wave length
    char typeMonochromator[20]; //type of monochromator
    float m2theta;              //monochromator two theta
    char collimator[20];        //collimator size and type
    char filter[4];             //filter type, like Ni
    float cammera
};
#endif

