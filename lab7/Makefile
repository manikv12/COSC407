all: clean serial cuda

serial:
	clang -o serial serial.c
cuda:
	nvcc -o cuda cuda.cu
clean:
	rm -f serial cuda
