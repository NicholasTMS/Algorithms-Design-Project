import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

class MergeSortStep {
    static class DataPair implements Comparable<DataPair> {
        long number;
        String text;

        DataPair(long number, String text) {
            this.number = number;
            this.text = text;
        }

        @Override
        public String toString() {
            return number + "/" + text;
        }

        @Override
        public int compareTo(DataPair other) {
            return Long.compare(this.number, other.number);
        }
    }

    static void merge(DataPair[] arr, int left, int mid, int right, ArrayList<String> steps) {
        DataPair[] temp = new DataPair[right - left + 1];
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            if (arr[i].compareTo(arr[j]) <= 0) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }

        while (i <= mid) {
            temp[k++] = arr[i++];
        }

        while (j <= right) {
            temp[k++] = arr[j++];
        }

        System.arraycopy(temp, 0, arr, left, temp.length);
        steps.add(Arrays.toString(arr.clone()));
    }

    static void sort(DataPair[] arr, int left, int right, ArrayList<String> steps) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            sort(arr, left, mid, steps);
            sort(arr, mid + 1, right, steps);
            merge(arr, left, mid, right, steps);
        }
    }

    public static void main(String[] args) {
        Scanner userInput = new Scanner(System.in);

        System.out.print("Enter starting line number: ");
        int startLine = userInput.nextInt();

        System.out.print("Enter ending line number: ");
        int endLine = userInput.nextInt();

        if (startLine > endLine) {
            System.out.println("Error: Starting line cannot be greater than ending line.");
        }

        ArrayList<DataPair> dataset = new ArrayList<>();
        File file = new File("dataset.csv");

        try (PrintWriter writer = new PrintWriter("merge_sort_step_" + startLine + "_" + endLine + ".txt")) {
            Scanner fileScanner = new Scanner(file);
            int currentLine = 0;
            while (fileScanner.hasNextLine()) {
                currentLine++;
                String line = fileScanner.nextLine();
                if (currentLine >= startLine && currentLine <= endLine) {
                    String[] parts = line.split(",");
                    if (parts.length >= 2) {
                        try {
                            long num = Long.parseLong(parts[0]);
                            String text = parts[1];
                            dataset.add(new DataPair(num, text));
                        } catch (NumberFormatException e) {
                            System.out.println("Skipping invalid number at line " + currentLine + ": " + parts[0]);
                            writer.println("Skipping invalid number at line " + currentLine + ": " + parts[0]);
                        }
                    }
                } else if (currentLine > endLine) {
                    break;
                }
            }
            fileScanner.close();

            if (dataset.isEmpty()) {
                System.out.println("No data found in the specified range.");
                writer.println("No data found in the specified range.");
                return;
            }

            DataPair[] arr = dataset.toArray(new DataPair[0]);

            System.out.println("\nOriginal subset (lines " + startLine + "-" + endLine + "):");
            writer.println("Original subset (lines " + startLine + "-" + endLine + "):");
            System.out.println(Arrays.toString(arr));
            writer.println(Arrays.toString(arr));

            ArrayList<String> steps = new ArrayList<>();
            steps.add(Arrays.toString(arr.clone())); // Initial state

            long startTime = System.currentTimeMillis();
            sort(arr, 0, arr.length - 1, steps);
            long endTime = System.currentTimeMillis();

            System.out.println("\nSorting steps:");
            writer.println("\nSorting steps:");
            for (int i = 0; i < steps.size(); i++) {
                String stepOutput = "Step " + (i + 1) + ": " + steps.get(i);
                System.out.println(stepOutput);
                writer.println(stepOutput);
            }

            System.out.println("\nFinal sorted subset:");
            writer.println("\nFinal sorted subset:");
            System.out.println(Arrays.toString(arr));
            writer.println(Arrays.toString(arr));

            String timeOutput = "Time taken: " + (endTime - startTime) + " milliseconds";
            System.out.println(timeOutput);
            writer.println(timeOutput);

        } catch (FileNotFoundException e) {
            System.out.println("Error: File not found");
        } finally {
            userInput.close();
        }
    }
}