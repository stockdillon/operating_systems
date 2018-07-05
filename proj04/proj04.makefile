makebuild: clean proj04

proj04: proj04.student.c
	g++ -std=c++11 -Wall proj04.student.c -o proj04

clean:
	rm -f proj04


