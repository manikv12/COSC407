all: clean cuda

cuda:
	nvcc -lm -o julia julia.cu
clean:
	rm -f julia julia.ppm
