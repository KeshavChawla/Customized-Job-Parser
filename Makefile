job_parser.o: job_parser.cc
	g++ -c -o $@ $< -std=c++11 -stdlib=libc++
	g++ -o job job_parser.o

.PHONY: clean

clean:
	rm job_parser.o job