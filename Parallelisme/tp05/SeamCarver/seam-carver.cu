#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

// Handle error ///////////////////////////////////////////////////////////////

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

// ENERGY CPU /////////////////////////////////////////////////////////////////////

unsigned int energy_at_cpu(
    const unsigned char *data,
    int w,
    int h,
    int x,
    int y) {
int x0 = x == 0 ? x : x - 1;
int x1 = x == w - 1 ? x : x + 1;
int ix0 = (y * w + x0) * 3;
int ix1 = (y * w + x1) * 3;
unsigned int dxr = data[ix0    ] - data[ix1    ];
unsigned int dxg = data[ix0 + 1] - data[ix1 + 1];
unsigned int dxb = data[ix0 + 2] - data[ix1 + 2];
unsigned int dx = dxr * dxr + dxg * dxg + dxb * dxb;

int y0 = y == 0 ? y : y - 1;
int y1 = y == h - 1 ? y : y + 1;
int iy0 = (y0 * w + x) * 3;
int iy1 = (y1 * w + x) * 3;
unsigned int dyr = data[iy0    ] - data[iy1    ];
unsigned int dyg = data[iy0 + 1] - data[iy1 + 1];
unsigned int dyb = data[iy0 + 2] - data[iy1 + 2];
unsigned int dy = dyr * dyr + dyg * dyg + dyb * dyb;

return dx + dy;
}

unsigned int * compute_energy_cpu(const unsigned char *data, int w, int h) {
unsigned int *energy = (unsigned int *)malloc(w * h * sizeof(unsigned int));
if (!energy) {
    fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);
    return NULL;
}

for (int y = 0; y < h; y++)
{
    for (int x = 0; x < w; x++) 
    {
        int i = y * w + x;
        energy[i] = energy_at_cpu(data, w, h, x, y);
    }
}

return energy;
}

// ENERGY GPU /////////////////////////////////////////////////////////////////////

//compute energy with GPU version
__global__ void compute_energy_kernel(
    unsigned char * data_GPU,
    unsigned int * energy_GPU,
    int w,
    int h
)
{
    // Get x and y
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    // Dimension test
     if(x < w && y < h)
     {
         // Compute energy_GPU
       int x0 = x == 0 ? x : x - 1;
       int x1 = x == w - 1 ? x : x + 1;
       int ix0 = (y * w + x0) * 3;
       int ix1 = (y * w + x1) * 3;
       unsigned int dxr = data_GPU[ix0    ] - data_GPU[ix1    ];
       unsigned int dxg = data_GPU[ix0 + 1] - data_GPU[ix1 + 1];
       unsigned int dxb = data_GPU[ix0 + 2] - data_GPU[ix1 + 2];
       unsigned int dx = dxr * dxr + dxg * dxg + dxb * dxb;

       int y0 = y == 0 ? y : y - 1;
       int y1 = y == h - 1 ? y : y + 1;
       int iy0 = (y0 * w + x) * 3;
       int iy1 = (y1 * w + x) * 3;
       unsigned int dyr = data_GPU[iy0    ] - data_GPU[iy1    ];
       unsigned int dyg = data_GPU[iy0 + 1] - data_GPU[iy1 + 1];
       unsigned int dyb = data_GPU[iy0 + 2] - data_GPU[iy1 + 2];
       unsigned int dy = dyr * dyr + dyg * dyg + dyb * dyb;

       energy_GPU[y * w + x] = dx + dy;

     }
}

unsigned int * compute_energy(const unsigned char *data, int w, int h) {
    // grid and block size
    dim3 block_size(16,16);
    dim3 grid_size((w + block_size.x - 1) / block_size.x, (h + block_size.y - 1) / block_size.y);

    // Allocation
    int size = w * h;
    unsigned char * data_GPU = nullptr;
    unsigned int * energy_GPU = nullptr;
    HANDLE_ERROR(cudaMalloc((void**)&data_GPU, size * 3 * sizeof(unsigned char)));
    HANDLE_ERROR(cudaMalloc((void**)&energy_GPU, size * sizeof(unsigned int)));

    // Fill data_GPU with data
    HANDLE_ERROR(cudaMemcpy(data_GPU, data, size * 3 * sizeof(unsigned char), cudaMemcpyHostToDevice));

    // Call kernel
    compute_energy_kernel<<<grid_size, block_size>>>(data_GPU, energy_GPU, w, h);

    // Get energy map back to CPU
    unsigned int *energy = (unsigned int *)malloc(w * h * sizeof(unsigned int));
    if (!energy) {
        fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);
        return NULL;
    }
    // Fill energy with energy_GPU
    HANDLE_ERROR(cudaMemcpy(energy, energy_GPU, size * sizeof(unsigned int), cudaMemcpyDeviceToHost));

    // Free GPU memory
    HANDLE_ERROR(cudaFree(data_GPU));
    HANDLE_ERROR(cudaFree(energy_GPU));
    
    return energy;
}

// SEAMS //////////////////////////////////////////////////////////////////////

struct seam_link {
    // The X and Y coordinates of the link are inferred by the position of the
    // link in a links array.

    // The minimal energy for any connected seam ending at this position.
    unsigned int energy;

    // The parent X coordinate for vertical seams, Y for horizontal seams.
    int parent_coordinate;
};

struct seam_link * compute_vertical_seam_links(
        const unsigned int *energy,
        int w,
        int h) {
    struct seam_link *links = (seam_link *)malloc(w * h * sizeof(struct seam_link));
    if (!links) {
        fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);
        return NULL;
    }

    for (int x = 0; x < w; x++) {
        links[x] = (struct seam_link) {
            .energy = energy[x],
            .parent_coordinate = -1
        };
    }

    for (int y = 1; y < h; y++)
    for (int x = 0; x < w; x++) {
        int i = y * w + x;

        int min_parent_energy = INT_MAX;
        int min_parent_x = -1;

        int parent_x = x == 0 ? x : x - 1;
        int parent_x_end = x == w - 1 ? x : x + 1;
        for (; parent_x <= parent_x_end; parent_x++) {
            int candidate_energy = links[(y - 1) * w + parent_x].energy;
            if (candidate_energy < min_parent_energy) {
                min_parent_energy = candidate_energy;
                min_parent_x = parent_x;
            }
        }

        links[i] = (struct seam_link) {
            .energy = energy[i] + min_parent_energy,
            .parent_coordinate = min_parent_x
        };
    }

    return links;
}

int * get_minimal_seam(
        const struct seam_link *seam_links,
        int num_seams,
        int seam_length) {
    int *minimal_seam = (int *)malloc(seam_length * sizeof(int));
    if (!minimal_seam) {
        fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);
    }

    int min_coordinate = -1;
    int min_energy = INT_MAX;

    for (int coordinate = 0; coordinate < num_seams; coordinate++) {
        int i = num_seams * (seam_length - 1) + coordinate;
        if (seam_links[i].energy < min_energy) {
            min_coordinate = coordinate;
            min_energy = seam_links[i].energy;
        }
    }

    int i = 0;
    int offset = min_coordinate;

    for (int d = 0; d < seam_length; d++) {
        minimal_seam[i++] = offset;

        struct seam_link end =
            seam_links[num_seams * (seam_length - 1 - d) + offset];

        offset = end.parent_coordinate;
    }

    return minimal_seam;
}

// REMOVAL ////////////////////////////////////////////////////////////////////

unsigned char * image_after_vertical_seam_removal(
        const unsigned char *original_data,
        const int *vertical_seam,
        int w,
        int h) {
    unsigned char *img = (unsigned char *)malloc((w - 1) * h * 3);
    if (!img) {
        fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);
        return NULL;
    }

    for (int y = 0; y < h; y++) {
        int seamx = vertical_seam[h - 1 - y];

        for (int x = 0, imgx = 0; imgx < w - 1; x++, imgx++) {
            if (x == seamx) { x++; }

            int    i = (y *  w      + x   ) * 3;
            int imgi = (y * (w - 1) + imgx) * 3;

            img[imgi    ] = original_data[i    ];
            img[imgi + 1] = original_data[i + 1];
            img[imgi + 2] = original_data[i + 2];
        }
    }

    return img;
}

// OUTPUT /////////////////////////////////////////////////////////////////////

int write_energy(
        const unsigned int *energy,
        int w,
        int h,
        const char *filename) {
    int result = 0;

    unsigned char *energy_normalized = (unsigned char *)malloc(w * h);
    if (!energy_normalized) {
        fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);

        result = 1;
        //goto cleanup;
    }

    int max_energy = 1;
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
        int i = y * w + x;
        max_energy = energy[i] > max_energy ? energy[i] : max_energy;
    }

    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
        int i = y * w + x;
        energy_normalized[i] = (char) ((double) energy[i] / max_energy * 255);
    }

    printf("Writing to '%s'\n", filename);
    if (!stbi_write_jpg(filename, w, h, 1, energy_normalized, 80)) {
        fprintf(stderr, "Unable to write output (%d)\n", __LINE__);

        result = 1;
    }

    return result;
}

int draw_vertical_seam(
        const unsigned char *data,
        const int *minimal_vertical_seam,
        int w,
        int h,
        const char *filename) {
    int result = 0;

    unsigned char *data_with_seams = (unsigned char *)malloc(w * h * 3);
    if (!data_with_seams) {
        fprintf(stderr, "Unable to allocate memory (%d)\n", __LINE__);

        result = 1;
    }

    memcpy(data_with_seams, data, w * h * 3);

    for (int y = h - 1; y >= 0; y--) {
        int x = minimal_vertical_seam[h - 1 - y];
        int i = (y * w + x) * 3;

        data_with_seams[i    ] = 255;
        data_with_seams[i + 1] = 0;
        data_with_seams[i + 2] = 0;
    }

    printf("Writing to '%s'\n", filename);
    if (!stbi_write_jpg(filename, w, h, 3, data_with_seams, 80)) {
        fprintf(stderr, "Unable to write output (%d)\n", __LINE__);

        result = 1;
    }

    return result;
}

int draw_image(
        const unsigned char *data,
        int w,
        int h,
        const char *filename) {
    printf("Writing %dx%d image to '%s'\n", w, h, filename);
    return stbi_write_jpg(filename, w, h, 3, data, 80);
}

// MAIN ///////////////////////////////////////////////////////////////////////

void show_usage(const char *program) {
    fprintf(
            stderr,
            "USAGE:\n"
            "  %s <input-filename> <output-directory> <num-iterations> <gpu?>\n",
            program);
}

unsigned char * run_iteration(
        const char *output_directory,
        const unsigned char *data,
        int w,
        int h,
        int iteration,
        bool use_gpu) {

    unsigned int *energy = NULL;
    struct seam_link *vertical_seam_links = NULL;
    int *minimal_vertical_seam = NULL;
    unsigned char *output_data = NULL;

    char output_filename[1024];

    if (use_gpu) {
        energy = compute_energy(data, w, h);
    } else {
        energy = compute_energy_cpu(data, w, h);
    }

    if (!energy) { }

    if (iteration == 0) {
        snprintf(output_filename , 1024, "%s/img-energy%d.jpg", output_directory, iteration);
        if (write_energy(energy, w, h, output_filename)) {
        }
    }

    vertical_seam_links = compute_vertical_seam_links(energy, w, h);
    free(energy);
    if (!vertical_seam_links) { }

    minimal_vertical_seam = get_minimal_seam(vertical_seam_links, w, h);
    free(vertical_seam_links);

    snprintf(
            output_filename,
            1024,
            "%s/img-seam-%04d.jpg",
            output_directory,
            iteration);
    printf("Iteration: %d\n", iteration);
    // write output image 
    // if (draw_vertical_seam(
    //             data,
    //             minimal_vertical_seam,
    //             w,
    //             h,
    //             output_filename)) {
    // }

    output_data =
        image_after_vertical_seam_removal(data, minimal_vertical_seam, w, h);

    return output_data;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        show_usage(argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_directory = argv[2];
    int num_iterations = atoi(argv[3]);

    bool use_gpu = strcmp(argv[4],"gpu") == 0;

    int result = 0;

    unsigned char *initial_img = NULL;
    unsigned char *data = NULL;

    printf("Reading '%s'\n", input_filename);

    int w, h, n;
    initial_img = stbi_load(input_filename, &w, &h, &n, 3);
    if (!initial_img) {
        fprintf(stderr, "Unable to read '%s'\n", input_filename);

        result = 1;
    }

    printf("Loaded %dx%d image\n", w, h);

    auto t0 = std::chrono::high_resolution_clock::now();
    data = initial_img;
    for (int i = 0; i < num_iterations; i++) {
        unsigned char *next_data =
            run_iteration(output_directory, data, w, h, i, use_gpu);

        if (!next_data) {
            fprintf(stderr, "Error running iteration %d\n", i);

            result = 1;
        }

        if (i > 0) { free(data); }
        data = next_data;
        w--;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Seam carving image compute in " << std::chrono::duration<double>(t1-t0).count() << " seconds\n";    

    char resized_output_filename[1024];
    snprintf(resized_output_filename, 1024, "%s/img-%s.jpg", output_directory, use_gpu ? "gpu" : "cpu");
    if (!draw_image(data, w, h, resized_output_filename)) {
        fprintf(
                stderr,
                "\033[1;31mUnable to write %s\033[0m\n",
                resized_output_filename);
    }

    return result;
}
