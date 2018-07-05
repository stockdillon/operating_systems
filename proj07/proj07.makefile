makebuild: clean proj07

proj07: proj07.student.c
	g++ -std=c++11 -Wall proj07.student.c -o proj07

clean:
	rm -f proj07


