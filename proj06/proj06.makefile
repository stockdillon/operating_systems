makebuild: clean proj06

proj06: proj06.student.c
	g++ -std=c++11 -Wall proj06.student.c -o proj06

clean:
	rm -f proj06


