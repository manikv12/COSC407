all: clean serial cuda
serial:
	gcc -lm -o edt_serial edt_serial.c
cuda:
	nvcc -arch=sm_13 -o edt_cuda edt_cuda.cu
clean:
	rm -f edt_serial edt_cuda
