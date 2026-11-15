import json
import matplotlib.pyplot as plt

with open("results.json") as fp:
    data = json.load(fp)

times = [time / 1000 for time in data["times"]]
sizes = data["sizes"]

fig, ax = plt.subplots(figsize=(18, 10))
ax.plot(range(len(times)), times, marker="o", linestyle="-", color="tab:blue")

ax.set_xticks(range(len(sizes)))
ax.set_xticklabels(sizes, rotation=90, fontsize=9)

ax.set_xlabel("size")
ax.set_ylabel("time (s)")
ax.set_title("Render time per size")
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("time_vs_size.png", dpi=150)
plt.show()
