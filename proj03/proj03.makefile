makebuild: clean proj03

proj03: proj03.student.c
	g++ -Wall proj03.student.c -o proj03

clean:
	rm -f proj03


