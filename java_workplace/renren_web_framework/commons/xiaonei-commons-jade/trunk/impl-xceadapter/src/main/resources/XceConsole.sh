#!/bin/bash

if [ ! ${JAVA_HOME} ];
  then echo "Environment variable 'JAVA_HOME' must present"; exit 0;
fi

stty erase ^h

${JAVA_HOME}/bin/java -cp xiaonei-commons-jade-impl-xceadapter.jar:* com.xiaonei.commons.jade.impl.xceadapter.datasource.XceConsole $1 $2 $3 $4

stty erase ^?