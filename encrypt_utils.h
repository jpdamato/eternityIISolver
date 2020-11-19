#include <string.h>

using namespace std;

string convertInt(int number)
{
   
   char buffer [33];   
   itoa (number,buffer,10);
   string ss(buffer) ;
   return ss;
}

string binToString(char* key , int keylength)
{
	std::string s(" ") ; 

	for (int i=0 ; i<keylength-1 ; i++)
      if (key[i] == 0 )
         s += " 0,";
	  else
	     s += " 1,";

      if (key[keylength-1] == 0 )
         s += " 0";
	  else
	     s += " 1";

	return s;

}

string charToString(char* key , int keylength)
{
	std::string s(" ") ; 

	for (int i=0 ; i<keylength-1 ; i++)
      if (key[i] == 0 )
         s += " 0,";
	  else
	     s += " 1,";

      if (key[keylength-1] == 0 )
         s += " 0";
	  else
	     s += " 1";

	return s;

}

string intToString(int* key , int keylength)
{
	std::string s(" ") ; 

	for (int i=0 ; i<keylength-1 ; i++)
      if (key[i] == 0 )
         s += " 0,";
	  else
	     s += " 1,";

      if (key[keylength-1] == 0 )
         s += " 0";
	  else
	     s += " 1";

	return s;

}

string shortToString(short* key , int keylength)
{
	std::string s(" ") ; 
	
	for (int i=0 ; i<keylength-1 ; i++)
    {		
		string sk =convertInt(key[i]); 
		s += sk+" ,";
	}

	string sk =convertInt(key[keylength-1]); 
	s += sk;

	return s;

}


bool replace(std::string& str, const std::string& from, const std::string& to) 
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


void keysReplacement(char* filename, char* tag, string key, char* dstfilename  )
{
	
	// locals 
    FILE* pFileStream = NULL;
    unsigned int szSourceLength;
    if(fopen_s(&pFileStream, filename, "rb") != 0) 
        {       
            return ;
        }
	    
	 // get the length of the source code
    fseek(pFileStream, 0, SEEK_END); 
    szSourceLength = ftell(pFileStream);
    fseek(pFileStream, 0, SEEK_SET); 

    // allocate a buffer for the source code string and read it in
    char* filecontent =new char[szSourceLength]; 
    
    if (fread((filecontent) , szSourceLength, 1, pFileStream) != 1)
    {
        fclose(pFileStream);
        free(filecontent);
        return ;
    }

    // close the file and return the total length of the combined (preamble + source) string
    fclose(pFileStream);
	// Load File
	
	/// Replace String
   std::string st(filecontent);
   replace(st, tag, key);

   // guardo  
    char* outS = new char[st.length() ];

    for (int i = 0; i<st.length() ; i++)
		outS[i] = st[i];
	outS[st.length()-1] = 0;
	for (int i = st.length()-1 ; i>0 ; i--)
	{
		if (outS[i] == '#') break;
		outS[i] = ' ' ;

	}
	FILE*  pFile = fopen ( dstfilename, "wb" );
	fwrite (outS, 1 , st.length() - 1 , pFile );
    fclose (pFile);
	free(filecontent);
	free(outS);
            

}
	