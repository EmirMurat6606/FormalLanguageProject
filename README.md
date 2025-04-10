# Formal Language Project — CFG & PDA Tools

This project was created as part of the course **Machines and Computability** (*Machines en Berekenbaarheid*) during the **first semester of the second year** of the **Computer Science** bachelor's program at **University of Antwerp** in the academic year **2024–2025**.

It contains tools and implementations to work with **Context-Free Grammars (CFGs)** and **Pushdown Automata (PDAs)**.

---

## 📂 Project Structure

The project includes the following C++ source files:

- `main.cpp` — Entry point
- `CFG.cpp`, `CFG.h` — CFG parsing, LL(1) table construction, acceptance, and manipulation
- `PDA.cpp`, `PDA.h` — PDA to CFG conversion
- `Logger.cpp`, `Logger.h` — Output formatting (tables, JSON)

---

## 📥 Input Files

All JSON-formatted input files used to test and run algorithms are located in the `InputFiles` directory in the root of the project.

---

## 🧠 Implemented Algorithms

### 🧾 1. LL(1) Table Construction and Parsing
- First and Follow set computation
- LL(1) parse table generation
- String parsing using LL(1) algorithm

### 🧮 2. CYK Parsing Algorithm
- Support for parsing using the CYK (Cocke–Younger–Kasami) algorithm
- Input via CNF (Chomsky Normal Form) compatible JSON

### 🔁 3. PDA to CFG Conversion
- Generation of equivalent context-free grammar from a pushdown automaton
- Utilizes transition expansion and state tracking

### 📜 4. CFG Handling
- Add and manage production rules
- Manage terminals, non-terminals, and start symbols
- Validate whether a symbol is terminal or not

### 📊 5. Logging and Output Formatting
- Print parse and acceptance tables
- JSON output generation for parse tables
- Utility functions for string and table formatting

---

## 🚀 Setup and Usage

1. **Clone the repository:**

```bash
git clone https://github.com/EmirMurat6606/FormalLanguageProject.git
cd FormalLanguageProject
```
- make sure to adjust `main.cpp` if you want to load a specific JSON input file.
---------------------------

## 📘 Notes
The project uses standard C++17 and has no external dependencies.

All CFG and PDA input files are provided in JSON format for clarity and easy parsing.

This project was developed with educational purposes in mind and may be extended further for research or teaching.