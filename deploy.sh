#!/bin/bash

#set build location

mkdir ../rfid-dk-bld/
cd ../rfid-dk-bld/
cmake  -G "Eclipse CDT4 - Unix Makefiles" ../rfid-dk-src

