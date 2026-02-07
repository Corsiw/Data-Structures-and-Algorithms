import numpy as np
import matplotlib

matplotlib.use('TkAgg')

import matplotlib.pyplot as plt
import csv
import math

filename = "leading_zeros.csv"

leading_zeros = []
avg_counts = []

with open(filename, newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        leading_zeros.append(int(row['LeadingZeros']))
        avg_counts.append(float(row['AverageCount']))

leading_zeros = np.array(leading_zeros)
avg_counts = np.array(avg_counts)
total = np.sum(avg_counts)
expected = np.zeros_like(avg_counts)

for i, k in enumerate(leading_zeros):
    if k < 32:
        expected[i] = total * math.pow(2, -(k + 1))
    else:
        expected[i] = total * math.pow(2, -32)

plt.figure(figsize=(10, 6))
width = 0.4

plt.bar(leading_zeros - width / 2, avg_counts, width=width, label='Empirical', color='skyblue')
plt.bar(leading_zeros + width / 2, expected, width=width, label='Theoretical', color='orange', alpha=0.7)

plt.xlabel('Number of Leading Zeros')
plt.ylabel('Count (averaged over runs)')
plt.title('Leading Zeros Distribution in Hashes')
plt.xticks(leading_zeros)
plt.legend()
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.tight_layout()
plt.show()
