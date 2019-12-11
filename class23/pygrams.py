#!/usr/bin/python3
import sys
import string
#import numpy as np
counts = {}
countsSize = sys.getsizeof(counts)
print(f'Hashtable started with {countsSize} bytes at length 0')
word = ""
nextWord = ""
hasWord = False
with open("test_book.txt", "r") as f:
    for line in f:
        for nextWord in line.split():
            for x in nextWord:
                if x != " " and not x.isalpha():
                    nextWord = nextWord.replace(x, " ")
            bigram = word + " " + nextWord
            bigram = bigram.lower()
            if hasWord:
                if bigram in counts.keys():
                    counts[bigram] = counts[bigram] + 1
                else:
                    counts[bigram] = 1
            else:
                hasWord = True
            if sys.getsizeof(counts) != countsSize:
                countsSize = sys.getsizeof(counts)
                print(f'Hashtable grew to {countsSize} bytes at length {len(counts)}')
            word = nextWord
    exc200 = False
    if max(counts.values()) > 200:
        exc200 = True
for x in counts:
    if not exc200 or counts[x] > 200:
        print(f"Bigram '{x}' has count of {counts[x]}")
print(f'Total of {len(counts.keys())} different bigrams recorded')