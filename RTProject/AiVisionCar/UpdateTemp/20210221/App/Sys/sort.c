/*
 * sort.c
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */
#include "sort.h"
#include "math.h"

static void sort_swap(float *a,float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

void Bubble_Sort(float *data,int len)
{
    float temp = 0.0;

    for(int i = 0 ; i < len - 1; i++)
    {
        for(int j = 0 ; j < len - 1 ; j++)
        {
             if(data[j] > data[j-1])
             {
                 temp = data[j+1];
                 data[j+1] = data[j];
                 data[j] = temp;
             }
        }
    }
}

void Quick_Sort(float arr[], int start, int end)
{
    if (start >= end)
        return;
    float mid = arr[end];
    int left = start, right = end - 1;
    while (left < right) {
        while (arr[left] < mid && left < right)
            left++;
        while (arr[right] >= mid && left < right)
            right--;
        sort_swap(&arr[left], &arr[right]);
    }
    if (arr[left] >= arr[end])
        sort_swap(&arr[left], &arr[end]);
    else
        left++;

    if (left)
        Quick_Sort(arr, start, left - 1);
    Quick_Sort(arr, left + 1, end);
}

