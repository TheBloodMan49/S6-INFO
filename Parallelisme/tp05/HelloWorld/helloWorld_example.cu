#include <iostream>
#include <stdio.h>

__global__ void cuda_hello()
{
    printf("Hello World from GPU!\n");
}
//
void c_hello()
{
    printf("Hello World from CPU in C!\n");
}
//
void cpp_hello()
{
    std::cout << "Hello World from CPU in C++!" << std::endl;
}
//
int main()
{
    // Call CUDA
    cuda_hello<<<1,1>>>(); 
    // Tester avec et sans cette ligne :
    cudaDeviceSynchronize();
    
    // Call CPU - C version
    c_hello();
    
    // Call CPU - C++ version
    cpp_hello();
    return 0;
}