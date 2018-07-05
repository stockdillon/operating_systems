makebuild: clean backup

backup: backup.cpp
	g++ -std=c++11 -Wall backup.cpp -o backup

clean:
	rm -f backup

