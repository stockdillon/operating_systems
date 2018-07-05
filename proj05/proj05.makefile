makebuild: clean proj05

proj05: proj05.student.c
	g++ -std=c++11 -Wall proj05.student.c -o proj05

clean:
	rm -f proj05


