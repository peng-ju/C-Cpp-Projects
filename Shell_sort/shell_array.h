#ifndef __SHELL_ARRAY_H__
#define __SHELL_ARRAY_H__

long *Array_Load_From_File(char *filename, int *size);

int Array_Save_To_File(char *filename, long *array, int size);

void Array_Shellsort(long *array, int size, double *n_comp);

#endif

