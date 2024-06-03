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


file_paths = [f'logs/Games/PopulationSize40_gameCount5_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y5 = averagey(file_paths)

file_paths2 = [f'logs/Games/PopulationSize40_gameCount10_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y10 = averagey(file_paths2)

file_paths3 = [f'logs/Games/PopulationSize40_gameCount20_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y20 = averagey(file_paths3)

file_paths4 = [f'logs/Games/PopulationSize40_gameCount50_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 9)]
y50 = averagey(file_paths4)

file_paths6 = [f'logs/Games/PopulationSize40_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 10)]
y100 = averagey(file_paths6)

file_paths5 = [f'logs/Games/PopulationSize40_gameCount400_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y400 = averagey(file_paths5)

x = []
for xval in range(1,2002,1):
    x.append(xval)

#plt.plot(x, y5, marker='o', markersize=0.5, linestyle='-', label='5 games per generation')
plt.plot(x, y10, marker='o', markersize=0.5, linestyle='-', label='10 games per generation')
#plt.plot(x, y20, marker='o', markersize=0.5, linestyle='-', label='20 games per generation')
plt.plot(x, y50, marker='o', markersize=0.5, linestyle='-', label='50 games per generation')
plt.plot(x, y100, marker='o', markersize=0.5, linestyle='-', label='100 games per generation')
plt.plot(x, y400, marker='o', markersize=0.5, linestyle='-', label='400 games per generation')
plt.legend()
plt.xlabel('Generations')
plt.ylabel('Winrate')
plt.title('Winrate Over Generations')
plt.xticks(range(1, len(x) + 1, 200))
plt.yticks(range(15, 70, 10))  # Y-axis from 0 to 100 with 10-step
plt.grid(True)
plt.show()
  