import subprocess
import csv

print('Compiling C program...')
subprocess.run(['make'])


program_path = 'Linear/build/ludo'
print('Running test...')

Gencount = 0

def append_to_csv(file_name, value):
    global Gencount
    Gencount = Gencount + 1
    with open(file_name, 'a') as file:
        file.write(str(value) + ',')

def run(gains):
    # Convert integers to strings
    args = [str(arg) for arg in gains]
    # Run the subprocess and capture the output
    result = subprocess.run(args=[program_path] + args, capture_output=True, text=True)

    # Check if the subprocess ran successfully
    if result.returncode == 0:
        # Convert the output to a number
        try:
            output_number = float(result.stdout.strip())
            #print("Output number:", output_number)
            append_to_csv(f"Linear/logs/linear.csv", output_number)
            return output_number
        except ValueError:
            print("Error: Output is not a valid number")
    else:
        print("Error:", result.stderr)


def adjust_weights(weights):
    change1 = True
    while(change1):
        change1 = False
        baseline = run(weights)
        print("Baseline:", baseline)
        baseline = baseline + 2 # To counteract uncertainties, make it pessimistic
        for i in range(len(weights)):
            change2 = True
            while(change2):
                change2 = False

                original_weight = weights[i]
                weight_up = original_weight

                # Adjust the weight up
                if weights[i] < 20:
                    if weights[i] > 0:
                        if baseline < 40:
                            weights[i] += 3
                        else:
                            weights[i] += 1
                        weight_up = weights[i]
                adjusted_output_up = run(weights)

                # Adjust the weight down
                if weights[i] > -20:
                    if weights[i] < 0:
                            if baseline < 40:
                                weights[i] = original_weight - 4
                            else:
                                weights[i] = original_weight - 1
                adjusted_output_down = run(weights)

                # Decide whether to keep the adjustment
                if adjusted_output_up is not None and adjusted_output_down is not None:
                    if adjusted_output_up > baseline: #and adjusted_output_up >= adjusted_output_down:
                        print(f"Weight {i} adjusted: {original_weight} -> {weight_up}, Output: {adjusted_output_up}")
                        weights[i] = weight_up
                        baseline = adjusted_output_up
                        change1 = True
                        change2 = True
                    elif adjusted_output_down > baseline: #and adjusted_output_down > adjusted_output_up:
                        print(f"Weight {i} adjusted: {original_weight} -> {weights[i]}, Output: {adjusted_output_down}")
                        baseline = adjusted_output_down
                        change1 = True
                        change2 = True
                    else:
                        weights[i] = original_weight  # Revert to original weight
        print(weights)
    return weights

# original_weights = [1, 2, 3, 4, -15, 10, 8, 2, 2, 9, -3, -4] # 56 % winrate
original_weights =  [1, 1, 1, 1, -1, 1, 1, 1, 1, -1, -1]

weights = original_weights
while(Gencount < 2000):
    new_weights = adjust_weights(weights)
    weights = new_weights
    print("Run", Gencount, "done")

print("All runs done")
new_baseline = run(new_weights)
print("New weights:", new_weights, "Winrate:", new_baseline)
