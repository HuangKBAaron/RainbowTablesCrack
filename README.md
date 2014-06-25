RainbowTablesCrack
==================

Rainbow tables generator and digest crack.



Install
-------

'''
cd ui/
'''
'''
make -f makefile_generator
'''
make -f makefile_breaker
'''


generator
---------

Usage: rbt_generate [--version] |
                    [--kl max_password_length]
                    [--min --may --num --special]
                    [--cl chain_length]
                    [--tl table_length]
                    [--t tables_number]
                    [--th threads_number]


cracker
-------

Usage: rbt_break [--version] |
                 [--rbt path/to/rainbow_table_package]
                 [--file digest_file]
                 [--th threads_number]

