# Coloring Embedder
## What is this?
This is the source code of our research paper submitted to SIGKDD 2018. In this paper, we propose a new data structure named ***Coloring Embedder***.
The Coloring Embedder is a probabilistic data structure for multi-set query. It has two key ideas, hyper mapping and coloring embedding. 
Experiments and mathematical analysis show that it has higher accuracy and faster query speed than the state-of-the-art. 
## Content
We implement the Coloring Embedder, the Multiple Bloom filter, the Coded Bloom filter and the shifting Bloom filter in C++. We use BOB hashing as the hashing functions.
We compare the Coloring Embedder with the three Bloom filter variances using both synthetic dataset (main.cpp) and real dataset (test.cpp).
## How to run it?
```makefile
mkdir build
cmake ..
make
../bin/demo
```
## Enjoy it!
