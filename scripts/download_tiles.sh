#!/usr/bin/env bash

# This script downloads example HGT files
# that include territory of Poland.

set -e
cd $(dirname `readlink -f "$0"`)

URL_PREFIX="https://dds.cr.usgs.gov/srtm/version2_1/SRTM3/Eurasia"
OUT_FOLDER="../res/assets/tiles"

mkdir ${OUT_FOLDER}

mkdir zips

for lon in {14..23}
do
    for lat in {49..54}
    do
        filename=N${lat}E0${lon}.hgt
	zip_file=zips/${filename}.zip
	# hgt_out=${OUT_FOLDER}/${filename}
        wget ${URL_PREFIX}/${filename}.zip -O ${zip_file}
	echo "${filename}: ${zip_file} => ${OUT_FOLDER}/${filename}"
        unzip -d ${OUT_FOLDER} ${zip_file} ${filename}
    done
done
