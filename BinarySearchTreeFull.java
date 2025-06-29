import java.io.*;
import java.util.*;

public class BinarySearchTreeFull extends BinarySearchTree {

    static class TimeAnalysis {
        long bestTime = Long.MAX_VALUE;
        long worstTime = Long.MIN_VALUE;
        long totalTime = 0;
        int searchCount = 0;

        void update(long time) {
            bestTime = Math.min(bestTime, time);
            worstTime = Math.max(worstTime, time);
            totalTime += time;
            searchCount++;
        }

        long getAverageTime() {
            return searchCount > 0 ? totalTime / searchCount : 0;
        }
    }

    public TimeAnalysis analyzeAllSearches(DataNode[] data) {
        TimeAnalysis analysis = new TimeAnalysis();

        for (DataNode node : data) {
            SearchResult result = search(node.key);
            analysis.update(result.searchTimeMs);
        }

        return analysis;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Binary Search Tree Full Analysis\n");

        // File input
        System.out.print("Enter path to sorted CSV file: ");
        String inputPath = scanner.nextLine().trim();

        List<DataNode> data = loadData(inputPath);
        if (data.isEmpty()) {
            System.out.println("No valid data loaded. Exiting.");
            return;
        }

        // Build tree
        BinarySearchTreeFull bst = new BinarySearchTreeFull();
        DataNode[] dataArray = data.toArray(new DataNode[0]);
        bst.buildBalancedTree(dataArray);
        System.out.println("Balanced BST built with " + data.size() + " nodes.");

        // Perform full analysis
        System.out.println("\nRunning comprehensive search analysis...");
        TimeAnalysis analysis = bst.analyzeAllSearches(dataArray);

        System.out.println("\nTime Complexity Analysis Results:");
        System.out.println("Best case time:    " + analysis.bestTime + " ms");
        System.out.println("Average case time: " + analysis.getAverageTime() + " ms");
        System.out.println("Worst case time:   " + analysis.worstTime + " ms");
        System.out.println("Total searches:    " + analysis.searchCount);

        // Save results
        System.out.print("\nSave results to file? (y/n): ");
        if (scanner.nextLine().trim().equalsIgnoreCase("y")) {
            saveAnalysisResults(analysis, dataArray.length);
        }

        scanner.close();
    }

    private static void saveAnalysisResults(TimeAnalysis analysis, int nodeCount) {
        String fileName = "bst_full_analysis.txt";
        try (PrintWriter writer = new PrintWriter(fileName)) {
            writer.println("Binary Search Tree Full Analysis");
            writer.println("Nodes in tree: " + nodeCount);
            writer.println("Total searches: " + analysis.searchCount);
            writer.println("\nTime Complexity Analysis:");
            writer.println("Best case time:    " + analysis.bestTime + " ms");
            writer.println("Average case time: " + analysis.getAverageTime() + " ms");
            writer.println("Worst case time:   " + analysis.worstTime + " ms");

            writer.println("\nTheoretical Complexities:");
            writer.println("Best case:   O(1) - Found at root");
            writer.println("Average case: O(log n) - Balanced tree");
            writer.println("Worst case:  O(log n) - Deepest leaf in balanced tree");

            System.out.println("Analysis saved to " + fileName);
        } catch (FileNotFoundException e) {
            System.out.println("Error saving results: " + e.getMessage());
        }
    }
}