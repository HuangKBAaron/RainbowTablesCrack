#ifndef RAINBOWTABLESCRACK_X_RAINBOW_CRACK_H
#define RAINBOWTABLESCRACK_X_RAINBOW_CRACK_H

#endif //RAINBOWTABLESCRACK_X_RAINBOW_CRACK_H




#define X_RAINBOW_CRACK_USAGE "Usage: xrainbow_crack -g\n"\
                              "                      -m MAX_PASSWORD_LENGTH\n"\
                              "                      -s CHARSET\n"\
                              "                      -c CHAIN_LENGTH\n"\
                              "                      -l TABLE_LENGTH\n"\
                              "                      -n TABLES_NUMBER\n"\
                              "       xrainbow_crack -f DIGEST_FILE -r PATH/TO/RAINBOW_TABLE_PACKAGE\n"\
                              "\n"\
                              "       xrainbow_crack -a PATH/TO/RAINBOW_TABLE_PACKAGE\n"\
                              "\n"\
                              "                      -t threads\n"\
                              "\n"\
                              "                      [--version]\n"

#define DEFAULT_THREADS 8

#define NUMERIC_CHARSET_CODE 'n'
#define LOWERALPHA_CHARSET_CODE 'l'
#define UPPERALPHA_CHARSET_CODE 'u'
#define SPECIAL_CHARSET_CODE 's'