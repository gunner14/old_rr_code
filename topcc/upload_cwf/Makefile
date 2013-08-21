#CC=g++ -Wall -g -D_DEBUG
CC=g++ -Wall -g -O2

INCLUDE= `curl-config --cflags` `Magick++-config --cflags --cppflags`
LIB=-lboost_system -lboost_thread -lfcgi `curl-config --libs` -ljson_linux-gcc-4.1.2_libmt -lMagick++ -lMagickWand -lMagickCore -ljpeg.ipp -lbz2 -lz -lm -lpthread -lmemcached

CXXFILES=main.cpp datatype.cpp infconfigparser.cpp runtimeconfig.cpp util.cpp imagehelper.cpp memcache.cpp downloader.cpp requestparser.cpp baseprocessor.cpp imageprocessor.cpp attachmentprocessor.cpp videoprocessor.cpp processdispatcher.cpp detect/download.cc detect/upload.cc detect/logging.cc
OFILES=${CXXFILES:.cpp=.o}
CXXFILES_TEST=$(CXXFILES:main.cpp=test.cpp)
OFILES_TEST=${CXXFILES_TEST:.cpp=.o}

#添加LD_LIBRARY_PATH,否则如果ldconfig -v找不到ipp库,make的父进程的LD_LIBRARY_PATH中也找不到ipp库,则会编译不过,这么不太好,运行的时候又会找不到环境变量
#应加在/etc/ld.so.conf,并执行ldconfig -v
export LD_LIBRARY_PATH=/usr/local/lib:/opt/intel/Compiler/11.1/046/lib/intel64:/opt/intel/Compiler/11.1/046/ipp/em64t/sharedlib
#source /opt/intel/Compiler/11.1/046/bin/iccvars.sh intel64 #会发现LD_LIBRARY_PATH的取值

SERVICE_NAME=uploadservice.fcgi
TEST_NAME=test

.SUFFIXES: .o .cpp
all:${SERVICE_NAME} ${TEST_NAME}
.cpp.o:	
	$(CC) ${INCLUDE} -c $< -o $@

${SERVICE_NAME}:$(OFILES)
	echo ${LD_LIBRARY_PATH}
	$(CC) -o $@ $(OFILES) $(LIB)
	cp -f $@ ../bin
${TEST_NAME}:$(OFILES_TEST)
	$(CC) -o $@ $(OFILES_TEST) $(LIB)

clean:
	rm *.o ${SERVICE_NAME} ${TEST_NAME}
