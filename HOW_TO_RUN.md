# How to Run — CCP6214 Assignment

Complete walkthrough, from compiling the code to running every program across
all 10 dataset sizes. All programs are **command-line based** (no menus): you
pass the inputs as arguments. **Commands below are for PowerShell** (Git Bash
equivalents are in Section 3.8).

---

## 0. Prerequisites

- A C++17 compiler (`g++`). Check with `g++ --version`.
- Open **PowerShell inside the `src/` folder** (all commands assume you are in `src/`):
  ```powershell
  cd src
  ```

> In PowerShell you run a program with a leading `.\` and the `.exe` suffix,
> e.g. `.\radix_sort.exe dataset_1000.csv`.

---

## 1. Compile everything

Every program is self-contained — compile each on its own (there is no `utils.cpp` to link).
(These `g++` commands work the same in PowerShell.)

```powershell
g++ -std=c++17 -O2 dataset_generator.cpp      -o dataset_generator
g++ -std=c++17 -O2 radix_sort.cpp             -o radix_sort
g++ -std=c++17 -O2 heap_sort.cpp              -o heap_sort
g++ -std=c++17 -O2 hash_table_search.cpp      -o hash_table_search
g++ -std=c++17 -O2 radix_sort_step.cpp        -o radix_sort_step
g++ -std=c++17 -O2 heap_sort_step.cpp         -o heap_sort_step
g++ -std=c++17 -O2 hash_table_search_step.cpp -o hash_table_search_step
```

---

## 2. The 10 dataset sizes (n)

```
1000  5000  10000  50000  100000  500000  1000000  5000000  10000000  50000000
```

---

## 3. Run each program across all 10 sizes (one loop per program)

Each block is a self-contained PowerShell loop — paste any one on its own.
**Run 3.1 first** (it creates the datasets the others read).

### 3.1 Generate all datasets
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) { .\dataset_generator.exe $n }
```

### 3.2 Radix sort
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) { .\radix_sort.exe "dataset_$n.csv" }
```

### 3.3 Heap sort
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) { .\heap_sort.exe "dataset_$n.csv" }
```

### 3.4 Hash table search (best / average / worst timing)
Auto-picks an existing key (first key in each file) for the search.
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) {
    $key = (Get-Content "dataset_$n.csv" -First 1).Split(',')[0]
    .\hash_table_search.exe "dataset_$n.csv" $key
}
```

### 3.5 Radix sort step (rows 1–7)
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) { .\radix_sort_step.exe "dataset_$n.csv" 1 7 }
```

### 3.6 Heap sort step (rows 1–7)
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) { .\heap_sort_step.exe "dataset_$n.csv" 1 7 }
```

### 3.7 Hash table search step
```powershell
foreach ($n in 1000,5000,10000,50000,100000,500000,1000000,5000000,10000000,50000000) {
    $key = (Get-Content "dataset_$n.csv" -First 1).Split(',')[0]
    .\hash_table_search_step.exe "dataset_$n.csv" $key
}
```

> The **step** programs (3.5–3.7) show the steps for only rows 1–7, so you
> normally just need them on the **smallest** size (1000) for the demo. The loops
> above run them on every size in case you want all the files.

### 3.8 Git Bash equivalents (only if you use Git Bash, not PowerShell)
```bash
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do ./dataset_generator $n; done
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do ./radix_sort dataset_$n.csv; done
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do ./heap_sort  dataset_$n.csv; done
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do key=$(head -1 dataset_$n.csv | cut -d, -f1); ./hash_table_search dataset_$n.csv $key; done
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do ./radix_sort_step dataset_$n.csv 1 7; done
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do ./heap_sort_step  dataset_$n.csv 1 7; done
for n in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000; do key=$(head -1 dataset_$n.csv | cut -d, -f1); ./hash_table_search_step dataset_$n.csv $key; done
```

> ⚠️ **The big sizes take time and memory.** `50,000,000` rows need roughly
> 1.5–2 GB of RAM and the sorts can take a while. If your PC struggles, run the
> small sizes first, then the large ones separately.

---

## 4. Output files (per size n)

| Program | Output |
|---|---|
| dataset_generator | `dataset_<n>.csv` |
| radix_sort | `radix_sorted_dataset_<n>.csv` (+ running time on screen) |
| heap_sort | `heap_sorted_dataset_<n>.csv` (+ running time on screen) |
| radix_sort_step | `dataset_<n>_radix_sorted_step_1_7.txt` |
| heap_sort_step | `dataset_<n>_heap_sorted_step_1_7.txt` |
| hash_table_search | `dataset_<n>_hash_table_search_step_<key>.txt`, `hash_table_search_dataset_<n>.txt` |
| hash_table_search_step | `dataset_<n>_hash_table_search_step_<key>.txt` |

Hash step found vs not-found example (size 1000, PowerShell):
```powershell
.\hash_table_search_step.exe dataset_1000.csv 1342720758   # found     -> 1342720758 = 1342720758/lwrop
.\hash_table_search_step.exe dataset_1000.csv 123456789    # not found -> -1 != 123456789
```

---

## 5. Notes

- **Same seed → same data.** The generator uses a fixed seed (`2431324631`,
  the group leader's student ID), so re-running `.\dataset_generator.exe 1000`
  always makes the same `dataset_1000.csv`.
- **Missing arguments** print a `Usage:` line and exit without crashing, e.g.
  `Usage: .\radix_sort.exe <dataset_file.csv>`.
- **Separators:** input dataset uses commas (`1342720758,lwrop`); all output
  files use slashes (`1342720758/lwrop`) to match the assignment sample format.
- **Running time excludes file I/O** — only the algorithm itself is timed.
- For the report, record the radix vs heap running times for each of the 10 sizes.
```
