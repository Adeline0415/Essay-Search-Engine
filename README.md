# Essay Search Engine 

This project implements a simple **essay search engine** in C++ using the **Trie (Prefix Tree)** data structure.  
It supports **prefix**, **suffix**, and **exact** word search, as well as logical operations **AND (+)** and **OR (/)** between queries.

---

## Project Overview

Modern search engines like Google and Yahoo rely on efficient string matching algorithms.  
In this project, we simulate a simplified essay search engine capable of handling thousands of essays efficiently.

Given:
- A directory containing essay files (`0.txt`, `1.txt`, `2.txt`, …)
- A query file with search expressions
- An output file path

The program outputs the titles of essays that match each query.

---

## Features

- **Case-insensitive search**  
  Uppercase and lowercase letters are treated as equivalent.

- **Trie-based structure**  
  Each essay has its own **prefix Trie** and **suffix Trie** for efficient lookup.

- **Search Types**
  - **Exact search**: `"word"`
  - **Prefix search**: `word`
  - **Suffix search**: `*word*`

- **Logical Operators**
  - **AND (`+`)** → Intersection of results  
  - **OR (`/`)** → Union of results  
  (Operators are **left-associative**)

- **Ignore special characters and digits**  
  Only alphabetic words are considered (e.g., `"AB?AB"` → `"ABAB"`).

---

## Implementation Details

### Core Data Structure: `Trie`
- Each essay is represented by two Tries:
  - `pos` → prefix Trie
  - `neg` → suffix Trie  
- All essays’ Tries are stored in a `vector<Root*> root`.

### Query Processing
- Queries are read line by line from the query file.
- Each line is tokenized using a provided `split` utility.
- The program iterates through each essay’s Trie to check if the query matches.
- Operators `+` and `/` are handled via a `preOP` flag to maintain left associativity.

### Code Structure
- **`node` struct**: represents Trie node.
- **`Root` class**: contains prefix and suffix Trie roots, and insert/search methods.
- **Utility functions**:  
  - `split()` → split string by spaces.  
  - `word_parse()` → remove non-alphabetic characters.  
  - `alpha()` → extract alphabetic characters only.

---

## File Structure

```

project/
├── data/
│   ├── 0.txt
│   ├── 1.txt
│   └── ...
├── query.txt
├── 110000229_project.cpp
├── 110000229_Report.pdf
├── Final_Project_formal.pptx
└── README.md

````

---

## xample Usage

### **Compile**
```bash
g++ -std=c++17 -O2 -o essay-search.exe 110000229_project.cpp
````

### **Execute**

```bash
./essay-search.exe data query.txt output.txt
```

### **Input Example**

#### data/0.txt

```
Graph Neural Networks
Graph neural networks are a class of machine learning models...
```

#### query.txt

```
"graph"
graph + neural
"graph" / *network*
```

### **Output Example**

#### output.txt

```
Graph Neural Networks
Graph Neural Networks
Graph Neural Networks
```

If no essay matches the query:

```
Not Found!
```

---

## Testing Environment

| Specification | Detail                                  |
| ------------- | --------------------------------------- |
| CPU           | AMD Ryzen Threadripper 2990WX (32-Core) |
| RAM           | 32 GB DDR4                              |
| OS            | Linux / Windows                         |
| Compiler      | GCC 9.4.0                               |
| Time Limit    | 4 seconds                               |

---

## Performance

* Supports **scalable datasets** (tested up to 8000+ essays).
* Efficient Trie-based lookups avoid brute-force string matching.
* Handles multiple search types and operators within 4 seconds on large datasets.

---

## Challenges & Debugging

One major challenge was debugging pointer-related issues when implementing Trie with `struct`.
Initially, all searches returned `"Not Found!"`. After investigation, it was found that pointer references in the struct were not correctly maintained.

**Solution:**
Reimplemented `Root` as a **class**, embedding the insert and search functions inside it.
This encapsulation solved the null pointer issue and improved code stability.

Another issue was forgetting to insert essay titles into the Trie.
After inserting titles alongside content, all queries produced correct results.

---

## References

* [GeeksforGeeks – Trie Insert and Search](https://www.geeksforgeeks.org/trie-insert-and-search/)
* [DigitalOcean – Trie Data Structure in C++](https://www.digitalocean.com/community/tutorials/trie-data-structure-in-c-plus-plus)


## Summary

This project demonstrates how an efficient **Trie-based search engine** can be built from scratch in C++.
It supports flexible query syntax, handles thousands of documents, and operates efficiently without external libraries — reflecting the power of algorithmic data structure design.
