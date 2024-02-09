#

import pandas as pd
import matplotlib.pyplot as plt

if __name__ == '__main__':
    df = pd.read_csv('/tmp/c4.txt', names=["Tick", "Amplitude"], header=None)
    df.plot(x='Tick', y='Amplitude') # kind='scatter'
    plt.show()
