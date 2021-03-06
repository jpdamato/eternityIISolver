#define OFFSET 5
#define MAX_ITERATIONS 800000
#define GPU 1
#define numThreads  48
#if defined GPU
constant
#endif
unsigned char gbFichas[256 * 4 ]= {
//Corner
1,17,0,0,
1,5,0,0,
9,17,0,0,
17,9,0,0,
//Border
2,1,0,1,
10,9,0,1,
6,1,0,1, //6
6,13,0,1,
11,17,0,1,
7,5,0,1,
15,9,0,1,
8,5,0,1,
8,13,0,1,
21,5,0,1,
10,1,0,9,
18,17,0,9,
14,13,0,9,
19,13,0,9,
7,9,0,9,
15,9,0,9,
4,5,0,9,
12,1,0,9,
12,13,0,9,
20,1,0,9,
21,1,0,9,
2,9,0,17,
2,17,0,17,
10,17,0,17,
18,17,0,17,
7,13,0,17,
15,9,0,17,
20,17,0,17,
8,9,0,17,
8,5,0,17,
16,13,0,17,
22,5,0,17,
18,1,0,5,
3,13,0,5,
11,13,0,5,
19,9,0,5,
19,17,0,5,
15,1,0,5,
15,9,0,5,
15,17,0,5,
4,1,0,5,
20,5,0,5,
8,5,0,5,
16,5,0,5,
2,13,0,13,
10,1,0,13,
10,9,0,13,
6,1,0,13,
7,5,0,13,
4,5,0,13,
4,13,0,13,
8,17,0,13,
16,1,0,13,
16,13,0,13,
21,9,0,13,
22,17,0,13,
// Other
6,18,2,2,
14,7,2,2,
10,3,2,10,
2,8,2,18,
18,22,2,18,
14,14,2,18,
11,10,2,18,
20,6,2,18,
22,8,2,18,
3,7,2,3,
7,12,2,3,
14,18,2,11,
15,4,2,11,
20,15,2,11,
8,3,2,11,
14,15,2,19,
19,15,2,19,
3,16,2,7,
20,3,2,7,
16,21,2,7,
19,18,2,15,
18,18,2,4,
11,4,2,4,
18,19,2,12,
6,14,2,12,
8,12,2,12,
16,20,2,12,
2,21,2,20,
6,22,2,20,
4,16,2,20,
11,12,2,8,
19,15,2,8,
19,4,2,8,
4,21,2,8,
12,14,2,8,
21,3,2,21,
4,19,2,22,
20,8,2,22,
21,6,2,22,
22,21,2,22,
12,15,10,10,
12,16,10,10,
16,19,10,10,
22,6,10,10,
4,15,10,18,
3,8,10,6,
19,8,10,6,
4,15,10,6,
16,11,10,6,
15,12,10,14,
12,15,10,14,
20,19,10,3,
20,16,10,3,
14,4,10,11,
7,12,10,11,
12,11,10,11,
22,16,10,11,
3,21,10,19,
16,12,10,7,
8,22,10,15,
14,22,10,4,
6,16,10,20,
14,19,10,20,
20,15,10,20,
12,22,10,8,
21,15,10,8,
14,6,10,16,
19,21,10,16,
4,3,10,16,
20,8,10,16,
6,20,10,21,
12,14,10,21,
14,16,10,22,
11,4,10,22,
4,3,10,22,
16,20,10,22,
20,7,18,18,
6,3,18,6,
6,11,18,6,
6,12,18,6,
19,21,18,6,
15,6,18,6,
16,12,18,6,
21,21,18,6,
3,4,18,14,
18,12,18,3,
18,22,18,3,
3,14,18,3,
15,12,18,3,
6,11,18,19,
4,22,18,19,
11,11,18,7,
11,19,18,7,
22,16,18,7,
7,7,18,4,
7,12,18,4,
22,7,18,4,
7,16,18,20,
8,6,18,20,
21,21,18,8,
6,20,18,16,
14,20,18,16,
15,11,18,22,
4,16,18,22,
3,4,6,14,
4,8,6,14,
3,3,6,11,
11,15,6,19,
19,21,6,19,
4,8,6,7,
20,16,6,7,
21,11,6,7,
15,15,6,15,
12,20,6,15,
7,21,6,4,
7,19,6,12,
14,4,6,20,
12,16,6,8,
8,15,6,8,
7,16,6,16,
11,16,6,21,
7,11,6,21,
19,8,14,14,
22,7,14,3,
19,12,14,11,
8,8,14,11,
21,7,14,19,
14,21,14,7,
3,19,14,7,
16,19,14,7,
3,3,14,15,
15,20,14,15,
11,7,14,4,
21,11,14,12,
21,22,14,12,
22,15,14,12,
11,22,14,20,
19,8,14,20,
20,20,14,20,
19,3,14,8,
21,8,14,16,
22,7,14,16,
12,19,14,21,
12,8,14,21,
16,3,14,21,
22,21,14,21,
22,7,3,3,
19,22,3,11,
8,15,3,11,
11,19,3,7,
16,15,3,7,
3,16,3,15,
8,8,3,4,
3,20,3,12,
4,22,3,12,
22,21,3,12,
19,15,3,20,
4,12,3,16,
11,4,3,21,
11,16,3,22,
21,21,3,22,
21,22,3,22,
12,22,11,11,
20,7,11,11,
16,15,11,11,
19,15,11,7,
12,12,11,7,
19,8,11,4,
7,22,11,20,
16,8,11,20,
12,20,11,8,
12,21,11,8,
19,20,19,19,
16,4,19,7,
7,4,19,4,
7,20,19,4,
12,15,19,4,
4,16,19,12,
15,22,19,20,
21,15,19,20,
7,21,19,8,
4,21,19,8,
15,12,7,15,
20,8,7,15,
22,20,7,4,
16,22,7,21,
21,22,15,15,
12,4,15,4,
4,21,15,12,
16,21,15,20,
22,8,4,4,
8,12,4,12,
16,20,12,8,
21,16,20,16,
16,22,20,22,
21,22,8,22
 };

#if defined GPU
constant
#endif
 unsigned char colors[23 * 3] = { 128 , 128 , 128 , 
	                     0 , 64 ,0 , //1
						 0 , 128 ,0 ,
						 0 , 192 ,0 ,
						 0 , 255 ,0 ,
						 0 , 0 ,64 ,
						 0 , 0 ,128 ,
						 0 , 0 ,192 ,
						 0 , 0 ,255 ,
						 64 , 0 ,0 ,
						 128 , 0 ,0 , //10
						 192 , 0 ,0 ,
						 255 , 0 ,0 ,
						 128 , 128 ,0 ,
						 0, 128 , 128 ,
						 128, 0 , 128 ,
						 192, 64, 64,
                         128, 192, 64, 
                         192, 64, 192,
						192, 135, 64,
						155, 202, 64, //20
						100, 224, 100,
						32, 240, 150 }; 

//constant short gbneighbours[256 * 200] = { $NEIGHBOUR } ;
#if defined GPU
 constant short gbposFromCenter[256 * 2] = {  7 ,8 ,7 ,7 ,8 ,7 ,8 ,8 ,8 ,9 ,7 ,9 ,6 ,9 ,6 ,8 ,6 ,7 ,6 ,6 ,7 ,6 ,8 ,6 ,9 ,6 ,9 ,7 ,9 ,8 ,9 ,9 ,9 ,10 ,8 ,10 ,7 ,10 ,6 ,10 ,5 ,10 ,5 ,9 ,5 ,8 ,5 ,7 ,5 ,6 ,5 ,5 ,6 ,5 ,7 ,5 ,8 ,5 ,9 ,5 ,10 ,5 ,10 ,6 ,10 ,7 ,10 ,8 ,10 ,9 ,10 ,10 ,10 ,11 ,9 ,11 ,8 ,11 ,7 ,11 ,6 ,11 ,5 ,11 ,4 ,11 ,4 ,10 ,4 ,9 ,4 ,8 ,4 ,7 ,4 ,6 ,4 ,5 ,4 ,4 ,5 ,4 ,6 ,4 ,7 ,4 ,8 ,4 ,9 ,4 ,10 ,4 ,11 ,4 ,11 ,5 ,11 ,6 ,11 ,7 ,11 ,8 ,11 ,9 ,11 ,10 ,11 ,11 ,11 ,12 ,10 ,12 ,9 ,12 ,8 ,12 ,7 ,12 ,6 ,12 ,5 ,12 ,4 ,12 ,3 ,12 ,3 ,11 ,3 ,10 ,3 ,9 ,3 ,8 ,3 ,7 ,3 ,6 ,3 ,5 ,3 ,4 ,3 ,3 ,4 ,3 ,5 ,3 ,6 ,3 ,7 ,3 ,8 ,3 ,9 ,3 ,10 ,3 ,11 ,3 ,12 ,3 ,12 ,4 ,12 ,5 ,12 ,6 ,12 ,7 ,12 ,8 ,12 ,9 ,12 ,10 ,12 ,11 ,12 ,12 ,12 ,13 ,11 ,13 ,10 ,13 ,9 ,13 ,8 ,13 ,7 ,13 ,6 ,13 ,5 ,13 ,4 ,13 ,3 ,13 ,2 ,13 ,2 ,12 ,2 ,11 ,2 ,10 ,2 ,9 ,2 ,8 ,2 ,7 ,2 ,6 ,2 ,5 ,2 ,4 ,2 ,3 ,2 ,2 ,3 ,2 ,4 ,2 ,5 ,2 ,6 ,2 ,7 ,2 ,8 ,2 ,9 ,2 ,10 ,2 ,11 ,2 ,12 ,2 ,13 ,2 ,13 ,3 ,13 ,4 ,13 ,5 ,13 ,6 ,13 ,7 ,13 ,8 ,13 ,9 ,13 ,10 ,13 ,11 ,13 ,12 ,13 ,13 ,13 ,14 ,12 ,14 ,11 ,14 ,10 ,14 ,9 ,14 ,8 ,14 ,7 ,14 ,6 ,14 ,5 ,14 ,4 ,14 ,3 ,14 ,2 ,14 ,1 ,14 ,1 ,13 ,1 ,12 ,1 ,11 ,1 ,10 ,1 ,9 ,1 ,8 ,1 ,7 ,1 ,6 ,1 ,5 ,1 ,4 ,1 ,3 ,1 ,2 ,1 ,1 ,2 ,1 ,3 ,1 ,4 ,1 ,5 ,1 ,6 ,1 ,7 ,1 ,8 ,1 ,9 ,1 ,10 ,1 ,11 ,1 ,12 ,1 ,13 ,1 ,14 ,1 ,14 ,2 ,14 ,3 ,14 ,4 ,14 ,5 ,14 ,6 ,14 ,7 ,14 ,8 ,14 ,9 ,14 ,10 ,14 ,11 ,14 ,12 ,14 ,13 ,14 ,14 ,14 ,15 ,13 ,15 ,12 ,15 ,11 ,15 ,10 ,15 ,9 ,15 ,8 ,15 ,7 ,15 ,6 ,15 ,5 ,15 ,4 ,15 ,3 ,15 ,2 ,15 ,1 ,15 ,0 ,15 ,0 ,14 ,0 ,13 ,0 ,12 ,0 ,11 ,0 ,10 ,0 ,9 ,0 ,8 ,0 ,7 ,0 ,6 ,0 ,5 ,0 ,4 ,0 ,3 ,0 ,2 ,0 ,1 ,0 ,0 ,1 ,0 ,2 ,0 ,3 ,0 ,4 ,0 ,5 ,0 ,6 ,0 ,7 ,0 ,8 ,0 ,9 ,0 ,10 ,0 ,11 ,0 ,12 ,0 ,13 ,0 ,14 ,0 ,15 ,0 ,15 ,1 ,15 ,2 ,15 ,3 ,15 ,4 ,15 ,5 ,15 ,6 ,15 ,7 ,15 ,8 ,15 ,9 ,15 ,10 ,15 ,11 ,15 ,12 ,15 ,13 ,15 ,14 ,15 ,15 } ;
 constant short gbborderpatternCounts[25] = {  64 ,24 ,4 ,1 ,4 ,24 ,3 ,4 ,6 ,24 ,5 ,2 ,2 ,24 ,1 ,6 ,4 ,24 ,3 ,3 ,3 ,3 ,2 ,0 ,0 } ;
#else
 short gbposFromCenter[256 * 2];
 short gbborderpatternCounts[25];
#endif



#if !defined GPU
int get_ThreadID(int dim)
{
	return omp_get_thread_num();
}
#else
int get_ThreadID(int dim)
{
	return get_global_id(dim);
}
#endif


void Char4(char* c4,char a, char b, char c, char d) 
{  
  c4[0] = a;
  c4[1] = b;
  c4[2] = c;
  c4[3] = d;

}

//----------------------
// Recursive FILL
//----------------------
int fixed(int px, int py)
{
	if( (px ==7 ) && (py ==8)) return 1;
	else if( (px ==13 ) && (py ==2)) return 1;
	else if( (px ==2 ) && (py ==13)) return 1;
	else if( (px ==2 ) && (py ==2)) return 1;
	else if( (px ==13 ) && (py ==13)) return 1;

	return 0;
}
#if !defined GPU
 typedef unsigned char uchar;
 typedef unsigned int uint;
#endif

 typedef struct Stack_T
 {
	 uchar elementID;
	 uchar neigh ; 
	 char rot;
	 uchar to;	 
 } Stack ; 

typedef struct Solution_T
{
	uchar result[256];
    uchar fUsed[256];
    uchar patternCounts[25];
	char tablero[256*4] ; 
//	int fichas[256*5] ;
	Stack stack[256 ];
	int stackIndex;
	int index; 
	char first;
	char finished ;	
   // short borderpatternCounts[25];
  //  short neighBours[256 * 200];
	int minIndex, maxIndex;
	uint count ; 

	int nThreads, thID,flag; 
#if !defined GPU
	void rfill(char* tablero, int* fichas, int index, short* neighBours, short* posFromCenter, short* borderpatternCounts) ; 
#endif
} Solution ; 

typedef struct State_T
{
	int stackIndex;
	int index; 
	char first;
	char finished ;
	int minIndex, maxIndex;
	uint count ; 
	int nThreads, thID,flag; 
} State ; 

#if defined GPU
typedef global Solution* Solution_PTR ;
typedef global Solution** Solution_ARRAY ;
typedef int* Int_ptr ;
typedef global char* Char_ptr ;
typedef global short* Short_PTR;
typedef global State* State_PTR ;
#else
typedef Solution* Solution_PTR ;
typedef State* State_PTR ;
typedef Solution** Solution_ARRAY ;
typedef int* Int_ptr ;
typedef char* Char_ptr ;
typedef short* Short_PTR;
#endif

void shiftL(Int_ptr ficha, int count)
{
	if (count == 0) return;
	if (count ==1) 
	{
	 char aux = ficha[0];
	 ficha[0] = ficha[1];
	 ficha[1] = ficha[2];
	 ficha[2] = ficha[3];
	 ficha[3] = aux;
	}
	else
	if (count ==2) 
	{
	 char aux = ficha[0];
	 ficha[0] = ficha[2];
	 ficha[2] = aux;
	 aux = ficha[1];
	 ficha[1] = ficha[3];
	 ficha[3] = aux;
	}
	else
	if (count ==3) 
	{
	 char aux = ficha[0];
	 ficha[0] = ficha[3];
	 ficha[3] = ficha[2];	 
	 ficha[2] = ficha[1];	 
	 ficha[1] = aux;
	}
	
}

void getPosFromCenter(Short_PTR posFromCenter, int index, int* pX, int* pY)
{
	*pX = gbposFromCenter[2*index];
	*pY = gbposFromCenter[2*index+1];
}

void computeNeighbours(Short_PTR neighBours)
{
	for (int i = 0;i<256 * 200 ; i++)		
		neighBours[i] = -1;

	for (int i = 0;i<256 * 4 ; i++)		
		for (int j=0; j<256*4 ; j++)
		{
		
			int fi = i / 4 , fir = i %4;
			int fj = j / 4 , fjr = j %4;
			// Si es la misma ficha
			if (fi == fj) continue;
			// El borde no me importa
			if (gbFichas[i] == 0 ) continue;
			if (gbFichas[i] == gbFichas[j])
			{
				int ni = fi*200 + fir * 50;
				bool repetido = false;
				while( neighBours[ni]>=0) 
				{ 
					//Ya es un vecino
					if ( neighBours[ni] == fj) repetido = true;
					ni++; 
				}
				if (!repetido)
				  neighBours[ni] = fj;
			}
		}
}
	// es una ficha de borde, pero no una posicion de borde
#define checkBorder(f0,f1, f2, f3,  posX,  posY)  (!( ((f0  == 0) && (posY >= 1)) ||  ((f1  == 0) && (posX < 15)) ||  ((f2  == 0) && (posY < 15)) ||   ((f3  == 0) && (posX >= 1)) ) )

// no me alcanza para completar con lo valores requeridos en el borde
#define checkPatterns( patternCounts,  f0, f1, f2,  f3)  (!(( patternCounts[f0]  <= gbborderpatternCounts[f0]) ||  ( patternCounts[f1]  <= gbborderpatternCounts[f1]) ||  ( patternCounts[f2]  <= gbborderpatternCounts[f2]) || ( patternCounts[f3]  <= gbborderpatternCounts[f3])  ) )

// Coincide con la ficha siguiente o el espacio esta vacio o es borde
#define can(  tablero,  f0, f1,  f2,  f3,  posX,  posY) ( ( ((posY > 0 ) && ( f0 ==  tablero[(posX)*16*4+(posY-1)*4+2])) || ((posY > 0 )   && ( tablero[(posX)*16*4+(posY-1)*4+2] == -1)) || ( (posY==0) && (f0 == 0) ) ) &&     ( ((posX > 0 )  && ( f3 ==  tablero[(posX-1)*16*4+posY*4+1])) || ((posX > 0 )   && ( tablero[(posX-1)*16*4+posY*4+1] == -1)) || ( (posX==0) && (f3 == 0)) ) &&     ( ((posX < 15 ) && ( f1 ==  tablero[(posX+1)*16*4+posY*4+3])) || ((posX < 15 ) && ( tablero[(posX+1)*16*4+posY*4+3] == -1)) || ( (posX == 15) && (f1 == 0))) &&     ( ((posY < 15 ) && ( f2 ==  tablero[(posX)*16*4+(posY+1)*4+0])) || ((posY < 15 ) && ( tablero[(posX)*16*4+(posY+1)*4+0] == -1)) || ( (posY == 15) && (f2 == 0) ) ) )

int put(uchar*  patternCounts,uchar* fUsed, char* tablero, Int_ptr ficha, int posx, int posy)
{
//	if (can(tablero, ficha, posx, posy))
	{
		int index = (posx * 16 + posy)*4;
		tablero[index] = ficha[0];
		tablero[index+1] = ficha[1];
		tablero[index+2] = ficha[2];
		tablero[index+3] = ficha[3];

		patternCounts[ficha[0]]--;
		patternCounts[ficha[1]]--;
		patternCounts[ficha[2]]--;
		patternCounts[ficha[3]]--;

		fUsed[ficha[4]] = true;
		return 1;
	}
//	else
	//	return 0;
}

int putG(Solution_PTR sol, Char_ptr tablero, Int_ptr ficha, int posx, int posy)
{
//	if (can(tablero, ficha, posx, posy))
	{
		int index = (posx * 16 + posy)*4;
		tablero[index] = ficha[0];
		tablero[index+1] = ficha[1];
		tablero[index+2] = ficha[2];
		tablero[index+3] = ficha[3];
/* BUG
		sol->patternCounts[ficha[0]]--;
		sol->patternCounts[ficha[1]]--;
		sol->patternCounts[ficha[2]]--;
		sol->patternCounts[ficha[3]]--;
		*/
		sol->fUsed[ficha[4]] = true;
		return 1;
	}
//	else
	//	return 0;
}

void extract(uchar*  patternCounts,uchar* fUsed, char* tablero, Int_ptr ficha, int posx, int posy)
{
		int index = (posx * 16 + posy)*4;
		tablero[index] = -1;
		tablero[index+1] = -1;
		tablero[index+2] = -1;
		tablero[index+3] = -1;
		if (ficha)
	    {
		  patternCounts[ficha[0]]++;
		  patternCounts[ficha[1]]++;
		  patternCounts[ficha[2]]++;
		  patternCounts[ficha[3]]++;

		  fUsed[ficha[4]] = false;
		}
}

#if defined GPU
kernel
#endif
void ifill(Solution_PTR solutions , State_PTR states, const Short_PTR neighBours, Short_PTR posFromCenter, Short_PTR  borderpatternCounts)
{		
 int thID = get_ThreadID(0);
 Solution_PTR sol = &solutions[thID];
 State_PTR  state = &states[thID];
 
// Int_ptr fichas = sol->fichas;
 sol->flag = 11;
 state->thID = thID;
 int index = sol->index ;
 int count = 0;
 Stack local_stack[256 ];
 uchar local_fUsed[256];
 uchar local_patternCounts[25];
 char local_tablero[256*4] ; 
 uchar local_result[256];
 //-- Copy to local structures
 for (int i=0 ; i<256; i++)	 local_stack[i] = sol->stack[i]; 
 for (int i=0 ; i<256*4; i++) local_tablero[i] = sol->tablero[i]; 
 for (int i=0 ; i<25; i++) local_patternCounts[i] = sol->patternCounts[i];
 for (int i=0 ; i<256; i++) local_fUsed[i] = sol->fUsed[i];
 for (int i=0 ; i<256; i++) local_result[i] = sol->result[i];
 
 if (sol->finished) 
 {
	  state->finished = true;
	  return; 
 }

 while (true)
 {
	if (index < 5)
	{
	//	#if !defined GPU
		//fprintf(stderr, "Min Depth %d  \n",sol->index );		
		//#endif
	}

	if (index > sol->maxIndex)
	{
		/*#if !defined GPU
		fprintf(stderr, "Max Depth %d  \n",sol->index );
		#endif
		*/
		sol->maxIndex = index;
	}

	// llegue a una solucion posible
	if (index > 144) 
	{
		  state->finished = true;    
		  state->index = 144;    
		  state->maxIndex = 144;    
		  break ; 
	}
	       
	        int posX,posY, paX, paY ,pattern;
			// Obtengo la posicion donde va la ficha
			posX = gbposFromCenter[2*index] ;
			posY = gbposFromCenter[2*index+1] ;
			//getPosFromCenter(posFromCenter, index, &posX, &posY);
			// Me fijo la posicion anterior
			//getPosFromCenter(posFromCenter, index-1, &paX, &paY);
			paX = gbposFromCenter[2*(index-1)] ;
			paY = gbposFromCenter[2*(index-1)+1] ;

			// Me ubique arriba
			if (paY > posY) pattern = 0;
			// Me ubique a la derecha
			else if (paX < posX) pattern = 1;
			// Me ubique abajo
			else if (paY < posY) pattern = 2;
			// Me ubique a la izq
			else if (paX > posX) pattern = 3;
			else 
			{
			    sol->flag = 15;	
				return;
			}
			
			// Me fijo la posicion anterior
			    int fIndex = local_result[index -1];
				int from , to ; 

			
				if (sol->first )
				{
					from = (sol->thID-1) * 50 / numThreads;
					to   =  sol->thID * 50 / numThreads;
					sol->first = false;
				}
				else
				{
					from= local_stack[index].neigh , 
					to = local_stack[index].to ;
				}
				
				bool passed = false;

				for (int iU = from ; iU<to && !passed ; iU++) // Accediendo los vecinos
				{
					int ifU = neighBours[fIndex*200+ pattern * 50 +iU];
					// Si no tengo vecino en esa posicion
					// Si es ficha de Borde
					//Si ya esta usada
					if (ifU <0) break ; 
						
					if ( (ifU < 60) || (local_fUsed[ifU]) ) continue;

					//-- Copio la ficha
					int ficha[5];

					ficha[0] = gbFichas[ifU * 4+0];
					ficha[1] = gbFichas[ifU * 4+1];
					ficha[2] = gbFichas[ifU * 4+2];
					ficha[3] = gbFichas[ifU * 4+3];
					ficha[4]= ifU;
					if (! checkBorder(ficha[0], ficha[1],ficha[2],ficha[3], posX , posY) ) continue;
					if (! checkPatterns(local_patternCounts,ficha[0], ficha[1],ficha[2],ficha[3])) continue;
					
					int initialRot = local_stack[index].rot;

					if (initialRot > 1 )
						shiftL(ficha, initialRot-1);				
					
					for (int iRU = initialRot ; iRU < 4 && !passed ; iRU++)
					{
					  
					  ficha[0] = gbFichas[ifU * 4+0];
					  ficha[1] = gbFichas[ifU * 4+1];
					  ficha[2] = gbFichas[ifU * 4+2];
					  ficha[3] = gbFichas[ifU * 4+3];
					  ficha[4]= ifU;
					  count++;
					  // La roto
					  shiftL(ficha, iRU);				
					  
					  // Intento ubicarla
					  if ( can(local_tablero, ficha[0],ficha[1],ficha[2],ficha[3], posX , posY) )
					  {
						local_result[index] = ficha[4];
						// Posicion actual					            
						local_stack[index].elementID = ifU;
						local_stack[index].neigh = iU;
			            local_stack[index].rot = iRU;
						
						index++;				

						put(local_patternCounts,local_fUsed, local_tablero, ficha,posX,posY);					
						passed = true;
					  }
					}	
					local_stack[index].rot = 0;
				}

				// Restauro la posicion anterior
				if (!passed)
				{                    
					local_stack[index].elementID = 0;
					local_stack[index].neigh = 0;
					local_stack[index].rot = 0;
					index--;
					//Saco la ficha
					  int ficha[5];
					  int antF = local_stack[index].elementID;
					  ficha[4] = local_stack[index].elementID;
					  //Int_ptr ficha = &fichas[sol->stack[3*sol->index+0] * OFFSET];
					  ficha[0] = gbFichas[antF * 4+0];
					  ficha[1] = gbFichas[antF * 4+1];
					  ficha[2] = gbFichas[antF * 4+2];
					  ficha[3] = gbFichas[antF * 4+3];					  									
					  extract(local_patternCounts,local_fUsed, local_tablero, ficha,posX,posY);

					 // Paso a la siguiente
					 local_stack[index].rot++;
					 if (local_stack[index].rot >=4 )
					 { 
						local_stack[index].rot = 0; 
						local_stack[index].neigh++ ;
					 }
					 if (index==0) break;
				}
     
	 if (count >= MAX_ITERATIONS ) 
	 { 
		  sol->count = 0 ;  	
		  sol->index = index;
		  state->index = sol->index;    
		  state->maxIndex = sol->maxIndex;    
		  state->minIndex = sol->minIndex;    
		  state->finished = false;
		  for (int i = 0;i<256 ; i++) sol->stack[i] = local_stack[i];
		  for (int i=0 ; i<256*4; i++) sol->tablero[i] =  local_tablero[i];
		  for (int i=0 ; i<25; i++) sol->patternCounts[i] = local_patternCounts[i]  ;
          for (int i=0 ; i<256; i++)  sol->fUsed[i] =local_fUsed[i] ;
		  for (int i=0 ; i<256; i++) sol->result[i] =local_result[i]  ;

		  return; 
	 }
	 count++;
	}
  sol->finished = true;
  sol->index = index;
  for (int i = 0;i<256 ; i++) sol->stack[i] = local_stack[i];
  for (int i=0 ; i<256*4; i++) sol->tablero[i] =  local_tablero[i];
  for (int i=0 ; i<25; i++) sol->patternCounts[i] = local_patternCounts[i]  ;
  for (int i=0 ; i<256; i++)  sol->fUsed[i] =local_fUsed[i] ;
  for (int i=0 ; i<256; i++) sol->result[i] =local_result[i]  ;

}


void start(Solution_PTR sol)
{
    for (int i=0; i<256;i++)
	{
		sol->stack[i].elementID = 0;
		sol->stack[i].neigh = 0;
		sol->stack[i].rot = 0;
		sol->stack[i].to = 50;
	}
		
	sol->stackIndex = 1;
	// Pongo la pieza fija
	sol->stack[0].elementID = 139;
	sol->stack[0].neigh = 0;
	sol->stack[0].rot = 0;	
	sol->first = true;
	sol->index = 1;
	sol->finished = false;
}

void initFichas(Solution_PTR sol, Short_PTR posFromCenter)
{
	//sol->fichas = new int[256 * 5];
	for (int i=0 ; i<256 ; i++)
	{
		/*sol->fichas[i*5] = gbFichas[i*4];
		sol->fichas[i*5+1] = gbFichas[i*4+1];
		sol->fichas[i*5+2] = gbFichas[i*4+2];
		sol->fichas[i*5+3] = gbFichas[i*4+3];
		sol->fichas[i*5+4] = i ; */
	}

	for (int i=0; i<25;i++)
	{
		sol->patternCounts[i] = 0;
		
	}

	for (int i=0; i<256*4;i++)
		sol->patternCounts[gbFichas[i]]++;
	
	

	//solution[j]->tablero = new char[512*4];  
	for (int i=0; i<256;i++)	
	{
		sol->stack[i].elementID = 0;
		sol->stack[i].neigh = 0;
		sol->stack[i].rot = 0;
	}
		 
    for (int i=0;i<256*4;i++) 
		sol->tablero[i] = -1;
    for (int i=0;i<256;i++) 
		sol->fUsed[i] = 0;
    // Pongo la ficha central
	int fichas[5] ;
	fichas[0] = gbFichas[139 * 4];
	fichas[1] = gbFichas[139 * 4+1];
	fichas[2] = gbFichas[139 * 4+2];
	fichas[3] = gbFichas[139 * 4+3];
	fichas[4] = 139;
	putG(sol,sol->tablero,(Int_ptr)&fichas[0],7,8);
    sol->result[0] = 139; 
}

#if defined GPU
kernel
#endif
	void startArray(Solution_PTR solutions, Short_PTR posFromCenter)
{
	int thID = get_ThreadID(0);
    solutions[thID].thID = thID+1;
	start(&solutions[thID]);	
	initFichas(&solutions[thID], posFromCenter);
}

#if defined GPU
kernel
#endif
void computeNeighBours(Short_PTR neighbours)
{
	int thID = get_ThreadID(0);
	if (thID == 0)
      computeNeighbours(neighbours);	
}
 


#if !defined GPU
void Solution::rfill(char* tablero, int* fichas, int index, short* neighBours, short* posFromCenter, short* borderpatternCounts)
{	
	if (index < 5)
	{
		fprintf(stderr, "Min Depth %d  \n",index );		
	}

	if (index > maxIndex)
	{
		fprintf(stderr, "Max Depth %d  \n",index );
		maxIndex = index;
	}

	// llegue a una solucion posible
	if (index > 144) 
	{
		  //  fprintf(stderr, "Tablero Iter: %d  Solucion: %d M \n",iter ,acumfound);
			//showImage(tablero, 4, 0);
		return ; 
	}
	else
	{		
			int posX,posY, paX, paY ,pattern;
			// Obtengo la posicion donde va la ficha
			getPosFromCenter(posFromCenter, index, &posX, &posY);
			// Me fijo la posicion anterior
			getPosFromCenter(posFromCenter, index-1, &paX, &paY);

			// Me ubique arriba
			if (paY > posY) pattern = 0;
			// Me ubique a la derecha
			else if (paX < posX) pattern = 1;
			// Me ubique abajo
			else if (paY < posY) pattern = 2;
			// Me ubique a la izq
			else if (paX > posX) pattern = 3;
			else return;

			
			// si ya esta ocupada, paso a la siguiente
			if (fixed(posX, posY) )
			{
				rfill(tablero, fichas, index+1, neighBours, posFromCenter,borderpatternCounts);
			}
			else
			{	
				// Me fijo la posicion anterior
				int fIndex = result[index -1];
				int from, to ;
				if (first )
				{
					from = (thID-1) * 50 / nThreads;
					to   =  thID * 50 / nThreads;
					first = false;
				}
				else
				{
					from = 0;
					to = 50;
				}

				for (int iU = from ; iU<to ; iU++) // Accediendo los vecinos
				//for (int iU=256*4-1 ; iU>= 0 ; iU--) //con rotacion
				//for (int iU=255 ; iU>= 0 ; iU--) //sin rotacion
				{
					int ifU = neighBours[fIndex*200+ pattern * 50 +iU];
					// Si no tengo vecino en esa posicion
					if (ifU <0) continue;
					//int ifU = iU / 4, iRU = iU % 4; // con rotacion
					//int ifU = iU , iRU = 0; // sin rotacion
					//-- Copio la ficha
					Int_ptr ficha = &fichas[ifU * OFFSET];
					
					// Si es ficha de Borde
					if (ficha[4] < 60) continue;

					//Si ya esta usada
					if (fUsed[ficha[4]]) continue;

					for (int iRU = 0 ; iRU < 4 ; iRU++)
					{
					 ficha[0] = gbFichas[ifU * 4+0];
					 ficha[1] = gbFichas[ifU * 4+1];
					 ficha[2] = gbFichas[ifU * 4+2];
					 ficha[3] = gbFichas[ifU * 4+3];
					 
					 // La roto
					 shiftL(ficha, iRU);				
					 // Intento ubicarla
					// if ( can(this, tablero, ficha, posX , posY,borderpatternCounts) )
					 {
						result[index] = ficha[4];
						// Posicion actual		
						stack[index].elementID = ifU;
						stack[index].neigh = iU;
			            stack[index].rot = iRU;
						
						//put(this, tablero, ficha,posX,posY);
						rfill(tablero, fichas, index+1,neighBours,posFromCenter,borderpatternCounts);
						//extract(this, tablero,ficha, posX,posY);
					 }
					}
				}
			}		
	}

}
#endif
#                                   