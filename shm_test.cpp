#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <sys/stat.h>

using namespace std;
using namespace boost::interprocess;

struct ProgramPosition{
	char name[100];
	int pid;
	double latitude;
	double longitude;
};

boost::interprocess::shared_memory_object mShmobj;
size_t mShmSize;
struct ProgramPosition TestPosRd;

int sharedMemoryWrite(void * BufferData, int Offset, size_t size)
{
	if( (size <= 0) || (size > 1024) ) {
		cout << "Wrong passed Size " << size << endl;
		return -1;
	}
	
	if( (Offset < 0) || ((Offset + size) >= 1024) ) {
		cout << "Wrong passed Offset = " << Offset << endl;
		return -2;
	}
     //Map the whole shared memory in this process
   mapped_region region(mShmobj,read_write, Offset, size);
   
   if(region.get_address() == NULL) {
		return -3;
   }

   //Obtain the shared structure
   memcpy(region.get_address(), BufferData, size);
   
   return size;
}

int sharedMemoryRead(void * BufferData, int Offset, size_t size)
{
	if( (size <= 0) || (size > 1024) ) {
		cout << "Wrong passed Size" << endl;
		return -1;
	}
	
	if( (Offset < 0) || ((Offset + size) >= 1024) ) {
		cout << "Wrong passed Offset" << endl;
		return -2;
	}

	//map the whole shared memory in this process
	mapped_region region(mShmobj,read_write, Offset, size);

	//get pointer to data
	void *mem_map = static_cast<void*>(region.get_address());
	
	if(mem_map == NULL) {
		cout << "Cannot read From SHM" << endl;
		return -3;
	}
	
	memcpy(BufferData, mem_map, size);

	return size;
}


int main()
{
	std::string mshmName("shmNew1");
	try {
		mShmobj = shared_memory_object(open_only, mshmName.c_str(), read_write);
		int Rdsize = sharedMemoryRead((void*)&TestPosRd, 0, sizeof(TestPosRd));
		if(Rdsize > 0) {
			cout << "Read SHM Data Amount = " <<  Rdsize << endl;
			cout << "Read Struct Name = " << TestPosRd.name << endl;
			cout << "Read Struct Pid = " << TestPosRd.pid << endl;
			cout << "Read Struct Latitude = " << TestPosRd.latitude << endl;
			cout << "Read Struct Longitude = " << TestPosRd.longitude << endl;
		}else {
			cout << "Cannot read from Shared Memory" << endl;
		}
		TestPosRd.latitude  +=  0.10;
		TestPosRd.longitude +=  0.10;
		int Wrsize = sharedMemoryWrite((void*)&TestPosRd, 0, sizeof(TestPosRd));
		if(Wrsize < 0) {
			cout << "Cannot Write To Shared Memory" << endl;
		} else {
			cout << "GPS Data Updated successfully to Shared Memory" << endl;
		}
	} catch (interprocess_exception &e) {
		cout << "Error processing Shared Memory = " << e.get_error_code() << endl;
		return 1;
	}

	return 0;        
}
