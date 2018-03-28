# voa
the is tool is designed to download VOA audio resource 

# install
1. install depend library: 
[libcurl](https://github.com/curl/curl)
[gumbo-parser](https://github.com/google/gumbo-parser)
[args](https://github.com/Taywee/args)

command to download all modules:
```
git submodule update --init --recursive
cd third_party/<module>
make & make install
```

2. make & make install


# Usage
```
  ./voa [Url] {OPTIONS}

  VOA English audio & video downloader
  
  OPTIONS:

    -h, --help                        Display this help menu
    -e[Extension], --ext=[Extension]  The file extension
    -f[Filter], --filter=[Filter]     The file filter
    Url                               the target url location
    "--" can be used to terminate flag options and force all following
    arguments to be treated as positional options

    Welcome to contribute and share with friends.

```

example:
```
./voa --ext=mp3 --filter=hq https://learningenglish.voanews.com/z/1689

```

### proxy setup
all_proxy=proxy.company.com:8080

**more detail please ref curl document:
The proxy environment variable contents should be in the format `"[protocol://][user:password@]machine[:port]"`. Where the protocol:// part is simply ignored if present (so http://proxy and bluerk://proxy will do the same) and the optional port number specifies on which port the proxy operates on the host. If not specified, the internal default port number will be used and that is most likely *not* the one you would like it to be.

There are two special environment variables. 'all_proxy' is what sets proxy for any URL in case the protocol specific variable wasn't set, and 'no_proxy' defines a list of hosts that should not use a proxy even though a variable may say so. If 'no_proxy' is a plain asterisk `("*")` it matches all hosts.


# Compile
compile the source in command 
```
compile single module and fix errors:
g++ -S -std=c++11 -I /usr/include/curl  voa_html.cpp

compile directive:
g++ -o voa -std=c++11 -l curl -l gumbo -I /usr/include/curl -I /usr/local/include  voa.cpp voa_html.cpp voa_curl.cpp

compile directive with debug
g++ -o voa -g -std=c++11 -l curl -l gumbo -I /usr/include/curl -I /usr/local/include  voa.cpp voa_html.cpp voa_curl.cpp
```

