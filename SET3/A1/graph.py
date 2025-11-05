import pandas as pd
import matplotlib.pyplot as plt

# === Чтение CSV ===
# Укажи путь к своему файлу:
filename = "out.csv"

df = pd.read_csv(filename, delim_whitespace=True)

print("Прочитанные данные:")
print(df.head())

# Ожидаемые названия колонок:
# Количество_точек | Площадь_большие_границы | Отн_откл_большие | Площадь_малые_границы | Отн_откл_малые
# Можно переименовать их для удобства:
df.columns = [
    "points",
    "area_large",
    "rel_err_large",
    "area_small",
    "rel_err_small"
]

# === График 1: Площадь с большими границами от количества точек ===
plt.figure(figsize=(8, 5))
plt.plot(df["points"], df["area_large"], marker="o")
plt.title("Площадь с большими границами от количества точек")
plt.xlabel("Количество генерируемых точек")
plt.ylabel("Площадь (большие границы)")
plt.grid(True)
plt.tight_layout()
plt.show()

# === График 2: Площадь с маленькими границами от количества точек ===
plt.figure(figsize=(8, 5))
plt.plot(df["points"], df["area_small"], marker="o", color="orange")
plt.title("Площадь с маленькими границами от количества точек")
plt.xlabel("Количество генерируемых точек")
plt.ylabel("Площадь (маленькие границы)")
plt.grid(True)
plt.tight_layout()
plt.show()

# === График 3: Относительное отклонение (большие границы) ===
plt.figure(figsize=(8, 5))
plt.plot(df["points"], df["rel_err_large"], marker="o", color="green")
plt.title("Относительное отклонение (большие границы)")
plt.xlabel("Количество генерируемых точек")
plt.ylabel("Относительное отклонение")
plt.grid(True)
plt.tight_layout()
plt.show()

# === График 4: Относительное отклонение (маленькие границы) ===
plt.figure(figsize=(8, 5))
plt.plot(df["points"], df["rel_err_small"], marker="o", color="red")
plt.title("Относительное отклонение (маленькие границы)")
plt.xlabel("Количество генерируемых точек")
plt.ylabel("Относительное отклонение")
plt.grid(True)
plt.tight_layout()
plt.show()
