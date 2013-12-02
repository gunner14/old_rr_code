package com.renren.ad.sns.services.uranus;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class Candidate implements Writable, Comparable<Candidate> {

    public static double EPS = 1e-8;

    private long gid = 0;

    private double weight = 0;

    private CandidateType source = CandidateType.UnKnown;

    public Candidate() {
        super();
    }

    public Candidate(long gid, double weight) {
        super();
        this.gid = gid;
        this.weight = weight;
    }

    public Candidate(long gid, double weight, CandidateType source) {
        this(gid, weight);
        this.source = source;
    }

    public void setGid(long gid) {
        this.gid = gid;
    }

    public void setWeight(double weight) {
        this.weight = weight;
    }

    public long getGid() {
        return gid;
    }

    public double getWeight() {
        return weight;
    }

    public CandidateType getSource() {
        return source;
    }

    public void setSource(CandidateType source) {
        this.source = source;
    }

    @Override
    public String toString() {
        return "Candidate [gid=" + gid + ", source=" + source + ", weight=" + weight + "]";
    }

    public String toSimpleString() {
        return "[gid=" + gid + ", src=" + source.getValue() + ", w=" + weight + "]";
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + (int) (gid ^ (gid >>> 32));
        long temp;
        temp = Double.doubleToLongBits(weight);
        result = prime * result + (int) (temp ^ (temp >>> 32));
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }

        Candidate other = (Candidate) obj;
        if (gid == other.gid
                && Double.doubleToLongBits(weight) == Double.doubleToLongBits(other.weight)
                && source == other.source) {
            return true;
        }
        return false;
    }

    @Override
    public int compareTo(Candidate oth) {
        double delta = this.weight - oth.weight;
        if (Math.abs(delta) < EPS) {
            return this.gid - oth.gid > 0 ? 1 : -1;
        } else {
            return delta > EPS ? 1 : -1;
        }
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        this.gid = in.readLong();
        this.weight = in.readDouble();
        this.source = CandidateType.getCandidateType(in.readInt());
    }

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeLong(this.gid);
        out.writeDouble(this.weight);
        out.writeInt(source.getValue());
    }

}
