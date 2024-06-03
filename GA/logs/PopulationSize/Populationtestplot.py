import matplotlib.pyplot as plt 
import csv 
import numpy 
  
def readcsv(filepath):
    with open(filepath,'r') as csvfile: 
        y = [] 
        lines = csv.reader(csvfile, delimiter=',') 
        for row in lines:
            for value in range(0,2001,1):
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


file_paths = [f'logs/PopulationSize/PopulationSize10_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y10 = averagey(file_paths)

file_paths2 = [f'logs/PopulationSize/PopulationSize100_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y100 = averagey(file_paths2)

file_paths3 = [f'logs/PopulationSize/PopulationSize50_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y50 = averagey(file_paths3)

file_paths4 = [f'logs/PopulationSize/PopulationSize40_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 10)]
y40 = averagey(file_paths4)

file_paths6 = [f'logs/PopulationSize/PopulationSize30_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 10)]
y30 = averagey(file_paths6)

file_paths5 = [f'logs/PopulationSize/PopulationSize20_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y20 = averagey(file_paths5)

x = []
for xval in range(1,2002,1):
    x.append(xval)

plt.plot(x, y10, marker='o', markersize=0.5, linestyle='-', label='Population 10')
plt.plot(x, y20, marker='o', markersize=0.5, linestyle='-', label='Population 20')
plt.plot(x, y30, marker='o', markersize=0.5, linestyle='-', label='Population 30')
#plt.plot(x, y40, marker='o', markersize=0.5, linestyle='-', label='Population 40')
plt.plot(x, y50, marker='o', markersize=0.5, linestyle='-', label='Population 50')
plt.plot(x, y100, marker='o', markersize=0.5, linestyle='-', label='Population 100')
plt.legend()
plt.xlabel('Generations')
plt.ylabel('Winrate')
plt.title('Winrate Over Generations')
plt.xticks(range(1, len(x) + 1, 200))
plt.yticks(range(15, 70, 10))  # Y-axis from 0 to 100 with 10-step
plt.grid(True)
plt.show()
  