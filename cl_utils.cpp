#include <oclUtils.h>
#include <iostream>

#include "cl_utils.h"



using namespace std;


// OpenCL Vars
cl_context cxGPUContext;        // OpenCL context
cl_command_queue cqCommandQueue = NULL;// OpenCL command que
cl_platform_id cpPlatform;      // OpenCL platform
cl_device_id cdDevice;          // OpenCL device
cl_program cpProgram;           // OpenCL program
cl_kernel ckKernel;             // OpenCL kernel
cl_mem cmDevSrcA;               // OpenCL device source buffer A
cl_mem cmDevSrcB;               // OpenCL device source buffer B 
cl_mem cmDevDst;                // OpenCL device destination buffer 
unsigned int szGlobalWorkSize;        // 1D var for Total # of work items
unsigned int szLocalWorkSize;		    // 1D var for # of work items in the work group	
unsigned int szParmDataBytes;			// Byte size of context information
unsigned int szKernelLength;			// Byte size of kernel code
cl_int ciErr1;			// Error code var
char* cPathAndName = NULL;      // var for full paths to data, src, etc.
char* cSourceCL = NULL;         // Buffer to hold source for compilation 


// Helper function to get error string
// *********************************************************************
const char* ErrorString(cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "";
}


cl_device_id getDefaultDevice()
{
	return cdDevice;
}

cl_context clgetContext()
{
	return cxGPUContext;
}

int printGPUCapacities()
{
	cl_platform_id platforms[100];
	cl_uint platforms_n = 0;
	clGetPlatformIDs(100, platforms, &platforms_n);

	printf("=== %d OpenCL platform(s) found: ===\n", platforms_n);
	for (unsigned int i=0; i<platforms_n; i++)
	{
		char buffer[10240];
		printf("  -- %d --\n", i);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL);
		printf("  PROFILE = %s\n", buffer);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 10240, buffer, NULL);
		printf("  VERSION = %s\n", buffer);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 10240, buffer, NULL);
		printf("  NAME = %s\n", buffer);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 10240, buffer, NULL);
		printf("  VENDOR = %s\n", buffer);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL);
		printf("  EXTENSIONS = %s\n", buffer);
	}

	if (platforms_n == 0)
		return 1;
	cl_device_id devices[100];
	cl_uint devices_n = 0;
	// CL_CHECK(clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 100, devices, &devices_n));
	clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 100, devices, &devices_n);

	printf("=== %d OpenCL device(s) found on platform:\n", platforms_n);
	for (unsigned int i=0; i<devices_n; i++)
	{
		char buffer[10240];
		cl_uint buf_uint;
		cl_ulong buf_ulong;
		printf("  -- %d --\n", i);
		clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
		printf("  DEVICE_NAME = %s\n", buffer);
		clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL);
		printf("  DEVICE_VENDOR = %s\n", buffer);
		clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
		printf("  DEVICE_VERSION = %s\n", buffer);
		clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL);
		printf("  DRIVER_VERSION = %s\n", buffer);
		clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL);
		printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
		clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL);
		printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
		clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
		printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n", (unsigned long long)buf_ulong);

		clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
		printf("  DEVICE_MAX_CONSTANT_BUFFER_SIZE = %llu\n", (unsigned long long)buf_ulong);

		clGetDeviceInfo(devices[i], CL_DEVICE_PROFILE, sizeof(buf_ulong), &buf_ulong, NULL);
		printf("  DEVICE_PROFILE = %llu\n", (unsigned long long)buf_ulong);

		clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(buf_ulong), &buf_ulong, NULL);
		printf("  DEVICE_GLOBAL_MEM_CACHE_TYPE = %llu\n", (unsigned long long)buf_ulong);
	}

	printf("=== ================================= \n");
	return 0;
}


void initGPU()
{
	//Get an OpenCL platform
    ciErr1 = clGetPlatformIDs(1, &cpPlatform, NULL);
	
    
    if (ciErr1 != CL_SUCCESS)
    {
		showLastError(ciErr1);
		return ;        
	}

    //Get the devices
    ciErr1 = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &cdDevice, NULL);
    
    if (ciErr1 != CL_SUCCESS)
    {
        return;
        
    }

    //Create the context
    cxGPUContext = clCreateContext(0, 1, &cdDevice, NULL, NULL, &ciErr1);
    
    if (ciErr1 != CL_SUCCESS)
    {
       showLastError(ciErr1);
		return;
        
    }

    // Create a command-queue
    cqCommandQueue = clCreateCommandQueue(cxGPUContext, cdDevice, 0, &ciErr1);
    
    if (ciErr1 != CL_SUCCESS)
    {
        showLastError(ciErr1);
		return; 
    }
	

}

cl_int getLastError()
{
  return ciErr1;
}

void showLastError(cl_int errorCode)
{
  cout<< ErrorString(errorCode)<<"\n";
}

cl_program getDefaultProgram()
{
	return cpProgram;
}

cl_kernel getDefaultKernel()
{
	return ckKernel;
}
// Round Up Division function
unsigned int RoundUp(int group_size, int global_size) 
{
    int r = global_size % group_size;
    if(r == 0) 
    {
        return global_size;
    } else 
    {
        return global_size + group_size - r;
    }
}

//////////////////////////////////////////////////////////////////////////////
//! Loads a Program file and prepends the cPreamble to the code.
//!
//! @return the source string if succeeded, 0 otherwise
//! @param cFilename        program filename
//! @param cPreamble        code that is prepended to the loaded file, typically a set of #defines or a header
//! @param szFinalLength    returned length of the code string
//////////////////////////////////////////////////////////////////////////////
char* _LoadProgSource(const char* cFilename, const char* cPreamble, unsigned int* szFinalLength)
{
    // locals 
    FILE* pFileStream = NULL;
    unsigned int szSourceLength;

    // open the OpenCL source code file
    #ifdef _WIN32   // Windows version
        if(fopen_s(&pFileStream, cFilename, "rb") != 0) 
        {       
            return NULL;
        }
    #else           // Linux version
        pFileStream = fopen(cFilename, "rb");
        if(pFileStream == 0) 
        {       
            return NULL;
        }
    #endif

    unsigned int szPreambleLength = strlen(cPreamble);

    // get the length of the source code
    fseek(pFileStream, 0, SEEK_END); 
    szSourceLength = ftell(pFileStream);
    fseek(pFileStream, 0, SEEK_SET); 

    // allocate a buffer for the source code string and read it in
    char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1); 
    memcpy(cSourceString, cPreamble, szPreambleLength);
    if (fread((cSourceString) + szPreambleLength, szSourceLength, 1, pFileStream) != 1)
    {
        fclose(pFileStream);
        free(cSourceString);
        return 0;
    }

    // close the file and return the total length of the combined (preamble + source) string
    fclose(pFileStream);
    if(szFinalLength != 0)
    {
        *szFinalLength = szSourceLength + szPreambleLength;
    }
    cSourceString[szSourceLength + szPreambleLength] = '\0';

    return cSourceString;
}

cl_command_queue getDefaultCommandQueue()
{
	return cqCommandQueue;
}

cl_kernel initGPUProgram(char * cSourceFileName, char* kernelName)
{
// Read the OpenCL kernel in from source file
    char* cSourceCL = _LoadProgSource(cSourceFileName, "", &szKernelLength);

	if (!cSourceCL)
	{
		 cout<< " File not found \n"  ;
		 exit(0);
	}
	cl_kernel _ck;
    // Create the program
    cpProgram = clCreateProgramWithSource(cxGPUContext, 1, (const char **)&cSourceCL, &szKernelLength, &ciErr1);
    if (ciErr1 != CL_SUCCESS)
    {
	//	oclLogBuildInfo(cpProgram , cdDevice);
		return NULL;
    }

    // Build the program with 'mad' Optimization option
    #ifdef MAC
        char* flags = "-cl-fast-relaxed-math -DMAC";
    #else
        char* flags = "-cl-fast-relaxed-math";
    #endif
    ciErr1 = clBuildProgram(cpProgram, 0, NULL, NULL, NULL, NULL);
    if (ciErr1 != CL_SUCCESS)
    {
		// write out the build log and ptx, then exit
      char cBuildLog[10240];
	  memset(cBuildLog , 0 , 10240);
      cout<< " cl Error \n"  ;
	  clGetProgramBuildInfo(cpProgram, cdDevice, CL_PROGRAM_BUILD_LOG, 
                          sizeof(cBuildLog), cBuildLog, NULL );
	  cout<< cBuildLog ;
     return NULL;
    }

    // Create the kernel
    _ck = clCreateKernel(cpProgram, kernelName, &ciErr1);
    if (ciErr1 != CL_SUCCESS)
    {
        return NULL;
    }
	ckKernel = _ck;
	return _ck;
}

 cl_mem getCLImage(unsigned char* RGB, int width, int height)
 {
	 return NULL;
 }

 void clDeleteBuffer(cl_mem buffer)
{
	clReleaseMemObject(buffer);
}

cl_kernel initKernel(char* kernelName)
{
	cl_kernel _ck;
	// Create the kernel
    _ck = clCreateKernel(cpProgram, kernelName, &ciErr1);
    if (ciErr1 != CL_SUCCESS)
    {
        return NULL;
    }
	
	return _ck;
}

cl_mem getDeviceBufferF(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
    cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

cl_mem getDeviceBufferC(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = 4;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
	cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_WRITE, szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

cl_mem getDeviceImage(int w, int h, void* buffer)
{
	cl_image_format image_format = {CL_RGBA, CL_UNSIGNED_INT8};
	cl_mem cmImg = clCreateImage2D(cxGPUContext,CL_MEM_READ_ONLY ,&image_format,w, h, 0,NULL,&ciErr1) ;

	size_t origin[3] = {0,0,0};
	size_t region[3] = {w,h,1};

	ciErr1 |= clEnqueueWriteImage(cqCommandQueue,cmImg,CL_TRUE,(const size_t*)&origin,(const size_t*)&region,0,0,buffer,0,NULL,NULL);
	if (ciErr1) showLastError(ciErr1);
	return cmImg;

}

cl_mem getDeviceBufferCRO(int iNumElements, void* buffer)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
	cl_mem cmDevSrcA;
	/*if (buffer)
	  cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR  , sizeof(cl_uchar) * iNumElements, buffer, &ciErr1);
	else
	*/
	cmDevSrcA = clCreateBuffer(cxGPUContext,  CL_MEM_READ_ONLY   , sizeof(cl_uchar) * iNumElements, NULL, &ciErr1);
	
	return cmDevSrcA;
}


cl_mem getDeviceBufferI(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
    cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_int) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

cl_mem getDeviceBufferF4(int iNumElements)
{
	 // set and log Global and Local work size dimensions
    szLocalWorkSize = GPU_LOCAL_WORK_SIZE;
    szGlobalWorkSize = RoundUp((int)szLocalWorkSize, iNumElements);  // rounded up to the nearest multiple of the LocalWorkSize

	 // Allocate the OpenCL buffer memory objects for source and result on the device GMEM
    cl_mem cmDevSrcA = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float4) * szGlobalWorkSize, NULL, &ciErr1);
	return cmDevSrcA;
}

//////////////////////////////////////////////////////////////////////////////
//! Get and log the binary (PTX) from the OpenCL compiler for the requested program & device
//!
//! @param cpProgram                   OpenCL program
//! @param cdDevice                    device of interest
//! @param const char*  cPtxFileName   optional PTX file name
//////////////////////////////////////////////////////////////////////////////
void clLogPtx(cl_program cpProgram, cl_device_id cdDevice, const char* cPtxFileName)
{
    // Grab the number of devices associated with the program
    cl_uint num_devices;
    clGetProgramInfo(cpProgram, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &num_devices, NULL);

    // Grab the device ids
    cl_device_id* devices = (cl_device_id*) malloc(num_devices * sizeof(cl_device_id));
    clGetProgramInfo(cpProgram, CL_PROGRAM_DEVICES, num_devices * sizeof(cl_device_id), devices, 0);

    // Grab the sizes of the binaries
    size_t* binary_sizes = (size_t*)malloc(num_devices * sizeof(size_t));    
    clGetProgramInfo(cpProgram, CL_PROGRAM_BINARY_SIZES, num_devices * sizeof(size_t), binary_sizes, NULL);

    // Now get the binaries
    char** ptx_code = (char**)malloc(num_devices * sizeof(char*));
    for( unsigned int i=0; i<num_devices; ++i)
    {
        ptx_code[i] = (char*)malloc(binary_sizes[i]);
    }
    clGetProgramInfo(cpProgram, CL_PROGRAM_BINARIES, 0, ptx_code, NULL);

    // Find the index of the device of interest
    unsigned int idx = 0;
    while((idx < num_devices) && (devices[idx] != cdDevice)) 
    {
        ++idx;
    }
    
    // If the index is associated, log the result
    if(idx < num_devices)
    {
         
        // if a separate filename is supplied, dump ptx there 
        if (NULL != cPtxFileName)
        {
           
            FILE* pFileStream = NULL;
            #ifdef _WIN32
                fopen_s(&pFileStream, cPtxFileName, "wb");
            #else
                pFileStream = fopen(cPtxFileName, "wb");
            #endif

            fwrite(ptx_code[idx], binary_sizes[idx], 1, pFileStream);
            fclose(pFileStream);        
        }
        else // log to logfile and console if no ptx file specified
        {
           //shrLog("\n%s\nProgram Binary:\n%s\n%s\n", HDASHLINE, ptx_code[idx], HDASHLINE);
        }
    }

    // Cleanup
    free(devices);
    free(binary_sizes);
    for(unsigned int i = 0; i < num_devices; ++i)
    {
        free(ptx_code[i]);
    }
    free( ptx_code );
}
