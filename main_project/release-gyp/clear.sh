if [ -d out ];
then 
  rm -r out
fi

if [ -d buddy ];
then
  rm -f buddy/*.Makefile buddy/*.mk
fi

if [ -d user ];
then
  rm -f user/*.Makefile user/*.mk
fi

if [ -d like ];
then
  rm -f like/*.Makefile like/*.mk
fi

if [ -d socialgraph ];
then
  rm -f socialgraph/*.Makefile socialgraph/*.mk
fi

if [ -d footprint ];
then
  rm -f footprint/*.Makefile footprint/*.mk
fi

if [ -d menu ];
then
  rm -f menu/*.Makefile menu/*.mk
fi

if [ -d score ];
then
  rm -f score/*.Makefile score/*.mk
fi

if [ -d bulletin ];
then
  rm -f bulletin/*.Makefile bulletin/*.mk
fi

if [ -d MiniGroup ];
then
  rm -f MiniGroup/*.Makefile MiniGroup/*.mk
fi

if [ -d FriendsRecentPhotos ];
then
  rm -f FriendsRecentPhotos/*.Makefile FriendsRecentPhotos/*.mk
fi

if [ -d test ];
then
  rm -f test/*.Makefile test/*.mk
fi

rm -f *.gypd
rm -f *.Makefile *.mk Makefile *.user *.sln *.vcproj *.suo *.ncb *.rules *.XML
rm -rf _UpgradeReport_Files
rm -f ../UtilSlice/slice/*.h ../UtilSlice/slice/*.cpp ../OceSlice/slice/*.h ../OceSlice/slice/*.cpp ../OceProto/proto/*.h ../OceProto/proto/*.cc
rm -f  ../DistCache/distributedcache/slice/*.h ../DistCache/distributedcache/slice/*.cpp ../DistCache/distributedcache/protobuf/*.h ../DistCache/distributedcache/protobuf/*.cc
#rm -rf out Default *.Makefile *.mk Makefile *.user *.sln *.vcproj *.suo *.ncb ../UtilSlice/slice/*.h ../UtilSlice/slice/*.cpp ../OceSlice/slice/*.h ../OceSlice/slice/*.cpp ../OceProto/proto/*.h ../OceProto/proto/*.cc

