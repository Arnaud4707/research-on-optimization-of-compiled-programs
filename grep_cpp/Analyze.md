# Analyse of data

In this version I edit the code for to have only one open function. With that base we will check performance.

```**perf record -g -o perf_custom_grep_1open.data -- custom_grep -r "llvm" .**```
This command record command's preformance given to perf.

An using ```**perf report -i perf_custom.data**``` we could check what function cost more of time in CPU:
![Return's image of perf](./images/perf_cgrep_1open.png "Perf custom grep wiht only one open")

And also compare with previous custom_grep:
![Return's image of perf](./images/perf_cgrep.png "Perf old custom grep")

Now we can see more memory is used with the second custom_grep although rdir take less time of CPU. There are a lot of call to read, if we remove ifstream (open) and read, we can gain somme perf.
