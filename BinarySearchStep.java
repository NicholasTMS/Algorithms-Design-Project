import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class BinarySearchStep {

    private static boolean showSortingSteps = false;
    private static int startRow = 0;
    private static int endRow = Integer.MAX_VALUE;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the path to your CSV file: ");
        String filePath = scanner.nextLine();

        System.out.print("Show sorting steps? (y/n): ");
        showSortingSteps = scanner.nextLine().equalsIgnoreCase("y");

        if (showSortingSteps) {
            System.out.print("Enter start row for showing steps (0-based): ");
            startRow = Integer.parseInt(scanner.nextLine());
            System.out.print("Enter end row for showing steps: ");
            endRow = Integer.parseInt(scanner.nextLine());
        }

        System.out.print("Enter the ID to search for: ");
        String targetId = scanner.nextLine();

        try {
            long readStart = System.nanoTime();
            List<Record> records = readCSV(filePath);
            long readEnd = System.nanoTime();

            long searchStart = System.nanoTime();
            Record result = binarySearch(records, targetId);
            long searchEnd = System.nanoTime();

            if (result != null) {
                System.out.println("\nSearch Result:");
                System.out.println("--------------------------------");
                System.out.println("Found at line number: " + result.lineNumber);
                System.out.println("ID: " + result.id);
                System.out.println("Value: " + result.value);
                System.out.println("Full record: " + result);
                System.out.println("--------------------------------");
            } else {
                System.out.println("\nID " + targetId + " not found in the file.");
            }

            // Performance metrics
            System.out.println("\nPerformance Metrics:");
            System.out.println("--------------------------------");
            System.out.printf("File Reading: %,d ns (%.3f ms)\n",
                    (readEnd - readStart), (readEnd - readStart) / 1_000_000.0);
            System.out.printf("Binary Search: %,d ns (%.6f ms)\n",
                    (searchEnd - searchStart), (searchEnd - searchStart) / 1_000_000.0);
            System.out.println("--------------------------------");

        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        } finally {
            scanner.close();
        }
    }

    static class Record implements Comparable<Record> {
        final String id;
        final String value;
        final int lineNumber;

        Record(String id, String value, int lineNumber) {
            this.id = id;
            this.value = value;
            this.lineNumber = lineNumber;
        }

        @Override
        public int compareTo(Record other) {
            return this.id.compareTo(other.id);
        }

        @Override
        public String toString() {
            return lineNumber + ": " + id + "," + value;
        }
    }

    private static List<Record> readCSV(String filePath) throws IOException {
        List<Record> records = new ArrayList<>();
        int lineNumber = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = br.readLine()) != null) {
                lineNumber++;
                String[] parts = line.split(",", 2);
                if (parts.length == 2) {
                    records.add(new Record(parts[0].trim(), parts[1].trim(), lineNumber));
                }
            }
        }

        if (showSortingSteps) {
            System.out.println("\nOriginal Data (Lines " + startRow + " to " + endRow + "):");
            printRecordsInRange(records, startRow, endRow);
        }

        // Enhanced sorting validation with step display
        for (int i = 1; i < records.size(); i++) {
            Record prev = records.get(i-1);
            Record current = records.get(i);

            if (prev.compareTo(current) > 0) {
                if (showSortingSteps && i >= startRow && i <= endRow) {
                    System.out.println("\nSorting Violation Found:");
                    System.out.println("  " + prev);
                    System.out.println("  " + current);
                }
                throw new IOException(String.format(
                        "Sorting violation detected:\n" +
                                "  Line %d: %s comes after\n" +
                                "  Line %d: %s\n" +
                                "File must be sorted in ascending order by ID.",
                        prev.lineNumber, prev.id,
                        current.lineNumber, current.id
                ));
            }

            if (showSortingSteps && i >= startRow && i <= endRow) {
                System.out.println("\nStep " + i + ": Checking order between:");
                System.out.println("  " + prev);
                System.out.println("  " + current);
                System.out.println("Result: " +
                        (prev.compareTo(current) <= 0 ? "In order" : "OUT OF ORDER"));
            }
        }

        if (showSortingSteps) {
            System.out.println("\nFinal Verified Order (Lines " + startRow + " to " + endRow + "):");
            printRecordsInRange(records, startRow, endRow);
        }

        return records;
    }

    private static void printRecordsInRange(List<Record> records, int start, int end) {
        for (int i = Math.max(0, start); i <= Math.min(end, records.size()-1); i++) {
            System.out.println(records.get(i));
        }
    }

    private static Record binarySearch(List<Record> records, String targetId) {
        int left = 0;
        int right = records.size() - 1;
        int step = 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            Record current = records.get(mid);
            int comparison = current.id.compareTo(targetId);

            if (showSortingSteps && mid >= startRow && mid <= endRow) {
                System.out.println("\nBinary Search Step " + step++ + ":");
                System.out.println("  Searching between indices " + left + " and " + right);
                System.out.println("  Middle index: " + mid);
                System.out.println("  Current line: " + current.lineNumber);
                System.out.println("  Record: " + current.id + "," + current.value);
                System.out.println("  Comparison result: " +
                        (comparison == 0 ? "MATCH" :
                                comparison < 0 ? "GO RIGHT" : "GO LEFT"));
            }

            if (comparison == 0) {
                return current;
            } else if (comparison < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return null;
    }
}