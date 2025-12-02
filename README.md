# 🧠 Philosophers (philo)

Implementation of the classic **Dining Philosophers Problem** in C, following the 42 School project.  
This repository contains both the **mandatory (threads + mutexes)** version and the **bonus (processes + semaphores)** version.

The goal is to simulate philosophers sitting around a table, alternating between **eating**, **thinking**, and **sleeping**, while sharing forks and respecting strict timing and synchronization rules.

---

## 📌 Features

### ✅ Mandatory Part — Threads & Mutexes
- One thread per philosopher  
- Each fork represented by a `pthread_mutex_t`  
- Safe locking to avoid race conditions  
- Accurate millisecond-based timing  
- Synchronized console output (`print_action`)  
- Death detection (if a philosopher doesn't eat before `time_to_die`)  
- Optional meal limit to stop simulation cleanly  
- Clean resource cleanup on exit  

---

### ⭐ Bonus Part — Processes & Semaphores
- One **process per philosopher**  
- Forks controlled by a **semaphore pool**  
- Output protected by a global semaphore  
- Death monitoring inside each philosopher process  
- Clean kill/termination of all processes on death  
- Uses POSIX semaphores (`sem_open`, `sem_wait`, `sem_post`, ...)  

---

## 🧵 Mandatory Usage (Threads)

```bash
cd philo
make
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philo_must_eat]
