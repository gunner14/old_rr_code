package com.renren.ad.sns.services.uranus.prg;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertNotNull;

import java.util.Arrays;
import java.util.List;

import org.junit.Test;

/**
 * 
 * @author xiang.zhang
 * last modified: 2012/06/05
 */
public class RankerTest {

    private int[] actual = null;

    private int[] expected = null;

    private int[] toIntArray(List<Integer> raw) {
        if (raw != null) {
            int[] result = new int[raw.size()];
            for (int i = 0; i < raw.size(); i++) {
                result[i] = raw.get(i);
            }
            return result;
        }
        return null;
    }

    Object[] testSeeds = new Object[] {
            // already in ascending order
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 0, new int[0] },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 1, new int[] { 0 } },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 3, new int[] { 0, 1, 2 } },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 5, new int[] { 0, 1, 2, 3, 4 } },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 6, new int[] { 0, 1, 2, 3, 4 } },
            // already in descending order
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 0, new int[0] },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 1, new int[] { 4 } },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 3, new int[] { 4, 3, 2 } },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 5, new int[] { 4, 3, 2, 1, 0 } },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 6, new int[] { 4, 3, 2, 1, 0 } },
            // common 
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0), 0, new int[0] },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0), 1, new int[] { 0 } },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0), 3, new int[] { 0, 2, 1 } },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0), 5, new int[] { 0, 2, 1, 4, 3 } },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0), 6, new int[] { 0, 2, 1, 4, 3 } }, };

    @Test
    public void testRankListOfDoubleInt() {
        for (Object seed : testSeeds) {
            Object[] items = (Object[]) seed;
            actual = toIntArray(Ranker.rank((List<Double>) items[0], (Integer) items[1]));
            expected = (int[]) items[2];
            assertNotNull(actual);
            assertArrayEquals(expected, actual);
        }
    }

    Object[] testSeeds2 = new Object[] {
            // already in ascending order
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0),
                    Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 0, new int[0] },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0),
                    Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 1, new int[] { 4 } },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0),
                    Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 3, new int[] { 4, 3, 2 } },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0),
                    Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 5, new int[] { 4, 3, 2, 1, 0 } },
            new Object[] { Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0),
                    Arrays.asList(1.0, 2.0, 3.0, 4.0, 5.0), 6, new int[] { 4, 3, 2, 1, 0 } },
            // already in descending order
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0),
                    Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 0, new int[0] },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0),
                    Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 1, new int[] { 0 } },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0),
                    Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 3, new int[] { 0, 1, 2 } },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0),
                    Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 5, new int[] { 0, 1, 2, 3, 4 } },
            new Object[] { Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0),
                    Arrays.asList(5.0, 4.0, 3.0, 2.0, 1.0), 6, new int[] { 0, 1, 2, 3, 4 } },
            // common 
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0),
                    Arrays.asList(1.0, 1.0, 1.0, 1.0, 1.0), 0, new int[0] },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0),
                    Arrays.asList(1.0, 1.0, 1.0, 1.0, 1.0), 1, new int[] { 3 } },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0),
                    Arrays.asList(1.0, 1.0, 1.0, 1.0, 1.0), 3, new int[] { 3, 4, 1 } },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0),
                    Arrays.asList(1.0, 1.0, 1.0, 1.0, 1.0), 5, new int[] { 3, 4, 1, 2, 0 } },
            new Object[] { Arrays.asList(1.0, 3.0, 2.0, 5.0, 4.0),
                    Arrays.asList(1.0, 1.0, 1.0, 1.0, 1.0), 10, new int[] { 3, 4, 1, 2, 0 } }, };

    @Test
    public void testRankListOfDoubleListOfDoubleInt() {
        for (Object seed : testSeeds2) {
            Object[] items = (Object[]) seed;
            actual = toIntArray(Ranker.rank((List<Double>) items[0], (List<Double>) items[1],
                    (Integer) items[2]));
            expected = (int[]) items[3];
            assertNotNull(actual);
            assertArrayEquals(expected, actual);
        }
    }

}
