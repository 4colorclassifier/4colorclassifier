# 4colorclassifier
## What is this?
This is the source code of our research paper submitted to SIGMOD 2018. In this paper, we propose a new data structure named ***4-color classifier***.
The 4-color classifier is a probabilistic data structure for multi-set query. Its performance is supported by the well-known **four color theorem**. 
Experiments and mathematical analysis show that it has higher accuracy and faster query speed than the state-of-the-art. 
## Content
We implement the 4-color classifier, the Multiple Bloom filter, the Coded Bloom filter and the shifting Bloom filter in C++. We use BOB hashing as the hashing functions.
We compare the 4-color classifier with the three Bloom filter variances using both synthetic dataset (main.cpp) and real dataset (test.cpp).
## How to run it?
```makefile
mkdir build
cmake ..
make
../bin/demo
```
## Enjoy it!
