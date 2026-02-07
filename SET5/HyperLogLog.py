import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import pandas as pd

# График 1: Nt vs F0 по шагам t
df1 = pd.read_csv("graph1_data.csv")

plt.figure(figsize=(10,6))

for stream_num in df1['stream_num'].unique():
    df_stream = df1[df1['stream_num']==stream_num]
    plt.plot(df_stream['frac'], df_stream['Exact'], 'o-', label=f'Exact stream {stream_num}')
    plt.plot(df_stream['frac'], df_stream['F_HLL'], 'x--', label=f'HLL stream {stream_num}')

plt.xlabel('Fraction of stream processed')
plt.ylabel('Number of unique elements')
plt.title('HLL estimate vs Exact F0')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("graph1.png", dpi=300)
plt.show()

# График 2: статистика оценки
df2 = pd.read_csv("graph2_data.csv")

frac = df2['frac']
meanHLL = df2['Mean_HLL']
stdHLL = df2['Std_HLL']

plt.figure(figsize=(10,6))
plt.plot(frac, meanHLL, 'o-', color='blue', label='Mean HLL estimate')
plt.fill_between(frac, meanHLL - stdHLL, meanHLL + stdHLL, color='blue', alpha=0.2, label='±1 StdDev')

plt.xlabel('Fraction of stream processed')
plt.ylabel('HLL estimate')
plt.title('HLL estimate statistics over streams')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("graph2.png", dpi=300)
plt.show()
