#!/bin/bash

if [ ! ${JAVA_HOME} ]; 
  then echo "Environment variable 'JAVA_HOME' must present"; exit 0;
fi

XML=true

MODE="reload"

FILE=""

while getopts xprt:m:l:h OPTION

do
  case ${OPTION} in
    x)
      XML=true;;
    p)
      XML=false;;
    r)
      MODE="reload";;
    m)
      MODE="merge"
      FILE=${OPTARG};;
    l)
      MODE="load"
      FILE=${OPTARG};;
    t)
      MODE="test"
      FILE=${OPTARG};;
    h)
      echo "JadeConfigurator [-x | -p] [-r | -m file | -l file]";
      echo "  -x   Reloading xml configurations";
      echo "  -p   Reloading old properties configurations";
      echo "  -r   Reloading";
      echo "  -m   Merge file to configuration";
      echo "  -l   Loading file as configration";
      exit 0;;
    \?)
      echo "JadeConfigurator [-x | -p] [-r | -m file | -l file]";
      exit 0;;
  esac
done

if (${XML}) then

  ${JAVA_HOME}/bin/java -cp xiaonei-commons-jade-impl-xceadapter.jar:* com.xiaonei.commons.jade.impl.xceadapter.configurator.RemoteXmlDocXceConfigurator ${MODE} ${FILE}

else


  ${JAVA_HOME}/bin/java -cp xiaonei-commons-jade-impl-xceadapter.jar:* com.xiaonei.commons.jade.impl.xceadapter.configurator.RemoteXceConfigurator ${MODE} ${FILE}

fi



