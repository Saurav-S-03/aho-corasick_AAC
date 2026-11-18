# Instructions

Input file used rn `/Datasets/Testing/full-domains-input.txt`

---

### Compile
`g++ domain_names_test.cpp ../../src/AC_automaton.cpp ../../src/charInputFormatter.cpp -I ../../include -o domains_test`  
(C++17 used in my system) [-std=c++17 optional]  

`g++ Datasets/Testing/domain_names_test.cpp src/AC_automaton.cpp src/charInputFormatter.cpp -I include -o domains_test`  
[To run cmd at Home Directory]

---

### Run
`./<object_name>` {For manual input}  
`./<object_name> < ../Testcases/Inputs/<file_name>.txt` {Direct read from file}

### cmd
`g++ Sample_Tests/TestCentre/test1.cpp src/AC_automaton.cpp src/format_inputs.cpp -Iinclude -o test`
