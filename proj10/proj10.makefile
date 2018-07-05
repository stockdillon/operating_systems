makebuild: clean proj10

proj10: proj10.student.c
	g++ -std=c++11 -Wall proj10.student.c -o proj10

clean:
	rm -f proj10

