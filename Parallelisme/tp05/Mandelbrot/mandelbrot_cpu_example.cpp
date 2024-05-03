#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <complex>
//
static const std::uint8_t MaxIteration = 255;  //!< Le nombre max d'itération est 255, soit de base le blanc.

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
std::vector<std::uint8_t> mandel(  const size_t width, 
                                    const size_t height)
{
    const double aspect = static_cast<double>(width) / static_cast<double>(height);
    std::vector<std::uint8_t> ret(width * height, 0);
    // Il faut boucler sur chaque pixel de l'image.
    for(unsigned int index = 0U; index < width * height; ++index)
    {
        double myrow = static_cast<double>(index) / static_cast<double>(width);
        double mycol = static_cast<double>(index % width);
        myrow /= height;
        mycol /= width;
        std::complex<double> z0{aspect * (2 * mycol - 1) - 0.5, 2 * myrow - 1 };
        double mag = 0.0;
        std::complex<double> z;
        std::uint8_t lim = MaxIteration;
        // On itère ensuite sur la suite via la relation :
        // z_0     = une valeur entre [0, 1] selon l'indice du pixel
        // z_{n+1} = z_n * z_n + z_0, n > 0
        // Si |z_{n}| > 2, la suite diverge on peut donc arrêter les itérations.
        // Si |z_{n}| <= 2, on est toujours dans l'ensemble de Mandelbrot. Comme par nature il est infini, 
        // on vérifie alors que le nombre d'itérations max. n'est pas atteint.
        do{
                z = z * z + z0;
                mag = std::norm(z);
            } while (lim-- && mag < 4.0);
        ret[index] = lim;
    }
    return ret;
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
    auto t0 = std::chrono::high_resolution_clock::now();
    const std::vector<std::uint8_t>& img = mandel(width, height);
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Generation of Mandelbrot set for image size " << width << " x " << height << " took "
              << std::chrono::duration<double>(t1-t0).count() << " seconds (CPU version)\n";
    save_pgm("output_CPU.pgm", width, height, img);
}
