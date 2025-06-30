import javax.xml.crypto.Data;
import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

class DataRow {
    public long numbers;
    public String characters;

    DataRow(long num, String chars) {
        numbers = num;
        characters = chars;
    }
}

public class quick_sort {
    // Main program
    public static void main(String[] args) {
        // Check that argument is provided
        if (args.length == 0) {
            System.out.println("Error: Please enter file to be sorted into the argument");
            System.exit(1);
        }

        String fileName = args[0];
        String[] splitFileName = fileName.split("_");
        int fileSize = Integer.parseInt(splitFileName[splitFileName.length - 1].replace(".csv", ""));

        File file = new File(fileName);
        DataRow[] dataList = new DataRow[fileSize];

        try (Scanner scanner = new Scanner(file)) {
            // Reading loop
            for (int i = 0; i < fileSize; i++) {
                String line = scanner.nextLine();
                String[] splitLine = line.split(",");
                DataRow d = new DataRow(Long.parseLong(splitLine[0]), splitLine[1]);
                dataList[i] = d;
            }

            // Log system start time before sort
            long startTime = System.nanoTime();

            // Run quick sort on the data
            quickSort(dataList);

            // Log system end time after sort
            long endTime = System.nanoTime();

            // Write the sorted array into the CSV file
            try {
                FileWriter writer = new FileWriter("quick_sort" + "_" + splitFileName[splitFileName.length - 1]);

                for (DataRow dataRow : dataList) {
                    writer.write(dataRow.numbers + "," + dataRow.characters + "\n");
                }

                writer.close();
            }
            catch (IOException error) {
                System.out.println("An error occured: " + error.getMessage());
            }

            // Calculate the run time of the algorithm
            long runTime = endTime - startTime;

            double durationMillis = runTime / 1000000.0;
            double durationSeconds =  runTime / 1000000000.0;

            // Print the runtime of the algorithm
            System.out.println("Runtime for quick sort on " + fileName + ": " + durationMillis + " ms or " + durationSeconds + " s.");
        }
        catch (FileNotFoundException e) {
            System.out.println("Error: Could not find the file " + args[0]);
            System.exit(1);
        }
    }

    public static void quickSort(DataRow[] dataList) {
        quickSort(dataList, 0, dataList.length - 1);
    }

    public static void quickSort(DataRow[] dataList, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(dataList, low, high);
            quickSort(dataList, low, pivotIndex - 1);
            quickSort(dataList, pivotIndex + 1, high);
        }
    }

    public static int partition(DataRow[] dataList, int low, int high) {
        // Choose pivot
        DataRow pivot = dataList[high];

        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (dataList[j].numbers <= pivot.numbers) {
                i = i + 1;

                // Swap i and j
                DataRow temp = dataList[i];
                dataList[i] = dataList[j];
                dataList[j] = temp;
            }
        }

        // Place pivot in correct position
        DataRow temp = dataList[i + 1];
        dataList[i + 1] = dataList[high];
        dataList[high] = temp;

        return i + 1;
    }
}