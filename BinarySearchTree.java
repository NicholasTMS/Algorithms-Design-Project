import java.io.*;
import java.util.*;

public class BinarySearchTree {

    static class DataNode {
        final long key;
        final String value;

        DataNode(long key, String value) {
            this.key = key;
            this.value = value;
        }

        @Override
        public String toString() {
            return key + "/" + value;
        }
    }

    static class TreeNode {
        DataNode data;
        TreeNode left, right;

        TreeNode(DataNode data) {
            this.data = data;
        }
    }

    static class SearchResult {
        final List<String> searchPath = new ArrayList<>();
        DataNode result;
        long searchTimeMs;
        boolean found;

        void addStep(TreeNode node) {
            searchPath.add(node != null ? node.data.toString() : "null");
        }
    }

    private TreeNode root;

    public void buildBalancedTree(DataNode[] sortedData) {
        root = buildBalancedTree(sortedData, 0, sortedData.length - 1);
    }

    private TreeNode buildBalancedTree(DataNode[] data, int start, int end) {
        if (start > end) return null;

        int mid = start + (end - start) / 2;
        TreeNode node = new TreeNode(data[mid]);
        node.left = buildBalancedTree(data, start, mid - 1);
        node.right = buildBalancedTree(data, mid + 1, end);
        return node;
    }

    public SearchResult search(long key) {
        SearchResult result = new SearchResult();
        long startTime = System.currentTimeMillis();
        result.found = search(root, key, result);
        result.searchTimeMs = System.currentTimeMillis() - startTime;
        return result;
    }

    private boolean search(TreeNode node, long key, SearchResult result) {
        if (node == null) {
            result.addStep(null);
            return false;
        }

        result.addStep(node);

        if (key == node.data.key) {
            result.result = node.data;
            return true;
        }
        return key < node.data.key
                ? search(node.left, key, result)
                : search(node.right, key, result);
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Binary Search Tree Implementation\n");

        // File input
        System.out.print("Enter path to sorted CSV file: ");
        String inputPath = scanner.nextLine().trim();

        List<DataNode> data = loadData(inputPath);
        if (data.isEmpty()) {
            System.out.println("No valid data loaded. Exiting.");
            return;
        }

        // Build tree
        BinarySearchTree bst = new BinarySearchTree();
        bst.buildBalancedTree(data.toArray(new DataNode[0]));
        System.out.println("Balanced BST built with " + data.size() + " nodes.");

        // Search loop
        while (true) {
            System.out.print("\nEnter key to search (or 'quit' to exit): ");
            String input = scanner.nextLine().trim();

            if (input.equalsIgnoreCase("quit")) {
                break;
            }

            try {
                long key = Long.parseLong(input);
                SearchResult result = bst.search(key);

                System.out.println("\nSearch " + (result.found ? "SUCCESSFUL" : "FAILED"));
                if (result.found) {
                    System.out.println("Match: " + result.result);
                }

                System.out.println("\nSearch path (" + result.searchPath.size() + " steps):");
                for (int i = 0; i < result.searchPath.size(); i++) {
                    System.out.printf("%2d: %s%n", i+1, result.searchPath.get(i));
                }

                System.out.printf("%nSearch time: %d ms%n", result.searchTimeMs);

                // Option to save results
                System.out.print("\nSave results to file? (y/n): ");
                if (scanner.nextLine().trim().equalsIgnoreCase("y")) {
                    saveResults(result, key);
                }

            } catch (NumberFormatException e) {
                System.out.println("Invalid number format. Please try again.");
            }
        }
        scanner.close();
    }

    static List<DataNode> loadData(String filePath) {
        List<DataNode> data = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split("/", 2);
                if (parts.length == 2) {
                    try {
                        data.add(new DataNode(Long.parseLong(parts[0]), parts[1]));
                    } catch (NumberFormatException e) {
                        System.out.println("Skipping invalid line: " + parts[0]);
                    }
                }
            }
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
        return data;
    }

    private static void saveResults(SearchResult result, long key) {
        String fileName = "bst_search_" + key + ".txt";
        try (PrintWriter writer = new PrintWriter(fileName)) {
            writer.println("Binary Search Tree Results");
            writer.println("Key searched: " + key);
            writer.println("Status: " + (result.found ? "FOUND" : "NOT FOUND"));

            if (result.found) {
                writer.println("Match: " + result.result);
            }

            writer.println("\nSearch Path:");
            for (int i = 0; i < result.searchPath.size(); i++) {
                writer.printf("%2d: %s%n", i+1, result.searchPath.get(i));
            }

            writer.printf("%nSearch time: %d ms%n", result.searchTimeMs);
            System.out.println("Results saved to " + fileName);
        } catch (FileNotFoundException e) {
            System.out.println("Error saving results: " + e.getMessage());
        }
    }
}