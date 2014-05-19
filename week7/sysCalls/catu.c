#include <unistd.h>

//calls system call interface exclusively

int main(int argc, char** argv)
{
	char buffer;
	while (read(0, &buffer, 1) > 0)
		write(1, &buffer, 1);
	return 0;
}
