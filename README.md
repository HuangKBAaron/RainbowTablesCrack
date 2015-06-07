#X Rainbow Crack

Rainbow tables generator and sha1 digests crack.

## Main features
* Analyze passwords contained in Rainbow Tables.
* Generate parameterized Rainbow Tables.
* Use generated Rainbow Tables to crack sha1 digests.

## More features
* Generate random passwords or masked passwords.
* Multi-threading. 
* Tested on MacOSX Yosemite and Ubuntu 14.04.

## Install X Rainbow Crack
### Prerrequisites

* GCC compiler and make.  
```
sudo apt-get install gcc make
```

* Openssl.  
```
sudo apt-get install libssl-dev
```

### Installation

* Download .zip package and unzip it.

* Change to root directory.  

* Compile binary.  
```
make
```
* Install.  
```
sudo make install clean
```


## How to use X Rainbow Tables
```
Usage: xrainbow_crack -g
                      --maxlen MAX_PASSWORD_LENGTH
                      --charset u|l|n|s [ --mask MASK_VALUE ]
                      --chainlen CHAIN_LENGTH
                      --tablelen TABLE_LENGTH
                      --tables TABLES_NUMBER

       xrainbow_crack --file DIGEST_FILE
                      --rainbowtable PATH/TO/RAINBOW_TABLE_PACKAGE

       xrainbow_crack --analyze PATH/TO/RAINBOW_TABLE_PACKAGE

                      [ --threads THREADS ]

                      [--version]
```
### Generate Rainbow Tables
```
xrainbow_crack -g -M MAX_PASSWORD_LENGTH -s u|l|n|s -m MASK_VALUE -c CHAIN_LENGTH -l TABLE_LENGTH -n TABLES_NUMBER
```
Where:
- MAX_PASSWORD_LENGTH: maximum length of generated passwords.
- u: uppercase.
- l: lowercase.
- n: digits.
- s: special characters.
- MASK_VALUE: 1 (bruteforce) | 2 (end two digits) | 4 (end four digits) | 5 (begin one uppercase) | 8 (begin one uppercase) | 9 (combo begin one uppercase and end two digits)
- CHAIN_LENGTH: length of chain.
- TABLE_LENGTH: length of each Rainbow Table.
- TABLES_NUMBER: number of Rainbow Tables.

### Analyze Rainbow Tables
```
xrainbow_crack -a PATH/TO/RAINBOW_TABLE_PACKAGE
```
Where:
- PATH/TO/RAINBOW_TABLE_PACKAGE: path to Rainbow Tables Package to analyze.

### Crack sha1 digests
```
xrainbow_crack -f DIGEST_FILE -r PATH/TO/RAINBOW_TABLE_PACKAGE
```
Where:
- PATH/TO/RAINBOW_TABLE_PACKAGE: path to Rainbow Tables Package to use.
- DIGEST_FILE: file with all sha1 digests that we want to crack (one per line).
