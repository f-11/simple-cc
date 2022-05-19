#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./simple-cc "$input" > tmp.s
}
