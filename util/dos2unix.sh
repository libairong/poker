#!/bin/bash

# 将文件的换行从 \r\n 统一替换成 \n
sed -i 's/\r$//' $1

echo "Successfully converted " + $1
