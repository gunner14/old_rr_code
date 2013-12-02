#!/bin/bash
#
# Author: Qu Zhi
# Date: 2012.6.28 18:25 
# This script is used to compile self-defined SqoopRecord class
# Add --jar-file {jarfilename} --class-name {classname} to use it.

follow_one() {
  # Resolve symlinks and relative path components along a path.  This requires
  # its argument to be an absolute path.  This does not recursively re-resolve
  # symlinks; if that is required, use the 'follow' method.

  target=$1
  OIFS=$IFS
  IFS='/'

  # Taking each dir component along the way, build up a new target directory,
  # resolving '.', '..', and symlinks.
  newtarget=''
  for part in ${target}; do
    if [ -z "${part}" ]; then
      continue # Empty dir part. 'foo//bar'
    elif [ "." == "${part}" ]; then
      continue # Nothing special to do for '.'
    elif  [ ".." == "${part}" ]; then
      IFS=$OIFS
      newtarget=`dirname ${newtarget}` # pop a component.
    elif [ -h "${newtarget}/${part}" ]; then
      IFS=$OIFS
      link=`readlink ${newtarget}/${part}`
      # links can be relative or absolute. Relative ones get appended to
      # newtarget; absolute ones replace it.
      if [ "${link:0:1}" != "/"  ]; then
        newtarget="${newtarget}/${link}" # relative
      else
        newtarget="${link}" # absolute
      fi
    else # Regular file component.
      newtarget="${newtarget}/${part}"
    fi
    IFS='/'
  done

  IFS=$OIFS
  echo $newtarget
}

follow() {
  # Portable 'readlink -f' function to follow a file's links to the final
  # target.  Calls follow_one recursively til we're finished tracing symlinks.

  target=$1
  depth=$2

  if [ -z "$depth" ]; then
    depth=0
  elif [ "$depth" == "1000" ]; then
    # Don't recurse indefinitely; we've probably hit a symlink cycle.
    # Just bail out here.
    echo $target
    return 1
  fi

  # Canonicalize the target to be an absolute path.
  targetdir=`dirname ${target}`
  targetdir=`cd ${targetdir} && pwd`
  target=${targetdir}/`basename ${target}`

  # Use follow_one to resolve links. Test that we get the same result twice,
  # to terminate iteration.
  first=`follow_one ${target}`
  second=`follow_one ${first}`
  if [ "${first}" == "${second}" ]; then
    # We're done.
    echo "${second}"
  else
    # Need to continue resolving links.
    echo `follow ${second} $(( $depth + 1 ))`
  fi
}

prgm=`follow $0`
bin=`dirname ${prgm}`
bin=`cd ${bin} && pwd`

source ${bin}/configure-sqoop "${bin}"
javac -classpath $HADOOP_CLASSPATH $1
classname=${1/".java"/".class"}
jarname=${1/".java"/".jar"}
jar cvf $jarname $classname
rm $classname 
