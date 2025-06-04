import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;

class DataRow {
    int numbers;
    String characters;

    DataRow(int num, String chars) {
        numbers = num;
        characters = chars;
    }
}

public class quick_sort {
    // Main program
    public static void main(String[] args) {
        File file = new File("dataset_sample_1000.csv");
        ArrayList<DataRow> dataList = new ArrayList<DataRow>();

        Scanner scanner = null;
        try {
            scanner = new Scanner(file);

            // Reading loop
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] splitLine = line.split(",");
                DataRow d = new DataRow(Integer.parseInt(splitLine[0]), splitLine[1]);
            }
        }
        catch (FileNotFoundException e) {
            System.out.println("Error: Could not find the file");
            System.exit(1);
        }
    }

    // Quick sort implementation
    quick_sort() {

    }
}

