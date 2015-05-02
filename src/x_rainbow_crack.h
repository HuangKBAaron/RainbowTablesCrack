#ifndef RAINBOWTABLESCRACK_X_RAINBOW_CRACK_H
#define RAINBOWTABLESCRACK_X_RAINBOW_CRACK_H

#endif //RAINBOWTABLESCRACK_X_RAINBOW_CRACK_H




#define X_RAINBOW_CRACK_USAGE "Usage: xrainbow_crack -g\n"\
                              "                      -M MAX_PASSWORD_LENGTH\n"\
                              "                      -m MIN_PASSWORD_LENGTH\n"\
                              "                      -s KEYSET\n"\
                              "                      -c CHAIN_LENGTH\n"\
                              "                      -l TABLE_LENGTH\n"\
                              "                      -n TABLES_NUMBER\n"\
                              "       xrainbow_crack -f DIGEST_FILE -r PATH/TO/RAINBOW_TABLE_PACKAGE\n"\
                              "\n"\
                              "                      -t threads\n"\
                              "\n"\
                              "                      [--version]\n"

#define DEFAULT_THREADS 8

#define NUMERIC_KEYSET_CODE 'n'
#define LOWERALPHA_KEYSET_CODE 'l'
#define UPPERALPHA_KEYSET_CODE 'u'
#define SPECIALCHARS_KEYSET_CODE 's'