#!/usr/bin/perl

while(<>) {
  print "$1\n" if (/\(LEX \(([^ ]+) /);
}
