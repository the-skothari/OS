// nonPreemptiveSJF.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h> // for INT_MAX

typedef struct
{
  int pid, at, bt, st, ct, tat, wt, rt;
  bool iscompleted;
} process;

// Sort by arrival time for initial ordering
int comp(const void *a, const void *b)
{
  process *p1 = (process *)a;
  process *p2 = (process *)b;
  return p1->at - p2->at;
}

void solve()
{
  int n;
  printf("Enter number of Processes : ");
  scanf("%d", &n);

  process v[n];
  for (int i = 0; i < n; i++)
  {
    printf("Enter arrival time and burst time for process %d : ", i + 1);
    v[i].pid = i + 1;
    scanf("%d %d", &v[i].at, &v[i].bt);
    v[i].iscompleted = false;
  }

  qsort(v, n, sizeof(process), comp);

  int completed = 0, currtime = 0;
  int gantt[n], starttime[n];
  int gantt_idx = 0;
  float avgtat = 0, avgwt = 0, avgrt = 0;

  while (completed != n)
  {
    int minbt = INT_MAX, sel_idx = -1;

    for (int i = 0; i < n; i++)
    {
      if (v[i].at <= currtime && !v[i].iscompleted)
      {
        if (v[i].bt < minbt)
        {
          minbt = v[i].bt;
          sel_idx = i;
        }
        else if (v[i].bt == minbt && v[i].at < v[sel_idx].at)
        {
          sel_idx = i;
        }
      }
    }

    if (sel_idx == -1)
    {
      currtime++; // CPU idle
    }
    else
    {
      v[sel_idx].st = currtime;
      v[sel_idx].ct = currtime + v[sel_idx].bt;
      v[sel_idx].tat = v[sel_idx].ct - v[sel_idx].at;
      v[sel_idx].wt = v[sel_idx].tat - v[sel_idx].bt;
      v[sel_idx].rt = v[sel_idx].st - v[sel_idx].at;

      avgtat += v[sel_idx].tat;
      avgwt += v[sel_idx].wt;
      avgrt += v[sel_idx].rt;

      starttime[gantt_idx] = v[sel_idx].st;
      gantt[gantt_idx] = v[sel_idx].pid;
      gantt_idx++;

      currtime = v[sel_idx].ct;
      v[sel_idx].iscompleted = true;
      completed++;
    }
  }

  // Find first arrival time for throughput calculation
  int first_arrival = v[0].at;
  for (int i = 1; i < n; i++)
  {
    if (v[i].at < first_arrival)
      first_arrival = v[i].at;
  }

  int total_time = currtime;
  int total_bt = 0;
  for (int i = 0; i < n; i++)
  {
    total_bt += v[i].bt;
  }

  float cpu_util = ((float)total_bt / total_time) * 100.0;
  float throughput = (float)n / (total_time - first_arrival);

  // Output
  printf("\nProcess\tAT\tBT\tST\tCT\tTAT\tWT\tRT\n");
  for (int i = 0; i < n; i++)
  {
    printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
           v[i].pid, v[i].at, v[i].bt, v[i].st,
           v[i].ct, v[i].tat, v[i].wt, v[i].rt);
  }

  printf("\nAverage Turnaround Time : %.2f\n", avgtat / n);
  printf("Average Waiting Time     : %.2f\n", avgwt / n);
  printf("Average Response Time    : %.2f\n", avgrt / n);
  printf("CPU Utilization          : %.2f%%\n", cpu_util);
  printf("Throughput               : %.2f processes/unit time\n", throughput);

  // Gantt Chart
  printf("\nGantt Chart:\n");
  for (int i = 0; i < gantt_idx; i++)
  {
    printf("| P%d ", gantt[i]);
  }
  printf("|\n");

  printf("%d", starttime[0]);
  for (int i = 1; i < gantt_idx; i++)
  {
    printf("   %d", starttime[i]);
  }
  printf("   %d\n", currtime);
}

int main()
{
  solve();
  return 0;
}
