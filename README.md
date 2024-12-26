# Local Binary Patterns (LBP) Image Comparison

## Project Overview
This project is a practical assignment for the Programming II course at the Federal University of Paraná. It involves implementing a C program to compare images based on their Local Binary Patterns (LBP) representation. The program supports grayscale images in the Portable Gray Map (PGM) format and aims to find the most similar image from a reference dataset.

## Features
The project implements the following functionalities:

1. **Image Comparison**: Compares a test image with a reference image dataset to find the most similar image.
    - The program calculates the Euclidean distance between LBP histograms of the images.
    - Outputs the name of the most similar image and its distance.

2. **LBP Image Generation**: Generates the LBP representation of an input image.
    - Creates a new image where each pixel value corresponds to its LBP value.

## Program Usage
The program offers two primary functionalities:

### 1. Image Comparison
Compares a test image with images in a reference dataset.

```bash
./lbp -d ./base -i img1.tif
```

**Output**:
```
Imagem mais similar: <most_similar_image> <distance>
```
- `<most_similar_image>`: The name of the most similar image in the dataset.
- `<distance>`: The Euclidean distance (6 decimal places).

### 2. LBP Image Generation
Generates an LBP image from an input grayscale image.

```bash
./lbp -i img1.tif -o img_out.tif
```

- Input image: `img1.tif`.
- Output image: `img_out.tif`.

**Note**: The output image format (P2 or P5) will match the format of the input image.

## Implementation Details

### Local Binary Patterns (LBP)
- For each pixel in the input image, an 8-pixel neighborhood is considered.
- Pixel values are thresholded based on the central pixel value:
    - Greater than or equal: `1`
    - Less than: `0`
- The binary values are multiplied by weights (powers of 2) to calculate the LBP value.
- A histogram of LBP values (0-255) is generated to represent the image.

### PGM Format Support
- The program supports PGM images in both P2 (ASCII) and P5 (binary) formats.
- Comments (lines starting with `#`) are ignored during processing.

### Optimization
- LBP calculations are stored in binary files (`.lbp`) to avoid redundant computation.
- If a `.lbp` file exists for a reference image, it is used directly for comparison.

## Code Structure

- **Makefile**: Used to compile the project.
- **README.md**: Project documentation.

### Compilation
To compile the program, use the provided Makefile:

```bash
make
```

This generates an executable named `LBP`.

### Dependencies
- The project requires a standard C compiler (e.g., GCC).
- No external libraries are needed.


## Testing
- Use the provided Makefile to run unit tests.

## Example Output
### Image Comparison
```bash
./lbp -d ./base -i test_image.pgm
Imagem mais similar: ref_image.pgm 0.003245
```

### LBP Image Generation
```bash
./lbp -i test_image.pgm -o lbp_image.pgm
```

## Submission Requirements
The following files are included in the submission:

1. All source code files.
2. A `Makefile` to compile the program.
3. This `README.md` for project documentation.
