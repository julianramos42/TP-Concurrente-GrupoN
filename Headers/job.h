#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED

enum Priority {
    FREE = 0,
    PREMIUM = 1
};

struct Job {
    int id;
    Priority priority;
    long long enqueueTime;
};

#endif // JOB_H_INCLUDED
