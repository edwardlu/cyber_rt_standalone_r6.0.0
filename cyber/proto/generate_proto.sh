#!/bin/bash
echo "##### Generate protobuf files Start #####"

path=$1
files=$(ls $path)

cd $1;

for filename in $files
do
    if [ "${filename##*.}"x = "proto"x ];then
        echo "Generate : "; 
	echo $filename
        protoc -I=./ $filename --cpp_out=./
    fi
done
echo "##### Generate protobuf files End #####"
