#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED

enum Priority {
    FREE = 0,
    PREMIUM = 1
};

enum JobState {
    CREATED,
    EN_COLA,
    ASIGNADO_VRAM,
    FINALIZADO
};

struct Job {
    int id;
    Priority priority;
    JobState state;
    long long enqueueTime;
};

#endif // JOB_H_INCLUDED
