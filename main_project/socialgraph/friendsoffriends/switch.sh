#!/bin/sh

if [ $# != 1 ]; then
	echo "Usage: $0 target"
	echo "target can be: local, test, real"
	exit 1
fi

target=$1

srcPom=
srcHSite=
dstPom="pom.xml"
dstHSite="src/main/resources/hbase-site.xml"

if [ "$target" = "local" ]; then
	srcPom="pom.xml.92"
	srcHSite="src/main/resources/hbase-site.xml.local"
elif [ "$target" = "test" ]; then
	srcPom="pom.xml.92"
	srcHSite="src/main/resources/hbase-site.xml.test"
elif [ "$target" = "real" ]; then	
	srcPom="pom.xml.94"
	srcHSite="src/main/resources/hbase-site.xml.real"
else
	echo "Unknown target: $1"
	exit 1
fi

cp $srcPom $dstPom
cp $srcHSite $dstHSite
mvn clean
