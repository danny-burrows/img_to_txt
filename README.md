<div align="center">
    <img src="img_to_txt.svg" style="width: 80%;" />
</div>

<strong>img_to_txt</strong> is still in early development but feel free to try it out! See the Build and Usage sections for more details on getting started. If you'd like to get involved please see the Contributing section. Also please submit an issue if you have any issues using or building.

### Platforms
Currently just Linux but Windows coming soon!

### Supported Images
Image Type      | Supported
------------- | -------------
jpg           | Full 
png           | Full 
bmp           | Full
tga           | Most Subsets
hdr           | RGBE Format
gif           | Static
pic           | Softimage
pnm/pgm/ppm   | Binary only

## Build
```bash
$ git clone https://github.com/danny-burrows/img_to_txt.git
$ make
```

## Usage (Proper docs coming soon)
```
Usage: img_to_txt [OPTION]... [FILE]...                       
Convert images to text. Either Ascii, Ansi (true colour),                   
or solid background (Ansi true color).                                      
                                                                            
Available input image types:                                                
    - jpg/jpeg                                                              
    - png                                                                   
    - bmp                                                                   
    - tga                                                                   
    - hdr                                                                   
    - gif (static)                                                          
    - pic (not tested yet)                                                  
    - ppm/pnm/pgm                                                           
                                                                            
Options:                                                                    
    -w --width          Set width and height (rows and cols) of output.     
    -h --height                                                             
                                                                            
    -o --original-size  Output original size. (Size of image in text)       
                                                                            
    -a --ascii          Ascii mode. (no ansi colors)                        
    -s --solid          Solid mode. (Background colors with no ascii chars) 
                                                                            
    -t --true-color     Use true color for ANSI output.                     
                        (Need true-color supported terminal)                
                                                                            
    -n --no-squashing   Disable standard image squashing that tries to make 
                        image look less elongated by chars being taller than
                        they are wide.                                      
                                                                            
    -q --quiet          Quiet mode (suppress header)                        
                                                                            
    -? --help           Print help message and exit.                        
                                                                            
    -v --version        Print version and exit.                             
                                                                            
Examples:                                                                   
                                                                            
    img_to_txt -h 100 -w 200 -s some_img.png                                
                                                                            
    img_to_txt -whqt 30 some_img.png                                        
                                                                            
    img_to_txt -a some_img.jpg                                              
                                                                            
    img_to_txt -q -t -o some_img.bmp                                                                     
```

## Contributing
Please, if you come accross any bugs or issues feel free to submit them on the issues page of the github. Pull requests are accepted too. :-)

If you'd like to get involved with the development download the source and open with vscode. A development environment should be setup for you. You'll need gcc, gdb and make.

### If you want a debug build...
```bash
$ make config=debug
```
