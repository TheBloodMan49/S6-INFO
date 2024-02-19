import java.util.ArrayList;
import java.util.Arrays;

public class Board {
    private final int N;
    private final int[][] blocks;

    public Board(int[][] blocks) {
        this.N = blocks.length;
        this.blocks = new int[N][N];
        for (int i = 0; i < N; i++) {
            this.blocks[i] = Arrays.copyOf(blocks[i], N);
        }
    }
    public Board(Board board) {
        this(board.blocks);
    }

    public int dimension() {
        return N;
    }

    public int[][] getBlocks() {
        return blocks;
    }

    public int hamming() {
        int sum = 0;
        int expected = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int block = blocks[i][j];
                if (block != expected && block != 0) {
                    sum++;
                }
                expected++;
            }
        }
        return sum;
    }

    public int manhattan() {
        int sum = 0;
        int expected = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int block = blocks[i][j];
                if (block != expected && block != 0) {
                    int rowexpected = (block - 1) / N;
                    int colexpected = (block - 1) % N;
                    sum += Math.abs(rowexpected - i) + Math.abs(colexpected - j);
                }
                expected++;
            }
        }
        return sum;
    }

    public int manhattanplus() {
        return manhattan() + hamming();
    }

    public int distance() {
        return manhattanplus();
    }

    public boolean isGoal() {
        return hamming() == 0;
    }

    public Board twin() {
        int[][] newBlocks = copyBlocks();
        if (newBlocks[0][0] != 0) {
            if (newBlocks[0][1] != 0) {
                // Swap positions of the first two blocks in the first row
                int temp = newBlocks[0][0];
                newBlocks[0][0] = newBlocks[0][1];
                newBlocks[0][1] = temp;
            } else {
                // Swap positions of the first two blocks in the first column
                int temp = newBlocks[0][0];
                newBlocks[0][0] = newBlocks[1][0];
                newBlocks[1][0] = temp;
            }
        } else {
            // Swap positions of the second and third blocks in the first row
            int temp = newBlocks[0][1];
            newBlocks[0][1] = newBlocks[0][2];
            newBlocks[0][2] = temp;
        }
        return new Board(newBlocks);
    }

    public boolean equals(Object that) {
        if (!(that instanceof Board)) return false;
        Board thatB = (Board) that;
        if (thatB.N != this.N) return false;

        for (int i = 0; i < N; i++) {
            if (!Arrays.equals(this.blocks[i], thatB.blocks[i])) {
                return false;
            }
        }
        return true;
    }

    public Iterable<Board> neighbors() {
        int zerox = 0;
        int zeroy = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (blocks[i][j] == 0) {
                    zerox = i;
                    zeroy = j;
                }
            }
        }

        ArrayList<Board> neighbors = new ArrayList<>();
        if (zerox > 0) {
            int[][] newBlocks = copyBlocks();
            newBlocks[zerox][zeroy] = newBlocks[zerox - 1][zeroy];
            newBlocks[zerox - 1][zeroy] = 0;
            neighbors.add(new Board(newBlocks));
        }
        if (zerox < N - 1) {
            int[][] newBlocks = copyBlocks();
            newBlocks[zerox][zeroy] = newBlocks[zerox + 1][zeroy];
            newBlocks[zerox + 1][zeroy] = 0;
            neighbors.add(new Board(newBlocks));
        }
        if (zeroy > 0) {
            int[][] newBlocks = copyBlocks();
            newBlocks[zerox][zeroy] = newBlocks[zerox][zeroy - 1];
            newBlocks[zerox][zeroy - 1] = 0;
            neighbors.add(new Board(newBlocks));
        }
        if (zeroy < N - 1) {
            int[][] newBlocks = copyBlocks();
            newBlocks[zerox][zeroy] = newBlocks[zerox][zeroy + 1];
            newBlocks[zerox][zeroy + 1] = 0;
            neighbors.add(new Board(newBlocks));
        }

        return neighbors;
    }

    private int[][] copyBlocks() {
        int[][] newBlocks = new int[N][N];
        for (int i = 0; i < N; i++) {
            newBlocks[i] = Arrays.copyOf(blocks[i], N);
        }
        return newBlocks;
    }

    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(N + "\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                s.append(String.format("%2d ", blocks[i][j]));
            }
            s.append("\n");
        }
        return s.toString();
    }
}
