#!/bin/bash

install_pbrt(){
    git clone https://github.com/mmp/pbrt-v2.git
    cd pbrt-v2/src
    make
}
download_database(){
    file=$1/"mat_list.txt"
    while IFS= read -r line
    do
        wget $brdfs_url$line
    done <"$file"
}

path=$(pwd)

echo "Starting MView setup"

echo "Search for dependences"

# pbrt verification
if [ -e $path/pbrt-v2 ]
then
    echo "Pbrt version 2 already installed!"
else
    echo "Installing pbrt..."
    install_pbrt
    echo "Pbrt was successful installed!"
    cd $path
fi

# merl database verification
if [ -e $path/Materials ] && [ ! -z $(ls Materials) ]
then
    echo "Merl database exists!"
else
    echo 
    brdfs_url="http://people.csail.mit.edu/wojciech/BRDFDatabase/brdfs/"
    if [ ! -e  $path/Materials ]; then    
        mkdir Materials
    fi    
    cd Materials
    echo "Downloading files..."
    download_database $path
    echo "Database was downloaded with success!"
    cd $path
fi    

# creating necessary directories
if [ ! -e $path/New ]
then
    mkdir New
fi

# build all the stuff
echo "Generating some libraries"
gcc -fPIC -o libbin.o -c BRDFWritebin.c
gcc -shared -o libbin.so libbin.o

echo "Setup finished!"
