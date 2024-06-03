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
  
file_paths = [f'logs/averagetest/PopulationSize40_gameCount50_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 21)]

averagey = []
for file in file_paths:
    y=readcsv(file)
    averagey.append(y)


averagey = [sum(float(number) for number in index) / len(index) for index in zip(*averagey)]
averagey = [round(num, 2) for num in averagey]

file_paths2 = [f'logs/averagetest/PopulationSize40_gameCount50_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]

averagey2 = []
for file in file_paths2:
    y=readcsv(file)
    averagey2.append(y)

averagey2 = [sum(float(number) for number in index) / len(index) for index in zip(*averagey2)]
averagey2 = [round(num, 2) for num in averagey2]

file_paths3 = [f'logs/averagetest/PopulationSize40_gameCount50_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 2)]

averagey3 = []
for file in file_paths3:
    y=readcsv(file)
    averagey3.append(y)

averagey3 = [sum(float(number) for number in index) / len(index) for index in zip(*averagey3)]
averagey3 = [round(num, 2) for num in averagey3]

file_paths4 = [f'logs/averagetest/PopulationSize40_gameCount50_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 6)]

averagey4 = []
for file in file_paths4:
    y=readcsv(file)
    averagey4.append(y)

averagey4 = [sum(float(number) for number in index) / len(index) for index in zip(*averagey4)]
averagey4 = [round(num, 2) for num in averagey4]

x = []
for xval in range(1,2002,1):
    x.append(xval)

plt.plot(x, averagey3, marker='o', markersize=0.5, linestyle='-', label='1 run')
plt.plot(x, averagey4, marker='o', markersize=0.5, linestyle='-', label='5 run average')
plt.plot(x, averagey2, marker='o', markersize=0.5, linestyle='-', label='10 run average')
plt.plot(x, averagey, marker='o', markersize=0.5, linestyle='-', label='20 run average')
plt.legend()
plt.xlabel('Generations')
plt.ylabel('Winrate')
plt.title('Winrate Over Generations')
plt.xticks(range(1, len(y) + 1, 200))
plt.yticks(range(15, 70, 10))  # Y-axis from 0 to 100 with 10-step
plt.grid(True)
plt.show()
  