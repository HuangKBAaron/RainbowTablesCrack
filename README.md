RainbowTablesCrack
==================

Rainbow tables generator and digest crack.



Install
-------

Situarse en el directorio ui/
```
cd ui/
```
Compilar el generador de Rainbow Tables
```
make -f makefile_generator
```
Compilar el cracker de contraseñas
```
make -f makefile_breaker
```


Generator
---------
```
Usage: rbt_generate [--version] |
                    [--kl max_password_length]
                    [--min --may --num --special]
                    [--cl chain_length]
                    [--tl table_length]
                    [--t tables_number]
                    [--th threads_number]
```

Cracker
-------
```
Usage: rbt_break [--version] |
                 [--rbt path/to/rainbow_table_package]
                 [--file digest_file]
                 [--th threads_number]
```
