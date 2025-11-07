import pandas as pd
import matplotlib.pyplot as plt

filename = "out.csv"

df = pd.read_csv(filename, delim_whitespace=True)

df.columns = [
    "n",
    "random_time_simple",
    "reversed_time_simple",
    "sorted_time_simple",
    "random_time_ins",
    "reversed_time_ins",
    "sorted_time_ins"
]

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["random_time_simple"], marker=".", linestyle='None')
plt.title("Рандомный массив + Simple Quick Sort")
plt.xlabel("n размер массива")
plt.ylabel("Время работы в наносекундах")
plt.grid(True)
plt.tight_layout()
plt.show()

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["reversed_time_simple"], marker=".", linestyle='None')
plt.title("Реверснутый массив + Simple Quick Sort")
plt.xlabel("n размер массива")
plt.ylabel("Время работы в наносекундах")
plt.grid(True)
plt.tight_layout()
plt.show()

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["sorted_time_simple"], marker=".", linestyle='None')
plt.title("Почти отсортированный массив + Simple Quick Sort")
plt.xlabel("n размер массива")
plt.ylabel("Время работы в наносекундах")
plt.grid(True)
plt.tight_layout()
plt.show()

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["random_time_ins"], marker=".", linestyle='None')
plt.title("Рандомный массив + Introsort")
plt.xlabel("n размер массива")
plt.ylabel("Время работы в наносекундах")
plt.grid(True)
plt.tight_layout()
plt.show()

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["reversed_time_ins"], marker=".", linestyle='None')
plt.title("Реверснутый массив + Introsort")
plt.xlabel("n размер массива")
plt.ylabel("Время работы в наносекундах")
plt.grid(True)
plt.tight_layout()
plt.show()

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["sorted_time_ins"], marker=".", linestyle='None')
plt.title("Почти отсортированный массив + Introsort")
plt.xlabel("n размер массива")
plt.ylabel("Время работы в наносекундах")
plt.grid(True)
plt.tight_layout()
plt.show()
