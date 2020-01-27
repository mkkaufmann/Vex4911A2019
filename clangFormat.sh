#!/bin/bash
#@echo off
for i in $(find ./src/ -iname "*.cpp"); do clang-format-9 -i -style=file $i || true
done

for i in $(find ./include/ -iname "*.hpp"); do clang-format-9 -i -style=file $i || true
done
