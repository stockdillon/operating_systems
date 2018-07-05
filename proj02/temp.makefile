makebuild: clean temp

temp: temp.cpp
	g++ -Wall temp.cpp -o temp

clean:
	rm -f temp

