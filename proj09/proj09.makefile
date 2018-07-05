makebuild: clean proj09

proj09: proj09.student.c
	g++ -std=c++11 -Wall proj09.student.c -o proj09

clean:
	rm -f proj09

