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
assert 2 "a=2;"
#assert 0 "return a;"
assert 8 "foo=5; bar=8; return bar;"
assert 21 "a=21; return a;"
assert 19 "a=3; b=4; if (a == b) return 4; else return 19;"
assert 21 "hoge=28; when(hoge > 22) hoge = hoge-1; return hoge;"
assert 15 "for (i=1; i<5; i=i+1) res = res + i; return res;"
echo OK
