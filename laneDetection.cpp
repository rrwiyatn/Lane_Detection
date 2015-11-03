#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string.h>
#include <sstream>

using namespace std;

//Declare Constants
const int ImageWidth=640;
const int ImageHeight=480;
const int threshold_value=255;

//Declare Arrays
int Image[ImageWidth*ImageHeight]; //uint8_t would be more efficient here

//Declare Functions
void readImage(char* FileName);
void convolve(int Image[], int filter[], const int ImageWidth, const int ImageHeight, const int threshold);
void threshold(int Image[], const int ImageWidth, const int ImageHeigth, const int threshold_value);
void writeImage(int k, std::string s);

//Filters
//replace with the correct filter
int filter[32]= {*, *, *, *, *, *, *, *,
        		 *, *, *, *, *, *, *, *,
				 *, *, *, *, *, *, *, *,
				 *, *, *, *, *, *, *, *};
//

int main ()
{
	//replace i with # of frames need to be processed
	for(int i=700;i<710;i++)
	{
		char input[50]={};
		std::string s="";
		std::stringstream out;
		out<<i;
		s=out.str();
		char *FileName=new char[s.length()+1];
		strcpy(FileName, s.c_str());
		char *format=".ppm";
		strncpy(input,FileName,sizeof(input));
		strncat(input,format,sizeof(input));
		readImage(input);
		convolve(Image, filter, ImageWidth, ImageHeight, threshold_value);
		writeImage(i,s);
	}
	return 0;
}

void readImage(char* FileName)
{
	int c;
	int c1;
	int c2;
	int c3;

	ifstream ImageIn (FileName, ios::binary);
	for(int i=0;i<38;i++)
			c = ImageIn.get();

	for(int i=0;i<ImageHeight*ImageWidth;i++)
	{
		//for color image
		c1 = int((ImageIn.get())*0.299);//R
		c2 = int((ImageIn.get())*0.587);//G
		c3 = int((ImageIn.get())*0.114);//B
		c = c1+c2+c3;//=grayscale weighted average

		/*//for grayscale image
		c1 = int(ImageIn.get());//R
		c2 = int(ImageIn.get());//G
		c3 = int(ImageIn.get());//B
		c = (c1+c2+c3)/3;
		*/

		Image[i]=c;
	}
	ImageIn.close();
	return;
}

void convolve(int Image[], int filter[], const int ImageWidth, const int ImageHeight, const int threshold_value)
{
	int sum = 0;
	int max = 0;
	for(int i=0;i<ImageHeight;i=i+4)
	{
		for(int j=0;j<ImageWidth;j=j+8)
	    {
			sum = 0;
			//WEIGHTED SUM
			for(int k=0;k<4;k++)
			{
				for(int l=0;l<8;l++)
				{
					sum += Image[(ImageWidth*i)+j+(k*ImageWidth)+l]*filter[k*8+l];
				}
			}
			if(max<abs(sum))
			{
				max=abs(sum);
			}
			for(int k=0;k<4;k++)
			{
				for(int l=0;l<8;l++)
				{
					Image[(ImageWidth*i)+j+(k*ImageWidth)+l]=sum;
				}
			}
	    }
	}
	//threshold the filtered image
	threshold(Image,ImageWidth,ImageHeight,threshold_value);
	return;
}

void threshold(int Image[], const int ImageWidth, const int ImageHeigth, const int threshold_value)
{
	for(int i=0;i<(ImageWidth*ImageHeight);i++)
	{
		if(Image[i]<threshold_value)
	    {
			Image[i]=0;
	    }
	    else
	    {
	    	Image[i]=255;
	    }
	}
	return;
}

void writeImage(int k, std::string s)
{
	char num;
	char *FileOut=new char[s.length()+1];
	strcpy(FileOut, s.c_str());
	char *format="_out.ppm";
	char output[50];
	strncpy(output,FileOut,sizeof(output));
	strncat(output,format,sizeof(output));
	ofstream ImageOut (output, ios::binary);
	ImageOut << "P6" << endl << ImageWidth << " " << ImageHeight << " 255" << endl;

	for(int i=0;i<ImageHeight*ImageWidth;i++)
	{
		for(int j=0;j<3;j++)
		{
			num = Image[i];
			ImageOut << num;
		}
	}
	ImageOut.close();
	return;
}
