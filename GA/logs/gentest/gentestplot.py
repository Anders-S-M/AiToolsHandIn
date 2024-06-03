import matplotlib.pyplot as plt 
import csv 
import numpy 
  
def readcsv(filepath):
    with open(filepath,'r') as csvfile: 
        y = [] 
        lines = csv.reader(csvfile, delimiter=',') 
        for row in lines:
            for value in range(0,3001,1):
                y.append(row[value]) 
    return y
  
def averagey(filepaths):
    tempy = []
    for file in filepaths:
        y=readcsv(file)
        tempy.append(y)
    tempy = [sum(float(number) for number in index) / len(index) for index in zip(*tempy)]
    tempy = [round(num, 2) for num in tempy]
    return tempy


file_paths = [f'GA/logs/gentest/PopulationSize40_gameCount50_mutationRate0.100000_generations3000_run{i}.csv' for i in range(1, 11)]
y01 = averagey(file_paths)



x = []
for xval in range(1,3002,1):
    x.append(xval)

plt.plot(x, y01, marker='o', markersize=0.5, linestyle='-')

plt.legend()
plt.xlabel('Generations')
plt.ylabel('Winrate')
plt.title('Winrate Over Generations')
plt.xticks(range(1, len(x) + 1, 400))
plt.yticks(range(15, 70, 10))  # Y-axis from 0 to 100 with 10-step
plt.grid(True)
plt.show()
  