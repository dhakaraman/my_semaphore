blocking: 
	gcc file1_2019014.c -o a1.out -lpthread && ./a1.out && rm a1.out

nonblocking:
	gcc file2_2019014.c -o a2.out -lpthread && ./a2.out && rm a2.out

