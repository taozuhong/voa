bin_PROGRAMS = voa
voa_SOURCES = \
              src/voa.cpp \
              src/voa_html.h \
              src/voa_html.cpp \
              src/voa_curl.h \
              src/voa_curl.cpp \
              src/args.hxx \
              src/voa_common.h \
              src/voa_common.cpp

voa_LDADD = -lgumbo -lcurl
AM_CPPFLAGS=-I/usr/include/curl -I/usr/local/include -std=c++11 -g -O0

