#!/bin/bash
# try to compare different executions with different values for parameters (tables_number, chain_lenghth, table_length)


charset=-a
keylength=6
threads=8

# prueba 1. tables number
table_length=50000
chain_length=1000

tables_number=1
while [  $tables_number -le 8 ]
do
  time ./generate -l $keylength $charset -cl $chain_length -t $tables_number -tl $table_length -ths $threads
  let tables_number=$tables_number+1
done

# prueba 2. chain length
table_length=50000
tables_number=5

chain_length=200
while [  $chain_length -le 1600 ]
do
  time ./generate -l $keylength $charset -cl $chain_length -t $tables_number -tl $table_length -ths $threads
  let chain_length=$chain_length+200 
done

# prueba 3. table length
chain_length=1000
tables_number=5

table_length=10000
while [  $table_length -le 80000 ]
do
  time ./generate -l $keylength $charset -cl $chain_length -t $tables_number -tl $table_length -ths $threads
  let table_length=$table_length+10000;
done
