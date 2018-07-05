makebuild: clean proj02

proj02: proj02.student.c
	g++ -Wall proj02.student.c -o proj02

clean:
	rm -f proj02


