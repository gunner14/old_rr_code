INCLUDE=-I/usr/local/include -I. -I.. -I../cwf/src#`curl-config --cflags` . 
LIB=-L/usr/local/lib -lpthread  -lrt -lz -lboost_system -lboost_thread#`curl-config --libs` 
#LIB=-L/usr/local/lib #-lpthread

CC=g++ -g -Wall 

CXXFILES_TEST=upload_post.cc download.cc ../cwf/src/base3/logging.cc ../cwf/src/base3/string_piece.cc
OFILES_TEST=${CXXFILES_TEST:.cc=.o}

CXXFILES_GET=get.cc syncdownload.cc ../cwf/src/base3/logging.cc ../cwf/src/base3/string_piece.cc
OFILES_GET=${CXXFILES_TEST:.cc=.o}

get:$(CXXFILES_GET)
	$(CC) -o $@ $(CXXFILES_GET) $(INCLUDE) $(LIB) 

upload_post:$(CXXFILES_TEST)
	$(CC) -o $@ $(CXXFILES_TEST) $(INCLUDE) $(LIB) 

clear:
	rm -rf *.o upload_test get
