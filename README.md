#X Rainbow Crack

Rainbow tables generator and sha1 digests crack.

## Main features
* Analyze passwords contained in Rainbow Tables.
* Generate parameterized Rainbow Tables.
* Use generated Rainbow Tables to crack sha1 digests.

## More features
* Generate random passwords or masked passwords.
* Multi-threading. 
* Generate masked passwords.
* Tested on MacOSX Yosemite and Ubuntu 14.04.

## Prerrequisites

* GCC compiler and make.  
```
sudo apt-get install gcc make
```

* Openssl.  
```
sudo apt-get install libssl-dev
```

## Installation

* Change to root directory.  

* Compile binary.  
```
make
```
* Install.  
```
sudo make install clean
```


## Usage
```
Usage: xrainbow_crack -g
                      -M MAX_PASSWORD_LENGTH
                      -s u|l|n|s [ -m MASK ]
                      -c CHAIN_LENGTH
                      -l TABLE_LENGTH
                      -n TABLES_NUMBER

       xrainbow_crack [ -f DIGEST_FILE | -b DIGEST ]
                      -r PATH/TO/RAINBOW_TABLE_PACKAGE

       xrainbow_crack -a PATH/TO/RAINBOW_TABLE_PACKAGE

                      [ -t threads ]

                      [--version]
```

