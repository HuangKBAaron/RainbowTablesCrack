#!/bin/bash
# try to compare collisions with different keygen rate executions


charset=-a
keylength=5
threads=8

# prueba 1.
chain_length=500
tables_number=4

table_length=10000
while [  $table_length -le 40000 ]
do
  time ./generate -l $keylength $charset -cl $chain_length -t $tables_number -tl $table_length -ths $threads
  let table_length=$table_length+5000
done

# prueba 2.
# done
