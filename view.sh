#!/usr/bin/gnuplot -persist
plot "log/linear" index 0, "log/linear" index 1
set term x11 1
plot "log/rbf" index 0, "log/rbf" index 1
