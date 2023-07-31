import subprocess
import difflib
import time

c_source_file = "main.c"  # replace with the actual path if needed
c_executable = "program"
report_file = "Report.txt"  # replace with the actual path if needed

# Generate list of input and output file names
input_files = [f"TestCases/Open/open_{i}.txt" for i in range(1, 112)]  # replace with actual path if needed
output_files = [f"TestCases/Open/open_{i}.output.txt" for i in range(1, 112)]  # replace with actual path if needed

def compile_c_program(source_file, output_file):
    # Run the gcc command to compile the C program
    result = subprocess.run(["gcc", "-o", output_file, source_file])

    if result.returncode == 0:
        print("Compilation successful!")
    else:
        print("Compilation failed!")
        exit(1)  # Exit the script if compilation fails

def run_test(c_program, input_file, output_file):
    # Run the C program with the input file and capture the output
    with open(input_file, "r") as infile:
        start_time = time.time()
        result = subprocess.run([c_program], stdin=infile, capture_output=True, text=True)
        end_time = time.time()

    execution_time = end_time - start_time

    print(f"\n{'='*50}")
    print(f"Running test with {input_file}...")
    print(f"Execution time: {execution_time} seconds")

    # Display the return code of the C program
    print(f"Return code: {result.returncode}")

    # Read the expected output file
    with open(output_file, "r") as outfile:
        expected_output = outfile.read()

    # Trim trailing spaces from each line
    actual_output_lines = [line.rstrip() for line in result.stdout.splitlines()]
    expected_output_lines = [line.rstrip() for line in expected_output.splitlines()]

    # Compare the program output with the expected output
    if actual_output_lines == expected_output_lines:
        print(f"\nTest PASSED!\n{'-'*50}")
        return True, execution_time, ''  # return True if test passed
    else:
        print(f"\nTest FAILED!\n{'-'*50}")

        # Generate the differences between the actual and expected outputs
        diff = difflib.context_diff(actual_output_lines,
                                    expected_output_lines,
                                    fromfile='Actual Output',
                                    tofile='Expected Output')
        diff_report = "\nDifferences:\n" + ''.join(diff) + '\n'

        print(diff_report)

        return False, execution_time, diff_report  # return False if test failed

# Compile the C program
compile_c_program(c_source_file, c_executable)

# Open the report file
with open(report_file, "w") as report:
    report.write("TEST REPORT\n\n")

    # Run the test with each input file
    for input_file, output_file in zip(input_files, output_files):
        passed, execution_time, diff_report = run_test(c_executable, input_file, output_file)
        report.write(f"Test with {input_file}:\n")
        report.write(f"Execution time: {execution_time} seconds\n")
        if passed:
            report.write("Test PASSED!\n")
        else:
            report.write("Test FAILED!\n")
            report.write(diff_report)
            report.write("-"*50 + "\n")
            print("Stopping at first failed test.")
            exit(1)  # exit if a test fails

    report.write("="*50 + "\n")
    report.write("Testing completed!\n")

print("Testing completed! Check the report file for details.")
