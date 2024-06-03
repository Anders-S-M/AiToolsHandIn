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


file_paths = [f'logs/MutationRate/PopulationSize20_gameCount100_mutationRate0.100000_generations2000_run{i}.csv' for i in range(1, 11)]
y01 = averagey(file_paths)

file_paths2 = [f'logs/MutationRate/PopulationSize20_gameCount100_mutationRate0.500000_generations2000_run{i}.csv' for i in range(1, 11)]
y05 = averagey(file_paths2)

file_paths3 = [f'logs/MutationRate/PopulationSize20_gameCount100_mutationRate1.000000_generations2000_run{i}.csv' for i in range(1, 11)]
y1 = averagey(file_paths3)

file_paths4 = [f'logs/MutationRate/PopulationSize20_gameCount100_mutationRate0.000000_generations2000_run{i}.csv' for i in range(1, 10)]
y0 = averagey(file_paths4)


x = []
for xval in range(1,2002,1):
    x.append(xval)

plt.plot(x, y0, marker='o', markersize=0.5, linestyle='-', label='Mutation 0')
plt.plot(x, y01, marker='o', markersize=0.5, linestyle='-', label='Mutation 0.1')
plt.plot(x, y05, marker='o', markersize=0.5, linestyle='-', label='Mutation 0.5')
plt.plot(x, y1, marker='o', markersize=0.5, linestyle='-', label='Mutation 1')
plt.legend()
plt.xlabel('Generations')
plt.ylabel('Winrate')
plt.title('Winrate Over Generations')
plt.xticks(range(1, len(x) + 1, 200))
plt.yticks(range(15, 70, 10))  # Y-axis from 0 to 100 with 10-step
plt.grid(True)
plt.show()
  