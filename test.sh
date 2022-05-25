#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./simple-cc "$input" | ./simulator/simulator
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => expected $expected, but got $actual"
    exit 1
  fi
}

assert 3 "3;"
assert 2 "return 2;"
assert 21 "a=21; return a;"
assert 19 "a=3; b=4; if (a == b) return 4; else return 19;"
echo OK
