## Execution

First of all, we will have to make several modifications to redirect the output file paths. to do this, we will have to enter to the following program and change the path of the output file:

* main_funcional.c


![](images_readme/paths.png)

In case you want to change the number of iterations or the population size, just change the following parameters: 

![](images_readme/popsize.png)

The programme runs in the following way: 

```{c}
gcc main_funcional.c -lm -o export.exe
```