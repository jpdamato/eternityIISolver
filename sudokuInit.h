

void init9x9(char* tablero)
{
	// Tablero inicial
	tablero[0] = 2;
	tablero[4] = 1;
	tablero[7] = 7;
	tablero[9] = 9;
	tablero[10] = 3;
	tablero[14] = 4;	
	tablero[16] = 5;
	tablero[21] = 9;
	tablero[25] = 6;
	tablero[28] = 5;
	tablero[29] = 7;
	tablero[32] = 3;
	tablero[48] = 7;
	tablero[53] = 1;

	tablero[55] = 9;	
	tablero[57] = 2;
	tablero[65] = 5;
	tablero[67] = 4;
	tablero[68] = 7;

	tablero[73] = 1;

	tablero[74] = 4;
	tablero[75] = 6;

	tablero[80] = 7;
	tablero[80] = 5;
}

#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15
#define G 16

#define index(y,x) (y-1) * 16 + x-1

void init16x16(char* tablero)
{
	// Tablero inicial
	// Line 1
	//tablero[index(1,1)] = A;
	tablero[index(1,5)] = E;
	tablero[index(1,8)] = 3;
	tablero[index(1,9)] = 1;
	tablero[index(1,15)] = 8;


//    tablero[index(1,1)] = A; //1th 2988 iteraciones | 2th 898 iteraciones | 4th ..| 8th 1494 iteraciones | 16th  996 iteraciones
   // tablero[index(1,2)] = 7; //417 iteraciones
//	tablero[index(1,3)] = F; //236 iteraciones  //4 threads 68it  //6 threas 102it
   //tablero[index(1,4)] = 9;
	//tablero[index(1,5)] = E;
	//tablero[index(1,6)] = B;
	//tablero[index(1,7)] = 6;
	//tablero[index(1,8)] = 3;
	//tablero[index(1,9)] = 1;
	//tablero[index(1,10)] = C;
	//tablero[index(1,11)] = 4;
	//tablero[index(1,12)] = G;
	//tablero[index(1,13)] = 5;
	//tablero[index(1,14)] = D;
	//tablero[index(1,15)] = 8;
	//tablero[index(1,16)] = 2;


	// Line 2
	tablero[index(2,1)] = 2;
	tablero[index(2,4)] = 3;	
	tablero[index(2,5)] = D;
	tablero[index(2,6)] = 5;
	tablero[index(2,9)] = 6;
	tablero[index(2,12)] = 9;
	tablero[index(2,13)] = E;
	tablero[index(2,15)] = F;
	tablero[index(2,16)] = G;
	
	// Line 3
	tablero[index(3,1)] = 8;
	tablero[index(3,2)] = 1;	
	tablero[index(3,3)] = 5;
	tablero[index(3,4)] = 6;
	tablero[index(3,8)] = 7;
	tablero[index(3,9)] = F;
	tablero[index(3,10)] = B;
	tablero[index(3,14)] = C;
	
	// Line 4
	tablero[index(4,6)] = A;
	tablero[index(4,10)] = 8;
	tablero[index(4,11)] = 3;
	tablero[index(4,14)] = 1;
	tablero[index(4,15)] = 7;

	// Line 5
	tablero[index(5,2)] = F;
	tablero[index(5,5)] = A;
	tablero[index(5,6)] = 6;
	tablero[index(5,11)] = C;
	tablero[index(5,12)] = E;
	tablero[index(5,13)] = 8;
	tablero[index(5,16)] = 1;

	// Line 6
	tablero[index(6,3)] = G;
	tablero[index(6,4)] = D;
	tablero[index(6,8)] = 1;
	tablero[index(6,10)] = 6;
	tablero[index(6,11)] = 5;
	tablero[index(6,14)] = F;
	tablero[index(6,16)] = 7;

	// Line 7
	tablero[index(7,1)] = 4;
	tablero[index(7,4)] = 1;
	tablero[index(7,5)] = B;
	tablero[index(7,7)] = D;
	tablero[index(7,16)] = 5;

	// Line 8
	tablero[index(8,1)] = B;
	tablero[index(8,6)] = 3;
	tablero[index(8,7)] = E;
	tablero[index(8,9)] = G;
	tablero[index(8,10)] = 1;
	tablero[index(8,15)] = A;

	// Line 9
	tablero[index(9,2)] = C;
	tablero[index(9,7)] = G;
	tablero[index(9,8)] = 9;
	tablero[index(9,10)] = D;
	tablero[index(9,11)] = 1;
	tablero[index(9,16)] = F;

	// Line 10
	tablero[index(10,1)] = G;
	tablero[index(10,10)] = F;
	tablero[index(10,12)] = A;
	tablero[index(10,13)] = 7;
	tablero[index(10,16)] = C;

	// Line 11
	tablero[index(11,1)] = 5;
	tablero[index(11,3)] = A;
	tablero[index(11,6)] = 2;
	tablero[index(11,7)] = F;
	tablero[index(11,9)] = 7;
	tablero[index(11,13)] = G;
	tablero[index(11,14)] = 6;

	// Line 12
	tablero[index(12,1)] = 6;
	tablero[index(12,4)] = F;
	tablero[index(12,5)] = 8;
	tablero[index(12,6)] = 4;
	tablero[index(12,11)] = G;
	tablero[index(12,12)] = C;
	tablero[index(12,15)] = B;

	// Line 13
	tablero[index(13,2)] = 6;
	tablero[index(13,3)] = D;
	tablero[index(13,6)] = 8;
	tablero[index(13,7)] = 5;
	tablero[index(13,11)] = 7;

	// Line 14
	tablero[index(14,3)] = 2;
	tablero[index(14,7)] = C;
	tablero[index(14,8)] = E;
	tablero[index(14,9)] = A;
	tablero[index(14,13)] = 9;
	tablero[index(14,14)] = 5;
	tablero[index(14,15)] = 3;
	tablero[index(14,16)] = D;

	// Line 15
	tablero[index(15,1)] = C;
	tablero[index(15,2)] = 4;
	tablero[index(15,4)] = G;
	tablero[index(15,5)] = F;
	tablero[index(15,8)] = D;
	tablero[index(15,11)] = 2;
	tablero[index(15,12)] = 1;
	tablero[index(15,13)] = B;
	tablero[index(15,16)] = A;
	
	// Line 16
	tablero[index(16,2)] = E;
	tablero[index(16,8)] = B;
	tablero[index(16,9)] = C;
	tablero[index(16,12)] = 8;
}