import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

class MergeSort {
    // A class to hold both the number and its corresponding string
    static class DataPair {
        public static int amount = 0;
        long number;
        String text;

        DataPair(long number, String text) {
            this.amount++;
            this.number = number;
            this.text = text;
        }

        @Override
        public String toString() {
            return number + "," + text;
        }

        // Method to format as CSV line
        String toCSV() {
            return number + "," + text;
        }
    }

    // Merges two subarrays of arr[]
    static void merge(DataPair[] arr, int left_index, int middle_index, int right_index) {
        int left_array_index = middle_index - left_index + 1;
        int right_array_index = right_index - middle_index;

        DataPair[] left_array = new DataPair[left_array_index];
        DataPair[] right_array = new DataPair[right_array_index];

        for (int i = 0; i < left_array_index; ++i) {
            left_array[i] = arr[left_index + i];
        }
        for (int j = 0; j < right_array_index; ++j) {
            right_array[j] = arr[middle_index + 1 + j];
        }

        int first_left = 0;
        int first_right = 0;
        int k = left_index;

        while (first_left < left_array_index && first_right < right_array_index) {
            if (left_array[first_left].number <= right_array[first_right].number) {
                arr[k] = left_array[first_left];
                first_left++;
            } else {
                arr[k] = right_array[first_right];
                first_right++;
            }
            k++;
        }

        while (first_left < left_array_index) {
            arr[k] = left_array[first_left];
            first_left++;
            k++;
        }

        while (first_right < right_array_index) {
            arr[k] = right_array[first_right];
            first_right++;
            k++;
        }
    }

    static void sort(DataPair[] arr, int left_index, int right_index) {
        if (left_index < right_index) {
            int m = left_index + (right_index - left_index) / 2;
            sort(arr, left_index, m);
            sort(arr, m + 1, right_index);
            merge(arr, left_index, m, right_index);
        }
    }

    static void writeToCSV(DataPair[] arr, String filename) {
        try (FileWriter writer = new FileWriter(filename)) {
            // Write header if needed
            writer.write("Number,Text\n");

            for (DataPair pair : arr) {
                writer.write(pair.toCSV() + "\n");
            }
            System.out.println("Successfully wrote sorted data to " + filename);
        } catch (IOException e) {
            System.out.println("An error occurred while writing to the file.");
            e.printStackTrace();
        }
    }

    public static void main(String[] a) {
        ArrayList<DataPair> dataset = new ArrayList<>();

        File file = new File("dataset.csv");

        try {
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] parts = line.split("/");
                if (parts.length >= 2) {
                    try {
                        long num = Long.parseLong(parts[0]);
                        String text = parts[1];
                        dataset.add(new DataPair(num, text));
                    } catch (NumberFormatException e) {
                        System.out.println("Skipping invalid number: " + parts[0]);
                        continue;
                        // Skip this line if the number is invalid
                    }
                }
            }
            scanner.close();

            // Convert ArrayList to array
            DataPair[] arr = new DataPair[dataset.size()];
            arr = dataset.toArray(arr);

            long startTime = System.currentTimeMillis();
            sort(arr, 0, arr.length - 1);
            long endTime = System.currentTimeMillis();

            // Write the sorted data to a new CSV file
            writeToCSV(arr, ("merge_sort_" + DataPair.amount + ".csv"));

            long milliseconds = endTime - startTime;

            System.out.println("Time taken: " + milliseconds + " Milliseconds");

        } catch (FileNotFoundException e) {
            System.out.println("Input file not found");
        }
    }
}