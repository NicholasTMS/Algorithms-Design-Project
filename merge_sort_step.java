import java.io.*;
import java.util.*;

class merge_sort_step {
    static class DataPair implements Comparable<DataPair> {
        final long number;
        final String text;
        final int lineNumber;

        DataPair(long number, String text, int lineNumber) {
            this.number = number;
            this.text = text;
            this.lineNumber = lineNumber;
        }

        @Override
        public String toString() {
            return lineNumber + ": " + number + "," + text;
        }

        @Override
        public int compareTo(DataPair other) {
            return Long.compare(this.number, other.number);
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter path to CSV file: ");
        String filePath = scanner.nextLine();

        System.out.print("Show sorting steps? (y/n): ");
        boolean showSteps = scanner.nextLine().equalsIgnoreCase("y");

        int startLine = 0;
        int endLine = Integer.MAX_VALUE;

        if (showSteps) {
            System.out.print("Enter starting line number (1-based): ");
            startLine = scanner.nextInt();
            System.out.print("Enter ending line number: ");
            endLine = scanner.nextInt();
            scanner.nextLine(); // Consume newline
        }

        try {
            // Read and process the file
            long readStart = System.nanoTime();
            List<DataPair> records = readCSV(filePath, startLine, endLine);
            long readEnd = System.nanoTime();

            if (records.isEmpty()) {
                System.out.println("No data found in the specified range.");
                return;
            }

            DataPair[] arr = records.toArray(new DataPair[0]);

            // Display original data
            if (showSteps) {
                System.out.println("\nOriginal Data (Lines " + startLine + "-" + endLine + "):");
                printRecords(arr);
            }

            // Perform sorting with metrics
            ArrayList<String> steps = new ArrayList<>();
            long sortStart = System.nanoTime();
            sort(arr, 0, arr.length - 1, showSteps ? steps : null);
            long sortEnd = System.nanoTime();

            // Display results
            if (showSteps) {
                System.out.println("\nSorting Steps:");
                for (int i = 0; i < steps.size(); i++) {
                    System.out.println("Step " + (i + 1) + ":");
                    System.out.println(steps.get(i));
                }
            }

            System.out.println("\nFinal Sorted Data:");
            printRecords(arr);

            // Performance metrics
            System.out.println("\nPerformance Metrics:");
            System.out.println("--------------------------------");
            System.out.printf("File Reading: %,d ns (%.3f ms)\n",
                    (readEnd - readStart), (readEnd - readStart) / 1_000_000.0);
            System.out.printf("Merge Sort:    %,d ns (%.3f ms)\n",
                    (sortEnd - sortStart), (sortEnd - sortStart) / 1_000_000.0);
            System.out.println("--------------------------------");
            System.out.printf("Records processed: %,d\n", arr.length);
            System.out.println("--------------------------------");

            // Write output to file
            writeOutputToFile(arr, steps, filePath, startLine, endLine);

        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        } finally {
            scanner.close();
        }
    }

    private static List<DataPair> readCSV(String filePath, int startLine, int endLine) throws IOException {
        List<DataPair> records = new ArrayList<>();
        int currentLine = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = br.readLine()) != null) {
                currentLine++;
                if (currentLine >= startLine && currentLine <= endLine) {
                    String[] parts = line.split(",", 2);
                    if (parts.length >= 2) {
                        try {
                            long num = Long.parseLong(parts[0].trim());
                            records.add(new DataPair(num, parts[1].trim(), currentLine));
                        } catch (NumberFormatException e) {
                            System.err.println("Skipping invalid number at line " + currentLine);
                        }
                    }
                } else if (currentLine > endLine) {
                    break;
                }
            }
        }
        return records;
    }

    private static void sort(DataPair[] arr, int left, int right, ArrayList<String> steps) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            sort(arr, left, mid, steps);
            sort(arr, mid + 1, right, steps);
            merge(arr, left, mid, right, steps);
        }
    }

    private static void merge(DataPair[] arr, int left, int mid, int right, ArrayList<String> steps) {
        DataPair[] leftArr = Arrays.copyOfRange(arr, left, mid + 1);
        DataPair[] rightArr = Arrays.copyOfRange(arr, mid + 1, right + 1);

        int i = 0, j = 0, k = left;
        while (i < leftArr.length && j < rightArr.length) {
            if (leftArr[i].compareTo(rightArr[j]) <= 0) {
                arr[k++] = leftArr[i++];
            } else {
                arr[k++] = rightArr[j++];
            }
        }

        while (i < leftArr.length) {
            arr[k++] = leftArr[i++];
        }

        while (j < rightArr.length) {
            arr[k++] = rightArr[j++];
        }

        if (steps != null) {
            steps.add("Merged " + left + "-" + right + ":\n" + arrayToString(arr, left, right));
        }
    }

    private static void printRecords(DataPair[] arr) {
        System.out.println(arrayToString(arr, 0, arr.length - 1));
    }

    private static String arrayToString(DataPair[] arr, int start, int end) {
        StringBuilder sb = new StringBuilder();
        for (int i = start; i <= end; i++) {
            sb.append(arr[i]).append("\n");
        }
        return sb.toString();
    }

    private static void writeOutputToFile(DataPair[] arr, ArrayList<String> steps,
                                          String inputPath, int startLine, int endLine) throws IOException {
        String outputFile = "merge_sort_results_" + new File(inputPath).getName();
        try (PrintWriter writer = new PrintWriter(outputFile)) {
            writer.println("Merge Sort Results");
            writer.println("Input file: " + inputPath);
            writer.println("Lines processed: " + startLine + "-" + endLine);
            writer.println("\nFinal Sorted Data:");
            writer.println(arrayToString(arr, 0, arr.length - 1));

            if (steps != null) {
                writer.println("\nSorting Steps:");
                for (int i = 0; i < steps.size(); i++) {
                    writer.println("Step " + (i + 1) + ":");
                    writer.println(steps.get(i));
                }
            }
        }
        System.out.println("\nResults written to: " + outputFile);
    }
}