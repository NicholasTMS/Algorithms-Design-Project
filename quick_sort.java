import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

class DataRow {
    int numbers;
    String characters;

    DataRow(int num, String chars) {
        numbers = num;
        characters = chars;
    }

    public int getNumbers() {
        return numbers;
    }

    public String getCharacters() {
        return characters;
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

        File file = new File(fileName);
        ArrayList<DataRow> dataList = new ArrayList<DataRow>();

        try (Scanner scanner = new Scanner(file)) {
            // Reading loop
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] splitLine = line.split(",");
                DataRow d = new DataRow(Integer.parseInt(splitLine[0]), splitLine[1]);
                dataList.add(d);
            }

            // Run quick sort on the data
            quickSort(dataList);

            // Write the sorted array into the CSV file
            try {
                String[] splitFileName = fileName.split("_");
                FileWriter writer = new FileWriter("quick_sort" + "_" + splitFileName[2]);

                for (DataRow dataRow : dataList) {
                    writer.write(dataRow.getNumbers() + "," + dataRow.getCharacters() + "\n");
                }

                writer.close();
            }
            catch (IOException error) {
                System.out.println("An error occured: " + error.getMessage());
            }
        }
        catch (FileNotFoundException e) {
            System.out.println("Error: Could not find the file " + args[0]);
            System.exit(1);
        }
    }

    public static void quickSort(ArrayList<DataRow> dataList) {
        quickSort(dataList, 0, dataList.size() - 1);
    }

    public static void quickSort(ArrayList<DataRow> dataList, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(dataList, low, high);
            quickSort(dataList, low, pivotIndex - 1);
            quickSort(dataList, pivotIndex + 1, high);
        }
    }

    public static int partition(ArrayList<DataRow> dataList, int low, int high) {
        // Choose pivot
        DataRow pivot = dataList.get(high);

        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (dataList.get(j).getNumbers() <= pivot.getNumbers()) {
                i = i + 1;

                // Swap i and j
                DataRow temp = dataList.get(i);
                dataList.set(i, dataList.get(j));
                dataList.set(j, temp);
            }
        }

        // Place pivot in correct position
        DataRow temp = dataList.get(i + 1);
        dataList.set(i + 1, dataList.get(high));
        dataList.set(high, temp);

        return i + 1;
    }
}

