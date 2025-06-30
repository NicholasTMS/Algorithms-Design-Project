import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class BinarySearch {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter path to sorted CSV file: ");
        String filePath = scanner.nextLine().trim();

        try {
            // Read and validate the sorted CSV file
            List<Record> records = readCSV(filePath);
            System.out.println("\nLoaded " + records.size() + " records from: " + filePath);

            // Benchmark search performance
            benchmarkSearch(records);

        } catch (IOException e) {
            System.err.println("\nError: " + e.getMessage());
        } finally {
            scanner.close();
        }
    }

    static class Record implements Comparable<Record> {
        final String id;
        final String value;

        Record(String id, String value) {
            this.id = id;
            this.value = value;
        }

        @Override
        public int compareTo(Record other) {
            return this.id.compareTo(other.id);
        }
    }

    private static List<Record> readCSV(String filePath) throws IOException {
        List<Record> records = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",", 2);
                if (parts.length == 2) {
                    records.add(new Record(parts[0].trim(), parts[1].trim()));
                }
            }
        }

        // Verify the records are sorted by ID
        for (int i = 1; i < records.size(); i++) {
            if (records.get(i-1).compareTo(records.get(i)) > 0) {
                throw new IOException("CSV is not sorted by ID. Problem at line " + (i+1) +
                        "\nFound: " + records.get(i-1).id +
                        " before " + records.get(i).id);
            }
        }

        return records;
    }

    private static Record binarySearch(List<Record> records, String targetId) {
        int left = 0;
        int right = records.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            Record current = records.get(mid);
            int comparison = current.id.compareTo(targetId);

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

    private static void benchmarkSearch(List<Record> records) {
        long minTime = Long.MAX_VALUE;
        long maxTime = Long.MIN_VALUE;
        long totalTime = 0;
        int totalSearches = records.size();

        System.out.println("\nRunning benchmark (" + totalSearches + " searches)...");

        // Warm up the JVM
        System.out.print("Warming up...");
        for (int i = 0; i < Math.min(1000, records.size()); i++) {
            binarySearch(records, records.get(i).id);
        }
        System.out.println(" done.");

        // Main benchmarking
        for (Record record : records) {
            long startTime = System.nanoTime();
            Record found = binarySearch(records, record.id);
            long endTime = System.nanoTime();
            long duration = endTime - startTime;

            if (found == null || !found.id.equals(record.id)) {
                System.err.println("Search failed for ID: " + record.id);
                continue;
            }

            minTime = Math.min(minTime, duration);
            maxTime = Math.max(maxTime, duration);
            totalTime += duration;
        }

        // Calculate statistics
        double avgTime = (double) totalTime / totalSearches;
        double minMs = minTime / 1_000_000.0;
        double maxMs = maxTime / 1_000_000.0;
        double avgMs = avgTime / 1_000_000.0;

        // Display performance metrics
        System.out.println("\nPerformance Metrics:");
        System.out.println("--------------------------------");
        System.out.printf("Dataset size:    %,d records\n", totalSearches);
        System.out.printf("Best case:       %,d ns (%.6f ms)\n", minTime, minMs);
        System.out.printf("Worst case:      %,d ns (%.6f ms)\n", maxTime, maxMs);
        System.out.printf("Average:         %,.0f ns (%.6f ms)\n", avgTime, avgMs);
        System.out.println("--------------------------------");
        System.out.printf("Total time:      %,d ns (%.3f ms)\n", totalTime, totalTime / 1_000_000.0);
        System.out.printf("Throughput:      %,.1f searches/ms\n", totalSearches / (totalTime / 1_000_000.0));
        System.out.println("--------------------------------");
    }
}