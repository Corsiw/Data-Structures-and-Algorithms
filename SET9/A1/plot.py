import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

simple = pd.read_csv("out2.csv")
string = pd.read_csv("out3.csv")

Path("plots").mkdir(exist_ok=True)

datasets = [
    ("Random", "Random"),
    ("Reversed", "Reversed"),
    ("AlmostSorted", "AlmostSorted")
]

time_columns = {
    "Simple QuickSort": {
        "Random": "QSRandomTimeNs",
        "Reversed": "QSReversedTimeNs",
        "AlmostSorted": "QSAlmostSortedTimeNs"
    },
    "Simple MergeSort": {
        "Random": "MSRandomTimeNs",
        "Reversed": "MSReversedTimeNs",
        "AlmostSorted": "MSAlmostSortedTimeNs"
    },
    "String QuickSort": {
        "Random": "SQRandomTimeNs",
        "Reversed": "SQReversedTimeNs",
        "AlmostSorted": "SQAlmostSortedTimeNs"
    },
    "String MergeSort": {
        "Random": "SMRandomTimeNs",
        "Reversed": "SMReversedTimeNs",
        "AlmostSorted": "SMAlmostSortedTimeNs"
    },
    "MSD Radix Sort": {
        "Random": "MSDRandomTimeNs",
        "Reversed": "MSDReversedTimeNs",
        "AlmostSorted": "MSDAlmostSortedTimeNs"
    },
    "MSD Radix + QuickSort": {
        "Random": "MSDHRandomTimeNs",
        "Reversed": "MSDHReversedTimeNs",
        "AlmostSorted": "MSDHAlmostSortedTimeNs"
    }
}

comparison_columns = {
    "Simple QuickSort": {
        "Random": "QSRandomComp",
        "Reversed": "QSReversedComp",
        "AlmostSorted": "QSAlmostSortedComp"
    },
    "Simple MergeSort": {
        "Random": "MSRandomComp",
        "Reversed": "MSReversedComp",
        "AlmostSorted": "MSAlmostSortedComp"
    },
    "String QuickSort": {
        "Random": "SQRandomComp",
        "Reversed": "SQReversedComp",
        "AlmostSorted": "SQAlmostSortedComp"
    },
    "String MergeSort": {
        "Random": "SMRandomComp",
        "Reversed": "SMReversedComp",
        "AlmostSorted": "SMAlmostSortedComp"
    },
    "MSD Radix Sort": {
        "Random": "MSDRandomComp",
        "Reversed": "MSDReversedComp",
        "AlmostSorted": "MSDAlmostSortedComp"
    },
    "MSD Radix + QuickSort": {
        "Random": "MSDHRandomComp",
        "Reversed": "MSDHReversedComp",
        "AlmostSorted": "MSDHAlmostSortedComp"
    }
}

for dataset_name, key in datasets:

    plt.figure(figsize=(12, 8))

    for algorithm, mapping in time_columns.items():

        source = simple if algorithm.startswith("Simple") else string

        plt.plot(
            source["SampleSize"],
            source[mapping[key]],
            marker='o',
            label=algorithm
        )

    plt.title(f"Execution Time ({dataset_name})")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ns)")
    plt.grid(True)
    plt.legend()

    plt.tight_layout()
    plt.savefig(f"plots/time_{dataset_name.lower()}.png")
    plt.close()

for dataset_name, key in datasets:

    plt.figure(figsize=(12, 8))

    for algorithm, mapping in comparison_columns.items():

        source = simple if algorithm.startswith("Simple") else string

        plt.plot(
            source["SampleSize"],
            source[mapping[key]],
            marker='o',
            label=algorithm
        )

    plt.title(f"Character Comparisons ({dataset_name})")
    plt.xlabel("Array Size")
    plt.ylabel("Number of Character Comparisons")
    plt.grid(True)
    plt.legend()

    plt.tight_layout()
    plt.savefig(f"plots/comparisons_{dataset_name.lower()}.png")
    plt.close()

print("Graphs saved to ./plots")