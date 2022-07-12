#!/bin/bash

echo ""
echo "使用方法："
echo "./run-git.sh web 或者 doc 或者 contract"
echo "按回车键执行，按其他键退出。"
echo ""

if [ $# -ne 1  ];then
    exit 1
fi

###################################################################################################

read -p "$(pwd)# pwd        " flag

if [ "$flag" != ""  ];then
    exit 0
fi

pwd

echo ""

###################################################################################################

read -p "$(pwd)# lh         " flag

if [ "$flag" != ""  ];then
    exit 0
fi

ls -lh

echo ""

###################################################################################################

read -p "$(pwd)# git status        " flag

if [ "$flag" != ""  ];then
    exit 0
fi

git status

echo ""

###################################################################################################

read -p "$(pwd)# git add -A .        " flag

if [ "$flag" != ""  ];then
    exit 0
fi

git add -A .

echo ""

###################################################################################################

read -p "$(pwd)# git commit -am \"edit $1\"        " flag

if [ "$flag" != ""  ];then
    exit 0
fi

git commit -am "edit $1"

echo ""

###################################################################################################

read -p "$(pwd)# git pull origin main        " flag

if [ "$flag" != ""  ];then
    exit 0
fi

git pull origin main

echo ""

###################################################################################################

read -p "$(pwd)# git push origin main        " flag

if [ "$flag" != ""  ];then
    exit 0
fi

git push origin main

echo ""

###################################################################################################

exit 0
