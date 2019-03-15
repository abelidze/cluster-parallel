#include <iostream>

int main()
{
#ifdef _OPENMP
	std::cout << "OpenMP supported" << std::endl;
#endif

	return 0;
}
