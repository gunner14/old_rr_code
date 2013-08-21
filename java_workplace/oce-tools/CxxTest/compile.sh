find go -type f -exec rm {} \;
g++ -I/opt/Ice-3.3/include/ -I/usr/include/ -L/opt/Ice-3.3/lib -lIce -lIceUtil -lIceXML -lIceBox -lIceStorm -lIceGrid main.cpp -o go
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/Ice-3.3/lib/
