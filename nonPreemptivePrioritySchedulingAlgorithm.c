// NON-PREEMPTIVE PRIORITY SCHEDULING ALGORITHM.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
  int pid, priority, at, bt, st, ct, tat, wt, rt;
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
  printf("Enter number of processes.\n");
  int n;
  scanf("%d", &n);

  process v[n + 1];
  for (int i = 0; i < n; i++)
  {
    printf("Enter arrival time, burst and priority for process %d :", i);
    scanf("%d%d%d", &v[i].at, &v[i].bt, &v[i].priority);
    v[i].pid = i;
    v[i].iscompleted = false;
  }

  qsort(v, n, sizeof(process), comp1);

  int completed = 0, time = 0, useful = 0;

  while (completed != n)
  {
    int idx = -1, maxi = -1;
    for (int i = 0; i < n; i++)
    {
      if (v[i].at <= time && !v[i].iscompleted)
      {
        if (v[i].priority > maxi)
        {
          maxi = v[i].priority;
          idx = i;
        }
        else if (v[i].priority == maxi && v[i].at < v[idx].at)
        {
          idx = i;
        }
      }
    }

    if (idx != -1)
    {
      v[idx].st = time;
      v[idx].ct = v[idx].st + v[idx].bt;
      v[idx].iscompleted = true;
      completed++;
      time += v[idx].bt;
      useful += v[idx].bt;
    }
    else
    {
      time++;
    }
  }

  for (int i = 0; i < n; i++)
  {
    v[i].tat = v[i].ct - v[i].at;
    v[i].wt = v[i].tat - v[i].bt;
    v[i].rt = v[i].st - v[i].at;
  }

  qsort(v, n, sizeof(process), comp2);

  int tottat = 0, totwt = 0, totrt = 0;
  printf("\nPid\tAT\tBT\tPRI\tST\tCT\tTAT\tWT\tRT\n");
  for (int i = 0; i < n; i++)
  {
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", v[i].pid, v[i].at, v[i].bt, v[i].priority, v[i].st, v[i].ct, v[i].tat, v[i].wt, v[i].rt);
    tottat += v[i].tat;
    totwt += v[i].wt;
    totrt += v[i].rt;
  }

  printf("\nAVG. Turn's Around Time : %f\n", (float)tottat / n);
  printf("AVG. Waiting Time : %f\n", (float)totwt / n);
  printf("AVG. Response Time : %f\n", (float)totrt / n);
  printf("CPU Utilization : %f\n", ((float)useful / time) * 100);
}

int main()
{
  solve();
  return 0;
}
