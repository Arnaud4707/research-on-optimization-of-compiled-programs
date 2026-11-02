# Research on optimization of compiled programs

This repository has for goal to optimize more of possible a compiled program ex: c/c++/rust/julia. To begin I will on c++ language with a custom_grep (with -r option) binary, the goal will be get to closer rg tool.

## commands tested

I choise a big ripository for target, "llvm-project".

```
rg --no-ignore --hidden "llvm" .
grep -rn "llvm" .
custom_grep -r "llvm" .
```

## benchmarks

The tool hyperfine :
```
hyperfine --metrics=all --warmup 3 -i COMMANDS
```

## Tools of analyse

For profiling command
```
sudo perf record -g -o perf_rg.data COMMAND
```

For analyse performance
```
perf report -i perf_custom.data
```

For view syscall 
```
strace
```