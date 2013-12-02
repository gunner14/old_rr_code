package com.renren.ad.sns.services.uranus;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;

import java.util.Arrays;

import org.junit.Test;

/**
 * 
 * @author xiang.zhang last modified: 2012/06/05
 */
public class CandidateListTest {

    @Test
    public void testAddCandidate() {
        CandidateList cl = new CandidateList(2);
        cl.add(new Candidate(0, 1.0));
        cl.add(new Candidate(0, 3.0));
        cl.add(new Candidate(0, 4.0));
        cl.add(new Candidate(0, 2.0));
        assertEquals(2, cl.size());
        assertEquals(new Candidate(0, 3.0), cl.poll());
        assertEquals(new Candidate(0, 4.0), cl.poll());
    }

    @Test
    public void testMergeCandidateListCandidateList() {
        CandidateList cList1 = new CandidateList(2);
        cList1.add(new Candidate(1, 1.0));
        cList1.add(new Candidate(3, 3.0));
        CandidateList cList2 = new CandidateList(1);
        cList2.add(new Candidate(2, 2.0));

        Candidate[] mergeList = CandidateList.merge(cList1, cList2).toArray(new Candidate[0]);
        Candidate[] expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(2, 2.0),
                new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);
    }

    @Test
    public void testMergeListOfCandidateCandidateList() {
        CandidateList cList1 = new CandidateList(2);
        cList1.add(new Candidate(1, 1.0));
        cList1.add(new Candidate(3, 3.0));
        Candidate[] mergeList = CandidateList.merge(Arrays.asList(new Candidate(2, 2.0)), cList1)
                .toArray(new Candidate[0]);
        Candidate[] expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(2, 2.0),
                new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);
    }

    @Test
    public void testMergeAndExclude() {
        CandidateList cList1 = new CandidateList(2);
        cList1.add(new Candidate(1, 1.0));
        cList1.add(new Candidate(3, 3.0));

        // test merge
        Candidate[] mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(3, 3.0)),
                cList1, new Candidate(-1, -1.0), 3).toArray(new Candidate[0]);
        Candidate[] expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);

        // test exclude
        mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(2, 2.0)), cList1,
                new Candidate(-1, -1.0), 3).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(2, 2.0),
                new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);
        mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(2, 2.0)), cList1,
                new Candidate(2, 2.0), 3).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);

        // test keep the maximum size 
        mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(2, 2.0)), cList1,
                new Candidate(-1, -1.0), 1).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0) };
        assertArrayEquals(expected, mergeList);
        mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(2, 2.0)), cList1,
                new Candidate(-1, -1.0), 2).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(2, 2.0) };
        assertArrayEquals(expected, mergeList);
        mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(2, 2.0)), cList1,
                new Candidate(-1, -1.0), 3).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(2, 2.0),
                new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);
        mergeList = CandidateList.mergeAndExclude(Arrays.asList(new Candidate(2, 2.0)), cList1,
                new Candidate(-1, -1.0), 4).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(2, 2.0),
                new Candidate(1, 1.0) };
        assertArrayEquals(expected, mergeList);
    }

    @Test
    public void testIntersectListOfCandidateCandidateList() {
        // no intersections
        CandidateList cList1 = new CandidateList(2);
        cList1.add(new Candidate(1, 1.0));
        cList1.add(new Candidate(3, 3.0));
        CandidateList cList2 = new CandidateList(1);
        cList2.add(new Candidate(2, 2.0));
        Candidate[] intersectList = CandidateList.intersect(cList1, cList2).toArray(
                new Candidate[0]);
        Candidate[] expected = new Candidate[0];
        assertArrayEquals(expected, intersectList);

        // all intersect
        CandidateList cList3 = new CandidateList(2);
        cList3.add(new Candidate(1, 1.0));
        cList3.add(new Candidate(3, 3.0));
        intersectList = CandidateList.intersect(cList1, cList3).toArray(new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(1, 1.0) };
        assertArrayEquals(expected, intersectList);
    }

    @Test
    public void testIntersectCandidateListCandidateList() {
        // no intersections
        CandidateList cList1 = new CandidateList(2);
        cList1.add(new Candidate(1, 1.0));
        cList1.add(new Candidate(3, 3.0));
        Candidate[] intersectList = CandidateList.intersect(Arrays.asList(new Candidate(2, 2.0)),
                cList1).toArray(new Candidate[0]);
        Candidate[] expected = new Candidate[0];
        assertArrayEquals(expected, intersectList);

        // all intersect
        intersectList = CandidateList.intersect(
                Arrays.asList(new Candidate(3, 3.0), new Candidate(1, 1.0)), cList1).toArray(
                new Candidate[0]);
        expected = new Candidate[] { new Candidate(3, 3.0), new Candidate(1, 1.0) };
        assertArrayEquals(expected, intersectList);
    }

}
