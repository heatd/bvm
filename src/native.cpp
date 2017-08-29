#include <native.h>

#ifdef __unix__

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

namespace Native
{

unsigned char *MapWholeFile(std::string filename, size_t *len)
{
	int fd = open(filename.c_str(), O_RDONLY);
	if(fd < 0)
		return NULL;
	size_t size_of_file = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	/* Map the file */
	unsigned char *byte = (unsigned char *) mmap(NULL,
						     size_of_file,
						     PROT_READ | PROT_WRITE,
						     MAP_PRIVATE,
						     fd,
						     0);
	 close(fd);
	 *len = size_of_file;
	 return byte;
}

void UnmapWholeFile(unsigned char *bytecode, size_t len)
{
	munmap(bytecode, len);
}

};
#endif
