// Copyright 2005 Mitsubishi Electric Research Laboratories All Rights Reserved.

// Permission to use, copy and modify this software and its documentation without
// fee for educational, research and non-profit purposes, is hereby granted, provided
// that the above copyright notice and the following three paragraphs appear in all copies.

// To request permission to incorporate this software into commercial products contact:
// Vice President of Marketing and Business Development;
// Mitsubishi Electric Research Laboratories (MERL), 201 Broadway, Cambridge, MA 02139 or 
// <license@merl.com>.

// IN NO EVENT SHALL MERL BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND
// ITS DOCUMENTATION, EVEN IF MERL HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// MERL SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED
// HEREUNDER IS ON AN "AS IS" BASIS, AND MERL HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
// UPDATES, ENHANCEMENTS OR MODIFICATIONS.

//Modificado por Mislene Nunes

//compilar: g++ BRDFWritebin.cpp -o main
//executar: ./main "material_05.binary"

#include "stdlib.h"
#include "math.h"
#include <stdio.h>

#define BRDF_SAMPLING_RES_THETA_H       90
#define BRDF_SAMPLING_RES_THETA_D       90
#define BRDF_SAMPLING_RES_PHI_D         360

#define RED_SCALE (1.0/1500.0)
#define GREEN_SCALE (1.15/1500.0)
#define BLUE_SCALE (1.66/1500.0)
#define M_PI	3.1415926535897932384626433832795

int column = 90*90*180;

void sample_bin(char filename[100], double material_red[column], double material_green[column], double material_blue[column])
{
	printf("%d\n",column);
	printf("red: %lf\n",material_red[0]);
	printf("green: %lf\n",material_green[0]);
	printf("blue: %lf\n",material_blue[0]);
	double *brdf;	
	FILE *f = fopen(filename, "wb");
	
	int dims[3];
	dims[0] = 90;
	dims[1] = 90;
	dims[2] = 180;
	fwrite(dims, sizeof(int), 3, f);
	printf("Write: %d %d %d\n", dims[0],dims[1],dims[2]);
	int n = dims[0] * dims[1] * dims[2];
	if (n != BRDF_SAMPLING_RES_THETA_H *
		 BRDF_SAMPLING_RES_THETA_D *
		 BRDF_SAMPLING_RES_PHI_D / 2) 
	{
		printf("Dimensions don't match in write\n");
		
	}

	//FILE* sample_red = fopen(material_red,"r");
	//FILE* sample_green = fopen(material_green,"r");
	//FILE* sample_blue = fopen(material_blue,"r");
	
	brdf = (double*) malloc (sizeof(double)*3*n);
	int i;
	for(i=0;i<n;i++){
		//fscanf(sample_red, "%lf", &brdf[i]);
		brdf[i] = material_red[i];
		//fscanf(sample_green, "%lf", &brdf[i+n]);
		brdf[i+n] = material_green[i];
		//fscanf(sample_blue, "%lf", &brdf[i+(n*2)]);
		brdf[i+(n*2)] = material_blue[i];
	}

	fwrite(brdf, sizeof(double), 3*n, f);
	fclose(f);
	//fclose(sample_red);
	//fclose(sample_green);
	//fclose(sample_blue);
}

/*int main(int argc, char *argv[])
{
	const char *name = argv[1];
	char material_red[50] = "material_Red.txt";
	char material_green[50] = "material_Green.txt";
	char material_blue[50] = "material_Blue.txt";
	
	double* brdf;	
	
	sample_bin(name, brdf, material_red, material_green, material_blue);
	
	return 0;
}*/


