
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

public class BoardTest {
    private Board goalBoard;
    private Board board1;
    private Board board2;
    private Board board3;

    @Before
    public void initialize() {
        goalBoard = new Board(new int[][]{{1, 2, 3}, {4, 5, 6}, {7, 8, 0}});
        board1 = new Board(new int[][]{{8, 1, 3}, {4, 0, 2}, {7, 6, 5}});
        board2 = new Board(new int[][]{{2, 5, 6}, {4, 1, 3}, {0, 8, 7}});
        board3 = new Board(new int[][]{{8, 7, 6}, {5, 1, 3}, {4, 2, 0}});
    }
    @Test
    public void hamming() {
        assertEquals(goalBoard.hamming(), 0);
        assertEquals(board1.hamming(), 5);
        assertEquals(board2.hamming(), 6);
        assertEquals(board3.hamming(), 8);
    }
    @Test
    public void manhattan() {
        assertEquals(goalBoard.manhattan(), 0);
        assertEquals(board1.manhattan(), 10);
        assertEquals(board2.manhattan(), 8);
        assertEquals(board3.manhattan(), 14);
    }
    @Test
    public void isGoal() {
        assertTrue(goalBoard.isGoal());
        assertFalse(board1.isGoal());
    }
    @Test
    public void twin() {
        List<Board> goalBoardTwins =
                Arrays.asList(
			      new Board(new int[][]{{2, 1, 3}, {4, 5, 6}, {7, 8, 0}}), 
			      new Board(new int[][]{{1, 3, 2}, {4, 5, 6}, {7, 8, 0}}), 
			      new Board(new int[][]{{1, 2, 3}, {5, 4, 6}, {7, 8, 0}}), 
			      new Board(new int[][]{{1, 2, 3}, {4, 6, 5}, {7, 8, 0}}),
			      new Board(new int[][]{{1, 2, 3}, {4, 5, 6}, {8, 7, 0}})
			      );
        assertTrue(goalBoardTwins.contains(goalBoard.twin()));
    }
    @Test
    public void equality() {
        assertTrue(goalBoard.equals(new Board(new int[][]{{1, 2, 3}, {4, 5, 6}, {7, 8, 0}})));
        assertFalse(goalBoard.equals(new Board(new int[][]{{1, 2, 3}, {5, 4, 6}, {7, 8, 0}})));
        assertFalse(new Board(new int[][]{{1, 2, 3}, {5, 4, 6}, {7, 8, 0}}).equals(new Board(new int[][]{{1, 2, 3, -1}, {5, 4, 6, -1},
                        {7, 8, 0, -1}, {-1, -1, -1, -1}})));
    }
    @Test
    public void neighbors() {
        List<Board> board1Neighbors =
                Arrays.asList(
			      new Board(new int[][]{{8, 1, 3}, {0, 4, 2}, {7, 6, 5}}), 
			      new Board(new int[][]{{8, 1, 3}, {4, 2, 0}, {7, 6, 5}}), 
			      new Board(new int[][]{{8, 0, 3}, {4, 1, 2}, {7, 6, 5}}), 
			      new Board(new int[][]{{8, 1, 3}, {4, 6, 2}, {7, 0, 5}})
			      );
        List<Board> neighbors = new ArrayList<Board>();
        for (Board board : board1.neighbors()) {
            neighbors.add(board);
        }
        assertTrue(neighbors.containsAll(board1Neighbors));
        assertTrue(board1Neighbors.containsAll(neighbors));
    }
}
