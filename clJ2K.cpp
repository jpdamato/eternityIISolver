#include <iostream>	// for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <omp.h>

#define OPENCV 1

#ifdef OPENCV
#include <opencv\cv.h>
#include <opencv\highgui.h>
#endif
using namespace std;

#include "data.h"
#include "cl_utils.h"
#include "encrypt_utils.h"
#include "sudokuInit.h"


Solution solution[numThreads];
State states[numThreads];
bool useGPU = false;
int numSubThreads= 1;
string pathFile;

double sft_clock(void)
{
#ifdef _WIN32
	/* _WIN32: use QueryPerformance (very accurate) */
	LARGE_INTEGER freq , t ;
	/* freq is the clock speed of the CPU */
	QueryPerformanceFrequency(&freq) ;
	/* cout << "freq = " << ((double) freq.QuadPart) << endl; */
	/* t is the high resolution performance counter (see MSDN) */
	QueryPerformanceCounter ( & t ) ;
	return ( t.QuadPart /(double) freq.QuadPart ) ;
#else
	/* Unix or Linux: use resource usage */
	struct rusage t;
	double procTime;
	/* (1) Get the rusage data structure at this moment (man getrusage) */
	getrusage(0,&t);
	/* (2) What is the elapsed time ? - CPU time = User time + System time */
	/* (2a) Get the seconds */
	procTime = t.ru_utime.tv_sec + t.ru_stime.tv_sec;
	/* (2b) More precisely! Get the microseconds part ! */
	return ( procTime + (t.ru_utime.tv_usec + t.ru_stime.tv_usec) * 1e-6 ) ;
#endif
}

#ifdef OPENCV
void showOccupancy()
{
	IplImage *img = cvCreateImage(cvSize(512,512),8,3);
	double advancement[numThreads];
	double maxA = MAX_ITERATIONS;

	if (useGPU)
		maxA = MAX_ITERATIONS / 40 ;

	for (int i=0; i<numThreads ; i++)
	{
		advancement[i] = solution[i].lastCount;		
	}

	CvPoint* points= new CvPoint[4];
	points[0] = cvPoint(0,0);
	points[1] = cvPoint(511,0);
	points[2] = cvPoint(511,511);
	points[3] = cvPoint(0,511);
	for (int i=0 ; i<numThreads; i++)
	{			
		// Rectangle
		points[0] = cvPoint((i+0)*512/ numThreads, 512  );
		points[1] = cvPoint((i+1)*512/ numThreads, 512 );
		points[2] = cvPoint((i+1)*512/ numThreads, 512 - 512 *advancement[i] /maxA );
		points[3] = cvPoint((i+0)*512/ numThreads, 512 - 512 *advancement[i] /maxA );

		cvFillConvexPoly(img,&points[0], 4,cvScalar( 120 , i *20 , i*20));
		cvRectangle(img, points[0],points[2],cvScalar(0,0,0));
	}

	cvShowImage("Occupancy",img);
	cvWaitKey(1);
	cvReleaseData(img);
}

void showAdvance()
{
	IplImage *img = cvCreateImage(cvSize(512,512),8,3);
	double advancement[numThreads];
	double maxA = 50*50*50 + 50*50*50 + 50*50 + 50;

	for (int i=0; i<numThreads ; i++)
	{
		double range0 =  solution[i].stack[3].neigh;
		double range1 =  solution[i].stack[4].neigh;
		double range2 =  solution[i].stack[5].neigh;
		double range3 =  solution[i].stack[6].neigh;
		double range4 =  solution[i].stack[7].neigh;

		advancement[i] = range0 * 50*50*50*50 + range1 *50*50*50 + range2*50*50 + range3*50+range4 ;
		maxA = max(maxA , advancement[i]);
	}

	CvPoint* points= new CvPoint[4];
	points[0] = cvPoint(0,0);
	points[1] = cvPoint(511,0);
	points[2] = cvPoint(511,511);
	points[3] = cvPoint(0,511);
	for (int i=0 ; i<numThreads; i++)
	{			
		// Rectangle
		points[0] = cvPoint((i+0)*512/ numThreads, 512  );
		points[1] = cvPoint((i+1)*512/ numThreads, 512 );
		points[2] = cvPoint((i+1)*512/ numThreads, 512 *advancement[i] /maxA );
		points[3] = cvPoint((i+0)*512/ numThreads, 512 *advancement[i] /maxA );

		cvFillConvexPoly(img,&points[0], 4,cvScalar( i *20 , 200,200));
		cvRectangle(img, points[0],points[2],cvScalar(0,0,0));
	}

	cvShowImage("Advancement",img);

	cvReleaseData(img);


}

void showImage(char* fichas, int offset, int count)
{
	IplImage *img = cvCreateImage(cvSize(512,512),8,3);

	CvPoint* points= new CvPoint[16];
	points[0] = cvPoint(0,0);
	points[1] = cvPoint(511,0);
	points[2] = cvPoint(511,511);
	points[3] = cvPoint(0,511);
	cvFillConvexPoly(img,&points[0], 4,cvScalar(255,255,255));

	for (int i=0 ; i<16; i++)
		for (int j=0 ; j<16; j++)
		{			
			// Rectangle
			points[0] = cvPoint(i*32,j*32);
			points[1] = cvPoint((i+1)*32*32,j*32);
			points[2] = cvPoint((i+1)*32,(j+1)*32);
			points[3] = cvPoint((i+0)*32,(j+1)*32);
			// Up triangle
			points[4] = cvPoint((i+0)*32,(j)*32);
			points[5] = cvPoint((i+1)*32,(j)*32);
			points[6] = cvPoint(i*32+16,j*32+16);
			// down triangle
			points[7] = cvPoint((i+0)*32,(j+1)*32);
			points[8] = cvPoint((i+1)*32,(j+1)*32);
			points[9] = cvPoint(i*32+16,j*32+16);
			// left  triangle
			points[10] = cvPoint((i+0)*32,(j)*32);
			points[11] = cvPoint((i)*32,(j+1)*32);
			points[12] = cvPoint(i*32+16,j*32+16);

			// right  triangle
			points[13] = cvPoint((i+1)*32,(j)*32);
			points[14] = cvPoint((i+1)*32,(j+1)*32);
			points[15] = cvPoint(i*32+16,j*32+16);

			int np = 4;			
			int index = i*16 + j;
			int fU = fichas[index*offset];
			int fD = fichas[index*offset+2];
			int fR = fichas[index*offset+1];
			int fL = fichas[index*offset+3];
			if (fU<0) continue;
			if (fD<0) continue;
			if (fR<0) continue;
			if (fL<0) continue;
			//	cvFillConvexPoly(img,&points[0], 4,cvScalar(i*8,j*8,0));
			cvFillConvexPoly(img,&points[4], 3,cvScalar(colors[fU*3],colors[fU*3+1], colors[fU*3+2]));
			cvFillConvexPoly(img,&points[7], 3,cvScalar(colors[fD*3],colors[fD*3+1], colors[fD*3+2]));
			cvFillConvexPoly(img,&points[10], 3,cvScalar(colors[fL*3],colors[fL*3+1], colors[fL*3+2]));
			cvFillConvexPoly(img,&points[13], 3,cvScalar(colors[fR*3],colors[fR*3+1], colors[fR*3+2]));
			cvRectangle(img, points[0],points[2],cvScalar(0,0,0));
		}

		cvShowImage("Tablero",img);
		cvWaitKey(-1);
		cvReleaseData(img);
}

void showImageSudoku(char* fichas,char* initialTab, int offset, int count)
{
	IplImage *img = cvCreateImage(cvSize(512,512),8,3);
	int size = 512 / MAX_SUDOKU ;
	CvPoint* points= new CvPoint[16];
	points[0] = cvPoint(0,0);
	points[1] = cvPoint(511,0);
	points[2] = cvPoint(511,511);
	points[3] = cvPoint(0,511);
	cvFillConvexPoly(img,&points[0], 4,cvScalar(255,255,255));
	char conv[10];
	CvFont font;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);



	for (int i=0 ; i<MAX_SUDOKU; i++)
		for (int j=0 ; j<MAX_SUDOKU; j++)
		{			
			// Rectangle
			points[0] = cvPoint(i*size,j*size);
			points[1] = cvPoint((i+1)*size*size,j*size);
			points[2] = cvPoint((i+1)*size,(j+1)*size);
			points[3] = cvPoint((i+0)*size,(j+1)*size);
			int index = j*MAX_SUDOKU + i;
			int fU = fichas[index];
			itoa( fU,conv,10);		   
			std::string si(conv); 
			int nP = 4;
			int isFixed = initialTab[index];

			cvFillConvexPoly(img,&points[0], 4,cvScalar(220,220,150 + index));

			cvDrawRect(img, points[0],points[2],cvScalar(20,20,20));
			if (isFixed >=0)
				cvPutText(img , si.c_str(),cvPoint( i*size + 8 ,j*size + 12), &font , cvScalar(0,0,255));
			else
				cvPutText(img , si.c_str(),cvPoint( i*size + 8 ,j*size + 12), &font , cvScalar(205,125,125));

		}

		cvShowImage("Tablero",img);
		cvWaitKey(-1);
		cvReleaseData(img);
}
#endif

void resetState()
{
	for (int i=0; i<numThreads ; i++)
	{
		solution[i].index = 0;
		for (int j=0 ; j<512 * 4 ; j++)	  solution[i].tablero[j] = -1;
		for (int j=0 ; j<256  ; j++)	  solution[i].stack[j].elementID = 0;
		for (int j=0 ; j<256  ; j++)	  solution[i].fUsed[j] = 0;
		for (int j=0 ; j<256  ; j++)	  solution[i].result[j] = 0;
	}
}

void saveState()
{
	/* C style */
	FILE* file2 = fopen("ouputState.bin", "wb");
	unsigned int sz =  sizeof(Solution)  ;  
	for (int i=0; i<numThreads ; i++)
	{	  
		fwrite(&solution[i],sz,1,file2);	  
	}

	fclose(file2);
}

void restoreState()
{
	/* C style */
	FILE* file2 = fopen("ouputState.bin", "r");
	unsigned int sz =  sizeof(Solution)  ;  
	for (int i=0; i<numThreads ; i++)
	{
		fseek(file2 , sz*i,0);
		fread(&solution[i],sz, 1, file2);	
	}
	fclose(file2);
}

int isBorder(char* ficha)
{
	return (!ficha[0]) || (!ficha[1])  || (!ficha[2]) || (!ficha[3]) ;

}

char* initFichasBordes()
{
	char* fichas = new char[256 * 5];
	int index = 0;
	int zeroCounter;
	for (int i=0 ; i<256 ; i++)
	{
		zeroCounter = 0;
		if ( !gbFichas[i*4] ) zeroCounter++;
		if ( !gbFichas[i*4+1] ) zeroCounter++;
		if ( !gbFichas[i*4+2] ) zeroCounter++;
		if ( !gbFichas[i*4+3] ) zeroCounter++;

		if (zeroCounter>0)
		{
			fichas[index*5] = gbFichas[i*4];
			fichas[index*5+1] = gbFichas[i*4+1];
			fichas[index*5+2] = gbFichas[i*4+2];
			fichas[index*5+3] = gbFichas[i*4+3];
			fichas[index*5+4] = index ; 
		}
		index++;
	}
	return fichas;
}

void getPosInBorder(int index, int *pX, int *pY)
{
	if ((index>=0) && (index < 14))
	{
		*pX = index+1;
		*pY = 0;
	}
	else
		if ((index>=14) && (index < 28))
		{
			*pX = 15;
			*pY = index - 14 + 1;
		}
		else
			if ((index>=28) && (index < 42))
			{
				*pX = 42-index;
				*pY = 15;
			}
			else
			{
				*pX = 0;
				*pY = 56 - index ;
			}

}


void computePosFromCenter(char* tablero, short* posFromCenter)
{
	int px =7 , py =8, index = 1;
	tablero[ py*16 + px] = 1;
	px=7; py=7;
	posFromCenter[0] = 7;
	posFromCenter[1] = 8;
	while (index<256)	
	{

		//-- Right
		while ((index<256) && (tablero[ (py+1)*16 + px] >0) && (px<16))
		{
			tablero[ py*16 + px] = 1;
			posFromCenter[2*index] = px;
			posFromCenter[2*index+1] = py;
			px++;
			index++;
		}

		// Down		
		while ((index<256) && (tablero[ py*16 + px-1] >0) && (px<16)&& (py<16))
		{
			tablero[py*16 + px] = 1;
			posFromCenter[2*index] = px;
			posFromCenter[2*index+1] = py;
			py++;
			index++;
		}
		// Left	
		while ((index<256) && (tablero[ (py-1)*16 + px] >0) && (px>=0)&& (py>=0))
		{
			tablero[py*16 + px] = 1;
			posFromCenter[2*index] = px;
			posFromCenter[2*index+1] = py;
			px--;
			index++;
		}
		//Up	
		if (px==16) {py=15 ; px=0;}
		while ((index<256) && (tablero[ py*16 + px+1] >0) && (py>=0))
		{
			tablero[py*16 + px] = 1;
			posFromCenter[2*index] = px;
			posFromCenter[2*index+1] = py;
			py--;
			index++;
		}	
		//py--;
		//fprintf(stderr, "Indice \n",index);
	}
}



unsigned int found = 0;
unsigned int acumfound = 0;
unsigned int nTests = 0;
int iter = 0;
int maxIndex = 50;

//-------------------------------------
//-- Finished Threads take some process
//-------------------------------------
void reallocateThreads(int startIndex)
{

	//------------------------------
	int i = 0;
	while (i<numThreads)
	{
		// Si el thread ya finalizo
		if (solution[i].finished)
		{
			int bestThread = -1;
			int maxtoProcess = 1, toProcess;
			// Busco el mejor thread para arrancar
			for (int j = 0; j<numThreads ; j++)
			{
				if (solution[j].finished) continue;
				// Leo en que posicion esta el stack del segundo nivel
				toProcess = solution[j].stack[startIndex].to - solution[j].stack[startIndex].neigh;

				if (toProcess > maxtoProcess )
				{
					bestThread = j;
					maxtoProcess = toProcess;

				}
			}
			// No se encontro un solcuion
			if (bestThread<0)
			{
				startIndex++;
				continue;

			}
			//- Modifico la pila de ejecucion del mejor thread
			// para que el thread original arranque en el proximo
			toProcess = (solution[bestThread].stack[startIndex].to - solution[bestThread].stack[startIndex].neigh) ; 


			// Copio los estados anteriores
			solution[i].index = startIndex;
			for (int j = 0; j< startIndex ; j++)
				solution[i].stack[j] = solution[bestThread].stack[j];


			solution[i].stack[startIndex].neigh = solution[bestThread].stack[startIndex].neigh + toProcess/2;
			solution[i].stack[startIndex].to = solution[bestThread].stack[startIndex].neigh + toProcess;
			solution[i].stack[startIndex].rot = 0;			
			solution[i].finished = false;
			//fprintf(stderr, "Reallocated Thread %d  -> %d \n",solution[i].thID, solution[bestThread].thID );		
			solution[bestThread].stack[startIndex].to = solution[bestThread].stack[startIndex].neigh + toProcess/2 ;

			if (solution[bestThread].stack[startIndex].to == solution[i].stack[startIndex].neigh)
				  solution[i].stack[startIndex].neigh = solution[i].stack[startIndex].neigh+1;

		}
		i++;
	}

}
//#define RECURSIVE 1

void fillFromCenter(char* tablero)
{
	cout<< "\n--------------------------------------------------------------------------" << endl;
	cout<< " Running on CPU  " << endl;
	cout<< "\n--------------------------------------------------------------------------" << endl;

	//short posFromCenter[256 * 2];
	short neighBours[256 * 200];

	//Reset Structure
	for (int i=0;i<256*4;i++) tablero[i] = -1;

	computePosFromCenter(tablero,gbposFromCenter);

	//short borderpatternCounts[25];	
	for (int i=0; i<60*4;i++)
		gbborderpatternCounts[gbFichas[i]]++;	

	//Reset Structure
	for (int i=0;i<256*4;i++) tablero[i] = -1;

	// showImage(tablero, 4, 0);  
	for (int j=0; j<numThreads ; j++)
	{   
		solution[j].nThreads = numThreads;

		//initFichas(solution[j], posFromCenter);
	}


#if defined RECURSIVE
	omp_set_num_threads(1);
#pragma omp parallel
	for (int j=0; j<8 ; j++)
		solution[j]->rfill(solution[j]->tablero, solution[j]->fichas, 1);
#else
	omp_set_num_threads(numThreads);
	computeNeighBours(neighBours);

#pragma omp parallel for
	for (int j=0; j<numThreads ; j++)
	{
		//  activeThreadID = j;
		startArray(solution , gbposFromCenter);	  

	}

	int threadFinished = 0;
	double meanTimeBetweenSolution = 0;
	int numIterations = 0;
	while (threadFinished < numThreads)
	{
		double first = sft_clock();    
#pragma omp parallel for
		for (int j=0; j<numThreads ; j++)
		{
			//	activeThreadID = j;
			ifill( &solution[0] , &states[0], neighBours,gbposFromCenter, gbborderpatternCounts);
		}

		cout   << "-----------------------------------" << endl;
		cout   << " Nothing FOUND by now \n" << endl;
		cout   << "-----------------------------------" << endl;
		printf("Required %f secs to Find  . \n ", sft_clock() - first);	
		numIterations ++;
		int maxIndex = 0;
		threadFinished = 0;
		for (int j=0; j<numThreads ; j++)
		{
			if (solution[j].maxIndex> maxIndex)
				maxIndex = solution[j].maxIndex;
			/*  if (solution[j]->maxIndex > 90)
			showImage(solution[j]->tablero,4,0);
			*/
			if (solution[j].finished)
			{
				if (threadFinished == 0) 
					fprintf(stderr, "Thread finished %d  ",solution[j].thID );		
				else
					fprintf(stderr, " , %d  ",solution[j].thID );		

				threadFinished++;
			}
		}
		fprintf(stderr, " \n");		 
		if (numIterations % 500 == 0)
		{
#ifdef OPENCV
			showAdvance();
			showOccupancy();
#endif
			// saveState();
		}

		reallocateThreads(2);
		/*
		resetState();
		restoreState();
		*/
		fprintf(stderr, "Max Depth %d  iteration %d \n",maxIndex,numIterations );

	}

#endif	  
	cout   << " Nothing FOUND \n" << endl;

}

void GPUEternity(char* tablero)
{
	int iclError;
	cout<< "\n--------------------------------------------------------------------------" << endl;
	cout<< " Running on GPU  " << endl;
	cout<< "\n--------------------------------------------------------------------------" << endl;

	short neighBours[256 * 200];
	short posFromCenter[256 * 2];
	//Reset Structure
	for (int i=0;i<256*4;i++) tablero[i] = -1;	
	computePosFromCenter(tablero,posFromCenter);

	short borderpatternCounts[25];	
	for (int i=0; i<25;i++)
		borderpatternCounts[i] = 0 ;	
	for (int i=0; i<60*4;i++)
		borderpatternCounts[gbFichas[i]]++;	
	//Reset Structure
	for (int i=0;i<256*4;i++) tablero[i] = -1;

	computeNeighBours(neighBours);


	string clFile = pathFile + "/clEternity.cl";
	string clCopyFile = pathFile + "/clEternity_copy.cl";

	//keysReplacement("clEternity.cl","$NEIGHBOUR",shortToString(neighBours , 256*200));
	keysReplacement( (char*)clFile.c_str(),"$POSFROMCENTER",shortToString(posFromCenter , 256*2),
		(char*)clCopyFile.c_str());

	keysReplacement(  (char*)clCopyFile.c_str(),"$BORDER",shortToString(borderpatternCounts , 25),
		(char*)clCopyFile.c_str());

	initGPU();
	initGPUProgram( (char*)clCopyFile.c_str(),"startArray");

	// Data
	cl_mem clSolutions = getDeviceBufferC(sizeof(Solution)*numThreads);
	cl_mem clStates = getDeviceBufferC(sizeof(State)*numThreads);
	cl_mem clPosFromCenter = getDeviceBufferC(sizeof(short)*256 * 2);
	cl_mem clNeighBours = getDeviceBufferC(sizeof(short)*256 * 200);
	cl_mem clBorderPatternCountes = getDeviceBufferC(sizeof(short)*25);
	// Kernels
	cl_kernel clkInitialization = initKernel("startArray");
	cl_kernel clkFind = initKernel("ifill");

	for (int j=0; j<numThreads ; j++)
	{     
		solution[j].nThreads = numThreads;		
	}

	// Send Data
	iclError = clEnqueueWriteBuffer(getDefaultCommandQueue(), clSolutions, CL_FALSE, 0,  sizeof(Solution)*numThreads, &solution[0], 0, NULL, NULL);	
	iclError = clEnqueueWriteBuffer(getDefaultCommandQueue(), clStates, CL_FALSE, 0,  sizeof(State)*numThreads, &states[0], 0, NULL, NULL);	
	iclError = clEnqueueWriteBuffer(getDefaultCommandQueue(), clPosFromCenter, CL_FALSE, 0,  sizeof(short)*256*2, posFromCenter, 0, NULL, NULL);	
	iclError = clEnqueueWriteBuffer(getDefaultCommandQueue(), clBorderPatternCountes, CL_FALSE, 0,  sizeof(short)*25, borderpatternCounts, 0, NULL, NULL);	
	iclError = clEnqueueWriteBuffer(getDefaultCommandQueue(), clNeighBours, CL_FALSE, 0,  sizeof(short)*256*200, neighBours, 0, NULL, NULL);	

	//--- Init
	iclError = clSetKernelArg(clkInitialization, 0, sizeof(cl_mem), (void*)&clSolutions);	
	iclError |= clSetKernelArg(clkInitialization, 1, sizeof(cl_mem), (void*)&clPosFromCenter);		

	uint lW = 1;	
	uint gM = numThreads;
	// Send Data and Read
	iclError |= clEnqueueNDRangeKernel(getDefaultCommandQueue(), clkInitialization, 1, NULL, &gM, &lW, 0, NULL, NULL);	
	iclError |= clFinish(getDefaultCommandQueue());		
	iclError |= clEnqueueReadBuffer(getDefaultCommandQueue(), clSolutions, CL_TRUE, 0, sizeof(Solution)*numThreads, solution, 0, NULL, NULL);

	//--- Find the solution
	int threadFinished = 0;
	//gM = 48;
	lW = numSubThreads;
	int numIterations = 0;
	while (threadFinished < numThreads)
	{
		double first = sft_clock();    
		iclError = clSetKernelArg(clkFind, 0, sizeof(cl_mem), (void*)&clSolutions);	
		iclError = clSetKernelArg(clkFind, 1, sizeof(cl_mem), (void*)&clStates);	
		iclError = clSetKernelArg(clkFind, 2, sizeof(cl_mem), (void*)&clNeighBours);	
		iclError = clSetKernelArg(clkFind, 3, sizeof(cl_mem), (void*)&clPosFromCenter);	
		iclError = clSetKernelArg(clkFind, 4, sizeof(cl_mem), (void*)&clBorderPatternCountes);	
		// Send Data and Read
		iclError |= clEnqueueNDRangeKernel(getDefaultCommandQueue(), clkFind, 1, NULL, &gM, &lW, 0, NULL, NULL);	
		iclError |= clFinish(getDefaultCommandQueue());		

		printf("Required %f secs to Find  . \n ", sft_clock() - first);	

		cout   << "-----------------------------------" << endl;
		cout   << " Nothing FOUND by now \n" << endl;
		cout   << "-----------------------------------" << endl;

		iclError |= clEnqueueReadBuffer(getDefaultCommandQueue(), clStates, CL_TRUE, 0, sizeof(State)*numThreads, states, 0, NULL, NULL);
		//  iclError |= clEnqueueReadBuffer(getDefaultCommandQueue(), clSolutions, CL_TRUE, 0, sizeof(Solution)*numThreads, solution, 0, NULL, NULL);

		int maxIndex = 0;
		threadFinished = 0;
		numIterations ++ ;
		for (int j=0; j<numThreads ; j++)
		{
			if (states[j].maxIndex> maxIndex)
				maxIndex = states[j].maxIndex;
			//  if (states[j].maxIndex > 100)
			//	   showImage(solution[j].tablero,4,0);

			if (states[j].finished)
			{
				//fprintf(stderr, "Thread finished %d  \n",states[j].thID );		
				threadFinished++;
			}
		}
		if (numIterations % 10 == 0)
		{
#ifdef OPENCV
			showAdvance();
			showOccupancy();
#endif
			// saveState();
		}
		fprintf(stderr, "Thread finished %d  \n",threadFinished );		

		if (threadFinished )
		{
			iclError = clEnqueueReadBuffer(getDefaultCommandQueue(), clSolutions, CL_TRUE, 0, sizeof(Solution)*numThreads, solution, 0, NULL, NULL);
			reallocateThreads(2);
			iclError |= clEnqueueWriteBuffer(getDefaultCommandQueue(), clSolutions, CL_FALSE, 0,  sizeof(Solution)*numThreads, &solution[0], 0, NULL, NULL);	
		}
		/*
		saveState();
		resetState();
		restoreState();
		*/
		fprintf(stderr, "Max Depth %d  \n",maxIndex );

	}



}

void help()
{
	cout
		<< "\n--------------------------------------------------------------------------" << endl
		<< "This program shows how to read a video file with OpenCV. In addition, it tests the"
		<< " similarity of two input videos first with PSNR, and for the frames below a PSNR "  << endl
		<< "trigger value, also with MSSIM."<< endl
		<< "Usage:"                                                                       << endl
		<< "./video-source referenceVideo useCaseTestVideo PSNR_Trigger_Value Wait_Between_Frames " << endl
		<< "--------------------------------------------------------------------------"   << endl
		<< endl;
}

void parseCommandLineArguments(int argc, char *argv[])
{
	char* pathF = NULL;


	if (argc >= 1) 
	{
		for (int i=1; i < argc; i++) 
		{
			int bFirstArgIsParam = false;
			int string_start = 0;
			while (argv[i][string_start] == '-')
				string_start++;
			char *string_argv = &argv[i][string_start];

			if (!stricmp(string_argv, "help")) {
				help();
				exit(0);
			}

			if (!strnicmp(string_argv, "threads",7)) {
				numSubThreads = atoi(&string_argv[8]);
				continue;
			}

			if (!stricmp(string_argv, "GPU")) {
				useGPU = true;
				continue;
			}

			if (!strnicmp(string_argv, "path",4)) {
				pathF = &string_argv[5];
				continue;
			}


		}
	}

	if (pathF)
	{
		pathFile = pathF;
	}
	else
	{
		string filename(argv[0]);

		const size_t last_slash_idx = filename.rfind('\\');
		if (std::string::npos != last_slash_idx)
		{
			pathFile = filename.substr(0, last_slash_idx);
		}
		else
			pathFile = "";
		string cpth(pathFile);
		pathFile = "";

		for (int i=0 ; i<cpth.length() ; i++)
			if (cpth[i] == '\\') 
				pathFile += '/';
			else
				pathFile+= cpth[i];

	}
	cout<< "Path File " << pathFile <<"\n";
}


void sudokuCPU()
{
	char* tablero = new char[MAX_SUDOKU * MAX_SUDOKU];
	for (int i=0 ; i<MAX_SUDOKU * MAX_SUDOKU ; i++)	 tablero[i] = -1;
	init16x16(tablero);

	int from = 1 , step = MAX_SUDOKU / numThreads;

	for (int j = 0 ; j< numThreads ; j++)
		for (int i=0 ; i<MAX_SUDOKU * MAX_SUDOKU ; i++)	
		{
			
			solution[j].index = 0;
			solution[j].finished = false; // (j != 0);
			solution[j].tablero[i] = tablero[i];

			if (tablero[i] >=0)
			{
				solution[j].stack[i].neigh = tablero[i];
			    solution[j].stack[i].to = tablero[i];
			}
			else
			{
				if (i == 0)
				{
					 solution[j].stack[i].neigh = from ;
			         solution[j].stack[i].to = from + step-1;

					 from = from + step ; 
				}
				else
				{
				  solution[j].stack[i].neigh = 1;
			      solution[j].stack[i].to = MAX_SUDOKU;
				}
			}
		}

		//init9x9(tablero);


		

		int threadFinished = 0;
		double meanTimeBetweenSolution = 0;
		int numIterations = 0;
		while (threadFinished < numThreads)
		{
			double first = sft_clock();    
#pragma omp parallel for
			for (int j=0; j<numThreads ; j++)
			{
				//	activeThreadID = j;
				sudokuFill( &solution[0] , &states[0], tablero);
			}

#ifdef OPENCV
			if (numIterations % 50 == 0)
			{		   
				int mxI = 0;
				for (int j=0; j<numThreads ; j++)
				{
					if (solution[j].maxIndex > solution[mxI].maxIndex )
						mxI = j;
				}

				//showImageSudoku((char*)&solution[mxI].tablero, tablero,0,81);
			}
#endif

			cout   << "-----------------------------------" << endl;
			cout   << " Nothing FOUND by now \n" << endl;
			cout   << "-----------------------------------" << endl;
			printf("Required %f secs to Find  . \n ", sft_clock() - first);	
			numIterations += 1;
			int maxIndex = 0;
			threadFinished = 0;
			for (int j=0; j<numThreads ; j++)
			{
				if (solution[j].maxIndex> maxIndex)
					maxIndex = solution[j].maxIndex;
				if (solution[j].finished)
				{
					if (threadFinished == 0) 
						fprintf(stderr, "Thread finished %d  ",solution[j].thID );		
					else
						fprintf(stderr, " , %d  ",solution[j].thID );		

					threadFinished++;
				}
			}

			fprintf(stderr, "Max Depth %d  iteration %d \n",maxIndex,numIterations );

			if (maxIndex >= MAX_SUDOKU * MAX_SUDOKU - 1)
			{
				int mxI = 0;
				for (int j=0; j<numThreads ; j++)
				{
					if (solution[j].maxIndex > solution[mxI].maxIndex )
						mxI = j;
				}	
				showImageSudoku((char*)&solution[mxI].tablero, tablero,0,81);
				break ;
			}
			reallocateThreads(0);		

		}



}

int main(int argc, char *argv[], char *window_name)
{   
	char* tablero = new char[512*4];

	//char* fichas = generateValidBorder(tablero);
	char* firstParameter = argv[1];
	parseCommandLineArguments(argc, argv);

	sudokuCPU();
	/*
	// Solve Eternity
	if (useGPU)
	{
	GPUEternity(tablero);
	}
	else
	{
	fillFromCenter(tablero);	
	}
	*/

	/*
	if ((firstParameter) && (firstParameter[0] == 'G'))
	{
	GPUEternity(tablero);
	}
	else
	{
	fillFromCenter(tablero);	
	}
	*/

	//showImage(tablero, 4, 0);
	/*for (int i=0;i<256;i++)
	{
	int px, py;
	getPosFromCenter(i, &px, &py);
	tablero[(py*16+px)*4+0] = 1;
	tablero[(py*16+px)*4+1] = 1;
	tablero[(py*16+px)*4+2] = 1;
	tablero[(py*16+px)*4+3] = 1;
	showImage(tablero, 4, 0);
	}
	*/



}