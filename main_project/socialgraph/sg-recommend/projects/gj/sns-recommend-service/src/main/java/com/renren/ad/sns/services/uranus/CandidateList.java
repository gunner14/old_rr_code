package com.renren.ad.sns.services.uranus;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.ad.util.io.WritablePriorityQueue;

public class CandidateList extends WritablePriorityQueue<Candidate> implements
		Comparable<CandidateList> {
	private static final Log LOG = LogFactory.getLog(CandidateList.class);
	private static final long serialVersionUID = -2988095063339882341L;
	private int tagId = 0;
	private int capacity = 0;

	@Override
	protected Candidate newElementInstance() {
		return new Candidate();
	}

	public CandidateList() {
	}

	public CandidateList(int capacity) {
		super(capacity);
		this.capacity = capacity;
	}

	@Override
	public boolean add(Candidate candidate) {
		boolean res = true;
		try {
			if (this.capacity > 0 && super.size() >= this.capacity) {
				Candidate c = super.peek();
				if (c.getWeight() < candidate.getWeight()) {
					super.poll();
					super.add(candidate);
				}
			} else {
				super.add(candidate);
			}
		} catch (Exception e) {
			res = false;
			LOG.error("Add Candidate " + candidate + " error!", e);
		}
		return res;
	}

	public void write(DataOutput out) throws IOException {
		super.write(out);
		out.writeInt(tagId);
	}

	public void readFields(DataInput in) throws IOException {
		super.readFields(in);
		this.tagId = in.readInt();
	}

	public int getTagId() {
		return tagId;
	}

	public void setTagId(int tagId) {
		this.tagId = tagId;
	}

	@Override
	public String toString() {
		return "CandidateList [tagId=" + tagId + "]";
	}

	/**
	 * use carefully : the <i>docId</i> in postingList shell be both <B> in
	 * order<B>
	 * 
	 * @param o1
	 * @param o2
	 * @return
	 */
	public static List<Candidate> merge(CandidateList oth1, CandidateList oth2) {
		Candidate[] o1 = null, o2 = null;
		List<Candidate> tmp = new ArrayList<Candidate>();
		List<Candidate> res = new ArrayList<Candidate>();

		if (oth1 == null) {
			o1 = new Candidate[0];
		} else {
			o1 = new Candidate[0];
			o1 = oth1.toArray(o1);
		}
		if (oth2 == null || oth2.size() == 0) {
			o2 = new Candidate[0];
		} else {
			o2 = new Candidate[0];
			o2 = oth2.toArray(o2);
		}
		for (Candidate doc : o1) {
			tmp.add(doc);
		}
		for (Candidate doc : o2) {
			tmp.add(doc);
		}
		Collections.sort(tmp, new CandidateComparator());
		long last = -1;
		for (Candidate doc : tmp) {
			if (last != doc.getGid()) {
				res.add(doc);
			}
			last = doc.getGid();
		}
		return res;
	}

	public static List<Candidate> merge(List<Candidate> oth1, CandidateList oth2) {
		Candidate[] o1 = null, o2 = null;
		List<Candidate> tmp = new ArrayList<Candidate>();
		List<Candidate> res = new ArrayList<Candidate>();

		if (oth1 == null) {
			o1 = new Candidate[0];
		} else {
			o1 = new Candidate[0];
			o1 = oth1.toArray(o1);
		}
		if (oth2 == null || oth2.size() == 0) {
			o2 = new Candidate[0];
		} else {
			o2 = new Candidate[0];
			o2 = oth2.toArray(o2);
		}
		for (Candidate doc : o1) {
			tmp.add(doc);
		}
		for (Candidate doc : o2) {
			tmp.add(doc);
		}
		Collections.sort(tmp, new CandidateComparator());
		long last = -1;
		for (Candidate doc : tmp) {
			if (last != doc.getGid()) {
				res.add(doc);
			}
			last = doc.getGid();
		}
		return res;
	}

	public static Map<Candidate, Integer> merge(
			Map<Candidate, Integer> result, CandidateList oth2,
			Candidate exclude, int maxSize) {
		List<Candidate> all = new ArrayList<Candidate>();
		all.addAll(oth2);
		Collections.sort(all, new CandidateComparator());
		
		long excludeGid = exclude.getGid();
		long lastGid = -1;
		for (Candidate c : all) {
			if (result.size() == maxSize) {
				break;
			}
			if (c.getGid() != lastGid && c.getGid() != excludeGid) {
				int num = 1;
				if (result.containsKey(c)){
					num += result.get(c);
				}
				result.put(c, num);
			}
			lastGid = c.getGid();
		}
		return result;
	}

	/**
	 * Merge two Candidate list, exclude the specified element, returns at most
	 * maxSize elements.
	 * 
	 * @param oth1
	 * @param oth2
	 * @param exclude
	 * @param maxSize
	 * @return
	 */
	public static List<Candidate> mergeAndExclude(List<Candidate> oth1,
			CandidateList oth2, Candidate exclude, int maxSize) {
		List<Candidate> result = new ArrayList<Candidate>();
		List<Candidate> all = new ArrayList<Candidate>();
		if (oth1 != null && oth2 != null) {
			all.addAll(oth1);
			all.addAll(oth2);
		} else if (oth1 == null && oth2 != null) {
			all.addAll(oth2);
		} else if (oth1 != null && oth2 == null) {
			all.addAll(oth1);
		}

		Collections.sort(all, new CandidateComparator());
		long lastGid = -1;
		long excludeGid = exclude.getGid();
		for (Candidate c : all) {
			if (result.size() == maxSize) {
				break;
			}
			if (c.getGid() != lastGid && c.getGid() != excludeGid) {
				result.add(c);
			}
			lastGid = c.getGid();
		}
		return result;
	}

	public static List<Candidate> intersect(List<Candidate> oth1,
			CandidateList oth2) {
		Candidate[] o1 = null, o2 = null;
		List<Candidate> res = new ArrayList<Candidate>();

		if (oth1 == null) {
			o1 = new Candidate[0];
		} else {
			o1 = new Candidate[0];
			o1 = oth1.toArray(o1);
		}
		if (oth2 == null || oth2.size() == 0) {
			o2 = new Candidate[0];
		} else {
			o2 = new Candidate[0];
			o2 = oth2.toArray(o2);
		}

		Set<Candidate> tmp = new HashSet<Candidate>();
		for (Candidate doc : o1) {
			tmp.add(doc);
		}
		for (Candidate doc : o2) {
			boolean isok = tmp.add(doc);
			if (!isok) {
				res.add(doc);
			}
		}
		Collections.sort(res, new CandidateComparator());
		return res;
	}

	public static List<Candidate> intersect(CandidateList oth1,
			CandidateList oth2) {
		Candidate[] o1 = null, o2 = null;
		List<Candidate> res = new ArrayList<Candidate>();

		if (oth1 == null) {
			o1 = new Candidate[0];
		} else {
			o1 = new Candidate[0];
			o1 = oth1.toArray(o1);
		}
		if (oth2 == null || oth2.size() == 0) {
			o2 = new Candidate[0];
		} else {
			o2 = new Candidate[0];
			o2 = oth2.toArray(o2);
		}

		Set<Candidate> tmp = new HashSet<Candidate>();
		for (Candidate doc : o1) {
			tmp.add(doc);
		}
		for (Candidate doc : o2) {
			boolean isok = tmp.add(doc);
			if (!isok) {
				res.add(doc);
			}
		}
		Collections.sort(res, new CandidateComparator());
		return res;
	}

	static class CandidateComparator implements Comparator<Candidate> {
		@Override
		public int compare(Candidate o1, Candidate o2) {
			double delta = o1.getWeight() - o2.getWeight();
			if (Math.abs(delta) < Candidate.EPS) {
				return o1.getGid() - o2.getGid() > 0 ? 1 : -1;
			} else {
				return delta > Candidate.EPS ? -1 : 1;
			}
		}
	}

	@Override
	public int compareTo(CandidateList o) {
		return o.size() - this.size();
	}

}
