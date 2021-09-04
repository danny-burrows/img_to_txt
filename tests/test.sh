#! /bin/bash
# This script is intended to be run from the root dir.

echo "Testing Ansi/Ascii Gradient..."
./img_to_txt tests/gradient-test.bmp
./img_to_txt tests/gradient-test.bmp -qa

echo "Testing other formats..."
./img_to_txt tests/test_img/* -s