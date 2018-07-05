makebuild: clean proj08

proj08: proj08.student.c
	g++ -std=c++11 -Wall proj08.student.c -o proj08

clean:
	rm -f proj08


