#!/usr/bin/env python3

# This script downloads HGT from https://dds.cr.usgs.gov
# Read usage and licensing information of this data before using it.

# Example: To download area representing Poland:
# download_tiles.py 49 14 54 23 <out_folder>

import os, sys

URL_PREFIX="https://dds.cr.usgs.gov/srtm/version2_1/SRTM3/Eurasia"

def main():
    os.system('cd $(dirname `readlink -f "$0"`)')

    if len(sys.argv) == 1:
        print("usage: download_tiles.py <lat_min> <lon_min> <lat_max> <lon_max> <out_folder>")
        print("")
        print("Example: To download area representing Poland:")
        print("download_tiles.py 49 14 54 23 <out_folder>")
        return

    if len(sys.argv) != 6:
        print("Illegal number of parameters")
        print("Call download_tiles.py for usage instructions")
        return

    lat_range = range(int(sys.argv[1]), int(sys.argv[3])+1)
    lon_range = range(int(sys.argv[2]), int(sys.argv[4])+1)
    out_folder = sys.argv[5]

    os.system("mkdir -p %s" % out_folder)
    os.system("mkdir -p zips")

    for lat in lat_range:
        for lon in lon_range:
            filename = "%c%02d%c%03d.hgt" % ('N' if lat >= 0 else 'S', lat if lat >= 0 else -lat, 'E' if lon >= 0 else 'W', lon if lon >= 0 else -lon)
            zip_file = "zips/%s.zip" % filename
            os.system("wget --no-verbose %s/%s.zip -O %s" % (URL_PREFIX, filename, zip_file))
            os.system("unzip -d %s %s %s" % (out_folder, zip_file, filename))
            os.system("rm %s" % zip_file)


if __name__ == '__main__':
    main()
