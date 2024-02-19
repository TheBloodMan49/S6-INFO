import java.util.ArrayList;
import java.util.List;
import java.util.PriorityQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

public class Solver {
    Board initial;
    List<Board> solution;
    ThreadPoolExecutor executor;

    private static class SearchNode implements Comparable<SearchNode> {
        private Board board;
        private int moves;
        private SearchNode previous;

        public SearchNode(Board board, int moves, SearchNode previous) {
            this.board = new Board(board);
            this.moves = moves;
            this.previous = previous;
        }
        public SearchNode(Board board, int moves) {
            this(board, moves, null);
        }
        @Override
        public int compareTo(SearchNode searchNode) {
            return (board.distance() + moves) - (searchNode.board.distance() + searchNode.moves);
        }
    }

    public Solver(Board initial) {
        this.initial = initial;
        this.solution = new ArrayList<>();
        iterativeDeepening();
    }

    private void aStar() {
        if (!isSolvable()) return;
        PriorityQueue<SearchNode> pq = new PriorityQueue<SearchNode>();
        SearchNode initialNode = new SearchNode(initial, 0);
        pq.add(initialNode);
        while (!pq.isEmpty()) {
            SearchNode node = pq.poll();
            if (node.board.isGoal()) {
                while (node != null) {
                    solution.add(0, node.board);
                    node = node.previous;
                }
                break;
            }
            for (Board neighbor : node.board.neighbors()) {
                if (node.previous == null || !neighbor.equals(node.previous.board)) {
                    pq.add(new SearchNode(neighbor, node.moves + 1, node));
                }
            }
        }
    }

    private void iterativeDeepening() {
        if (!isSolvable()) return;
        executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(8);
        int limit = initial.distance();
        while (solution.isEmpty()) {
            System.out.println("Trying limit: " + limit);
            int finalLimit = limit;
            executor.execute(() -> idaStar(new SearchNode(initial, 0), finalLimit));
            do {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } while (executor.getActiveCount() > 0);
            limit += 1;
        }
    }

    private void idaStar(SearchNode node, int limit) {
        if (node.board.isGoal()) {
            while (node != null) {
                solution.add(0, node.board);
                node = node.previous;
            }
            return;
        }
        if (node.moves + node.board.distance() > limit) return;
        for (Board neighbor : node.board.neighbors()) {
            if (node.previous == null || !neighbor.equals(node.previous.board)) {
                SearchNode finalNode = node;
                executor.execute(() -> idaStar(new SearchNode(neighbor, finalNode.moves + 1, finalNode), limit));
                if (!solution.isEmpty()) return; // found solution
            }
        }
    }

    public boolean isSolvable() {
        /*
        int[] list = new int[initial.dimension() * initial.dimension()];
        int[][] blocks = initial.getBlocks();
        int zeroRow = 0;
        int zeroCol = 0;
        for (int i = 0; i < initial.dimension(); i++) {
            for (int j = 0; j < initial.dimension(); j++) {
                if (blocks[i][j] == 0) {
                    zeroRow = i;
                    zeroCol = j;
                }
                list[i * initial.dimension() + j] = blocks[i][j];
            }
        }
        int inversions = 0;
        for (int i = 0; i < list.length; i++) {
            if (list[i] == 0) continue;
            for (int j = i + 1; j < list.length; j++) {
                if (list[j] == 0) continue;
                if (list[i] > list[j]) inversions++;
            }
        }

        int zeroDistance = (initial.dimension() - 1 - zeroRow) + (initial.dimension() - 1 - zeroCol);

        return (inversions + zeroDistance) % 2 == 0;*/
        return true;
    }

    public int moves() {
	    return solution.size() - 1;
    }

    public Iterable<Board> solution() {
        return solution;
    }

    public static void main(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
}
