# voa
the is tool is designed to download VOA audio resource 


compile single module and fix errors:
g++ -S -std=c++11 -I /usr/include/curl  voa_html.cpp


compile directive:
g++ -o voa -std=c++11 -l curl -l gumbo -I /usr/include/curl -I /usr/local/include  voa.cpp voa_html.cpp voa_curl.cpp

compile directive with debug
g++ -o voa -g -std=c++11 -l curl -l gumbo -I /usr/include/curl -I /usr/local/include  voa.cpp voa_html.cpp voa_curl.cpp


test command line:
./voa http://view.news.qq.com/original/intouchtoday/n4191.html
./voa https://learningenglish.voanews.com/z/1689
