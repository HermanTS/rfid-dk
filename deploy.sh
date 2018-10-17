#!/bin/bash

#set build location

mkdir ../rfid-dk-bld-dbg/
cd ../rfid-dk-bld-dbg/
cmake -DCMAKE_BUILD_TYPE=DEBUG -G "Eclipse CDT4 - Unix Makefiles" ../rfid-dk-src

