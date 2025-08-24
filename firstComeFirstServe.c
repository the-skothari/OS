#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct
{
  int pid, at, bt, st, ct, tat, wt, rt;
  bool iscompleted;
} process;

int comp1(const void *a, const void *b)
{
  process *p1 = (process *)a;
  process *p2 = (process *)b;
  return p1->at - p2->at;
}

int comp2(const void *a, const void *b)
{
  process *p1 = (process *)a;
  process *p2 = (process *)b;
  return p1->pid - p2->pid;
}

void solve()
{
  int n;
  printf("Enter number of processes: ");
  scanf("%d", &n);

  process v[n];
  for (int i = 0; i < n; i++)
  {
    v[i].pid = i + 1;
    printf("Enter arrival time and burst time for process %d:  ", v[i].pid);
    scanf("%d%d", &v[i].at, &v[i].bt);
    v[i].iscompleted = false;
  }

  qsort(v, n, sizeof(process), comp1);

  int completed = 0, time = 0, useful = 0;

  while (completed != n)
  {
    int mini = INT_MAX, idx = -1;
    for (int i = 0; i < n; i++)
    {
      if (v[i].at <= time && !v[i].iscompleted)
      {
        if (v[i].at < mini)
        {
          mini = v[i].at;
          idx = i;
        }
        else if (v[i].at == mini && v[i].pid < v[idx].pid)
        {
          idx = i;
        }
      }
    }

    if (idx != -1)
    {
      v[idx].st = time;
      v[idx].ct = time + v[idx].bt;
      v[idx].tat = v[idx].ct - v[idx].at;
      v[idx].wt = v[idx].tat - v[idx].bt;
      v[idx].rt = v[idx].st - v[idx].at;
      time = v[idx].ct;
      useful += v[idx].bt;
      v[idx].iscompleted = true;
      completed++;
    }
    else
    {
      time++;
    }
  }

  qsort(v, n, sizeof(process), comp2);

  int tottat = 0, totwt = 0, totrt = 0;
  printf("\nPid\tAT\tBT\tST\tCT\tTAT\tWT\tRT\n");
  for (int i = 0; i < n; i++)
  {
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
           v[i].pid, v[i].at, v[i].bt, v[i].st, v[i].ct, v[i].tat, v[i].wt, v[i].rt);
    tottat += v[i].tat;
    totwt += v[i].wt;
    totrt += v[i].rt;
  }

  printf("\nAVG. Turn's Around Time\t: %f\n", (float)tottat / n);
  printf("AVG. Waiting Time\t\t: %f\n", (float)totwt / n);
  printf("AVG. Response Time\t\t: %f\n", (float)totrt / n);
  printf("CPU Utilization\t\t\t: %f\n", ((float)useful / time) * 100);
}

int main()
{
  solve();
  return 0;
}
