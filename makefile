all: clean shortener test

shortener: urlshortener

test: dbtest

urlshortener:
	g++ -o urlshortener app.cpp DBManager.cpp Urlshortener.cpp -I/usr/include/mysql -lmysqlclient -std=c++11 -L/usr/lib64/mysql -lmysqlpp -lcgicc

dbtest:
	g++ -o dbtest dbtest.cpp -I/usr/include/mysql -lmysqlclient -std=c++11 -L/usr/lib64/mysql -lmysqlpp -lcgicc
	
clean:
	rm -rf dbtest urlshortener


install: urlshortener
	cp urlshortener /var/www/cgi-bin/urlshortener
	