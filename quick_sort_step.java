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

public class quick_sort_step {
    private static StringBuilder sortingSteps = new StringBuilder();

    // Main program
    public static void main(String[] args) {
        // Check that argument is provided
        if (args.length < 3) {
            System.out.println("Error: Please enter the .csv file to be sorted, start row and end row.");
            System.exit(1);
        }

        // Parse the arguments
        String fileName = args[0];
        int startRow = Integer.parseInt(args[1]);
        int endRow = Integer.parseInt(args[2]);

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

            // Create sublist of start row to end row
            ArrayList<DataRow> subsetDataList = new ArrayList<>(dataList.subList(startRow, endRow + 1));

            sortingSteps.setLength(0);
            sortingSteps.append(dataRowListToString(subsetDataList)).append("\n");

            // Run quick sort on the sublist
            quickSort(subsetDataList);

            // Copy the sorted subset back to the main list
            for (int i = 0; i < subsetDataList.size(); i++) {
                dataList.set(startRow + i, subsetDataList.get(i));
            }

            // Write into .txt file
            String txtFileName = "quick_sort_step_" + startRow + "_" + endRow + ".txt";
            try (PrintWriter writer = new PrintWriter(new FileWriter(txtFileName))) {
                writer.print(sortingSteps.toString());
                System.out.println("Sorting steps written to " + txtFileName);
            } catch (IOException e) {
                System.out.println("Error writing to file: " + e.getMessage());
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

            sortingSteps.append("pi: ").append(pivotIndex);
            sortingSteps.append(" ").append(dataRowListToString(dataList)).append("\n");

            quickSort(dataList, low, pivotIndex - 1);
            quickSort(dataList, pivotIndex + 1, high);
        }
    }

    public static int partition(ArrayList<DataRow> dataList, int low, int high) {
        // Choose pivot
        DataRow pivot = dataList.get(high);

        int i = low - 1;

        // Create before partition view
        ArrayList<DataRow> before = new ArrayList<>(dataList);

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

        // Create left and right partitions for clarity
        ArrayList<DataRow> leftPart = new ArrayList<>();
        for (int k = low; k <= i; k++) {
            leftPart.add(dataList.get(k));
        }

        ArrayList<DataRow> rightPart = new ArrayList<>();
        for (int k = i + 2; k <= high; k++) {
            rightPart.add(dataList.get(k));
        }

        return i + 1;
    }

    private static String dataRowListToString(ArrayList<DataRow> list) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < list.size(); i++) {
            if (i > 0) sb.append(", ");
            sb.append(list.get(i).getNumbers());
        }
        sb.append("]");
        return sb.toString();
    }
}

