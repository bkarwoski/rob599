#!/usr/bin/python3
import sys
import string
counts = {}
countsSize = sys.getsizeof(counts)
print(f'Hashtable started with {countsSize} bytes at length 0')
word = ""
nextWord = ""
hasWord = False
with open("book.txt", "r") as f:
    for line in f:
        for nextWord in line.split():
            bigram = word + " " + nextWord
            #print(f'word is {word}')
            #print(f'nextWord is {nextWord}')
            bigram = bigram.lower()
            for x in bigram:
                if x != " " and not x.isalpha():
                    bigram = bigram.replace(x, "")
            if hasWord:
                #print(bigram)
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
#print(len(counts))
#print(sys.getsizeof(counts))
for x in counts:
    if counts[x] > 200:
        print(f'Bigram {x} has count of {counts[x]}')
print(f'Total of {len(counts.keys())} different bigrams recorded')