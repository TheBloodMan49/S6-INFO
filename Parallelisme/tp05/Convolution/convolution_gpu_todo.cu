#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>

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

// Cette méthode ouvre une image pmg et la stocke dans un vecteur 8 bits.
std::vector<std::uint8_t> open_pgm(const std::string & filename, int& numrows, int& numcols)
{
  int row = 0, col = 0, tmp;
  std::ifstream infile{filename};
  std::stringstream ss;
  std::string inputLine = "";

  std::cout << "Reading input image..." << std::endl;

  // First line : version
  getline(infile,inputLine);
  if(inputLine.compare("P2") != 0) std::cerr << "Version error" << std::endl;
  else std::cout << "| Version : " << inputLine << std::endl;

  // Continue with a stringstream
  ss << infile.rdbuf();
  // Third line : size
  ss >> numcols >> numrows >> tmp;
  std::cout << "| " << numcols << " columns and " << numrows << " rows" << std::endl;

  std::vector<std::uint8_t> data(numcols * numrows);

  // Following lines : data
  for(row = 0; row < numrows; ++row)
  {
    for (col = 0; col < numcols; ++col) 
    {
      ss >> tmp;
      data[row*numcols + col] = tmp;
    }
  }

  infile.close();

  return data;
}

// Cette méthode sauvegarder un vecteur sous forme d'une image en niveau de gris sur 8 bits.
void save_pgm(  const std::string&                 filename,
                const size_t                       width,
                const size_t                       height,
                const std::vector<std::uint8_t>&   data)
{
    std::ofstream fout{ filename };
    // Header
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

    std::cout << "Save image done!" << std::endl;
}
//
__global__ void convol_kernel(/* TODO */)
{
 // TODO
}
//
std::vector<std::uint8_t> apply_convolution(
  const std::vector<std::uint8_t> & input_image, 
  int numrows, 
  int numcols, 
  int numiter)
{    
    // Allocate output image (same size) and usefull buffer
    dim3 block_size(16, 16);
    dim3 grid_size(numcols / block_size.x, numrows / block_size.y);
    // TODO
    
    // Copy data from RAM to VRAM
    // TODO
    
    // Call kernel
    for (int i = 0; i < numiter; i++)
    {
      convol_kernel<<<grid_size, block_size>>>(output_image_gpu, buffer_image_gpu, numrows, numcols);
      std::swap(output_image_gpu, buffer_image_gpu);
    }

    // Get resuts on RAM
    std::vector<std::uint8_t> output_image(input_image.size());
    HANDLE_ERROR(cudaMemcpy(output_image.data(), output_image_gpu, sizeof(std::uint8_t) * input_image.size(), cudaMemcpyDeviceToHost));

    std::cout << "Convolution ... done" << std::endl;
    return output_image;
}
//
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage:\n"
                  << argv[0] << " [PATH/image.pgm] [number of convolution iterations. ex:100]\n";
        return 1;
    }

    std::string input_file = argv[1];
    int num_iter = std::stoul(argv[2]);

    std::string output_file {"image_convolution_GPU.pgm"};

    int numrows, numcols;

    // Read input image
    const std::vector<std::uint8_t>& input_image = open_pgm(input_file, numrows, numcols);

    // Do convolution
    auto t0 = std::chrono::high_resolution_clock::now();
    const std::vector<std::uint8_t>& output_image = apply_convolution(input_image, numrows, numcols, num_iter);
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Image convolution done in " << std::chrono::duration<double>(t1-t0).count() << " seconds (GPU version)\n";    

    // Export output image
    save_pgm(output_file, numcols, numrows, output_image);
}
