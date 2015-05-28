#Rainbow Tables Crack (TFM version)

Rainbow tables generator and sha1 crack.

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
                      -m MAX_PASSWORD_LENGTH
                      -s [ u | l | n | s ]
                      -c CHAIN_LENGTH
                      -l TABLE_LENGTH
                      -n TABLES_NUMBER

       xrainbow_crack [ -f DIGEST_FILE | -b DIGEST ]
                      -r PATH/TO/RAINBOW_TABLE_PACKAGE

       xrainbow_crack -a PATH/TO/RAINBOW_TABLE_PACKAGE

                      [ -t threads ]

                      [--version]
```

