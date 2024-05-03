#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
//
#include <cuComplex.h>
//
#define MaxIteration 255;  //!< Le nombre max d'itération est 255, soit de base le blanc.
//
static void HandleError(	cudaError_t err,
                            const char *file,
                            int line )
{
    if (err != cudaSuccess)
    {
        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
        file, line );
        exit( EXIT_FAILURE );
    }
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))
// Cette méthode sert uniquement à sauvegarder le vecteur sous forme d'une image en niveau de gris sur 8 bits.
void save_pgm(  const char*                         filename,
                const size_t                        width,
                const size_t                        height,
                const std::vector<std::uint8_t>&   data)
{
    std::ofstream fout{ filename };
    // L'en-tête
    fout << "P2\n" << width << " " << height << " 255\n";
    for (size_t row = 0; row < height; ++row)
    {
        for (size_t col = 0; col < width; ++col)
        {
            fout << (col ? " " : "")
                 << static_cast<unsigned>(data[row * width + col]);
        }
        fout << "\n";
    }
    fout.close();
}
//
__global__ void mandel_kernel_double(int width, int height, uint8_t* ret)
{
    // TODO : garder le même algorithme qu'en C++, il faut juste "traduire" les éléments
    // de C++ en CUDA.
    // Que devient la boucle 'for' si on souhaite calculer chaque pixel dans un thread ?
    // Attention à l'indice global du thread, il faut vérifier que nous sommes bien dans les bornes de l'image !
    // Utiliser les méthodes et les éléments fournis dans le PDF concernant les nombres complexes avec CUDA 
    // (on peut aussi explorer 'cuComplex.h' pour voir ce qui s'y trame).

  double aspect = (double) width / height;
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  if (x < width && y < height)
  {
    double myrow = (double) y;
    double mycol = (double) x;
    myrow /= height;
    mycol /= width;
    cuDoubleComplex z0 = make_cuDoubleComplex(aspect * (2 * mycol - 1) - 0.5, 2 * myrow - 1);
    double mag = 0.0;
    cuDoubleComplex z = make_cuDoubleComplex(0.0, 0.0);
    uint8_t lim = MaxIteration;
    do
    {
      z = cuCadd(cuCmul(z, z), z0);
      mag = cuCabs(z);
    } while (lim-- && mag < 4.0);
    ret[y * width + x] = lim;
  }
}
//
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage:\n"
                  << argv[0] << " [width] [height]\n";
        return 1;
    }
    const size_t width  = std::stoul(argv[1]);
    const size_t height = std::stoul(argv[2]);
    std::vector<std::uint8_t> image(height * width, 0);
    // Note : il est possible de manipuler le pointeur de données sous-jacent au vecteur via la méthode '.data()'
    auto t0 = std::chrono::high_resolution_clock::now();
    // TODO : Appeler mandel_kernel_double
    uint8_t *d_image;
    HANDLE_ERROR(cudaMalloc(&d_image, width * height * sizeof(uint8_t)));
    HANDLE_ERROR(cudaMemcpy(d_image, image.data(), width * height * sizeof(uint8_t), cudaMemcpyHostToDevice));
    int blockSize = 32;
    dim3 dimGrid((width + blockSize - 1) / blockSize, (height + blockSize - 1) / blockSize);

    mandel_kernel_double<<<dimGrid, dim3(blockSize, blockSize)>>>(width, height, d_image);
    HANDLE_ERROR(cudaMemcpy(image.data(), d_image, width * height * sizeof(uint8_t), cudaMemcpyDeviceToHost));
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Generation of Mandelbrot set for image size " << width << " x " << height << " took "
              << std::chrono::duration<double>(t1-t0).count() << " seconds (GPU version)\n";
    save_pgm("output_GPU.pgm", width, height, image);
}
