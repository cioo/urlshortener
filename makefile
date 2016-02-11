all: clean build

build: dbtest

dbtest:
	g++ -o dbtest dbtest.cpp -I/usr/include/mysql -lmysqlclient -std=c++11 -L/usr/lib64/mysql -lmysqlpp -lcgicc
	
clean:
	rm -rf dbtest

