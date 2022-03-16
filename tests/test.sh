#! /bin/bash
# This script is intended to be run from the root dir.

printf "\033[38;2;255;150;150m==============================\nTesting Ansi/Ascii Gradient...\n==============================\n\n\033[0m"
./img_to_txt tests/gradient-test.bmp
./img_to_txt -qa tests/gradient-test.bmp

# NOTE: Options must come before the image path to be applied.

printf "\033[38;2;255;150;150m========================\nTesting image formats...\n========================\n\n\033[0m"
./img_to_txt -s tests/test_img/*

# NOTE: -s is only being applied to last image.

printf "\033[38;2;255;150;150m======================\nTesting image sizes...\n======================\n\n\033[0m"
./img_to_txt -sw 30 -h 10 tests/gradient-test.bmp
./img_to_txt -swh 10 tests/gradient-test.bmp
