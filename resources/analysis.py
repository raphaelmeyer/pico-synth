#

import pandas as pd
import matplotlib.pyplot as plt

if __name__ == '__main__':
    df = pd.read_csv('/tmp/waves.csv')
    print(df)

    df.plot(subplots=True)
    plt.show()
