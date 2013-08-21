# !/bin/bash
# author: yuwei.mu

ROOT_DIR="xoa2-server"
TGZ_DIR="$ROOT_DIR/bin"
TGZ_FILE="xoa2-server-1.0.tar.gz"

usage() {
  echo "Usage: sh $0 [$ROOT_DIR-VERSION.tar.gz]"
}

cd ../..

if [ $1 ]; then
  TGZ_FILE=$1
fi

DST="$TGZ_DIR/$TGZ_FILE"
echo $DST

echo "Output file is : $(tput setaf 1)$TGZ_FILE$(tput sgr0)"

if [ -e $DST ]
then
  echo "$(tput setaf 1)file $TGZ_FILE already exists!$(tput sgr0)"
  usage
  exit 1
fi

LIST_FILE=./$TGZ_DIR/.excluded_list
find $ROOT_DIR -name \* | sed -n '/\/\.[a-z]\+/p' > $LIST_FILE
echo $DST >> $LIST_FILE
echo "$(tput setaf 3)Packaging ...$(tput sgr0)"
cat $LIST_FILE

tar -zcvf $DST -X $LIST_FILE $ROOT_DIR

echo "Package OK : $(tput setaf 3)$TGZ_FILE$(tput sgr0)"
