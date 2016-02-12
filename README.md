# urlshortener
Service to shorten URLs

This app is built using C++11 and depends on mysql++-devel & cgicc. Please install required packages before compiling the app. 

to build:
make

to install cgi app to cgi-bin folder:
sudo make install


If you would like programmatically retrieve a new short url go to: http://ec2-52-53-217-102.us-west-1.compute.amazonaws.com/cgi-bin/urlshortener?add={your url goes here}
Repsonse of this request will be shortened url.
