# Analyse of data

An using **hyperfine** we will have this informations:
![Return's image of hyperfine](./images/benchmacks.png "Initial Benchmarks")

We can see rg is 400% more faster than grep -rn and custom_grep -r.
we can also see than rg have sinificatively take less of time in User's code than in System.
Use of Memory RAM is also different in function of command, I don't know for now at what it do. 

First of all I will begin by optimize basics things like:
 - limited calls to system 
 - reduce time in function
 - cleaning code

The command **strace** is good for detect syscall

The command **perf** is good for analyse performance
