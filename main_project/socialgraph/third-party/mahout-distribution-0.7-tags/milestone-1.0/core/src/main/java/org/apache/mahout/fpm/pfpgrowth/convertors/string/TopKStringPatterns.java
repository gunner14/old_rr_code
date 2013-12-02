/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.mahout.fpm.pfpgrowth.convertors.string;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import org.apache.hadoop.io.Writable;
import org.apache.mahout.common.Pair;

import com.google.common.collect.Lists;

/**
 * A class which collects Top K string patterns
 * 
 */
public final class TopKStringPatterns implements Writable {

    private final List<Pair<List<String>, Long>> frequentPatterns;

    public TopKStringPatterns() {
        frequentPatterns = Lists.newArrayList();
    }

    public TopKStringPatterns(Collection<Pair<List<String>, Long>> patterns) {
        frequentPatterns = Lists.newArrayList();
        frequentPatterns.addAll(patterns);
    }

    public Iterator<Pair<List<String>, Long>> iterator() {
        return frequentPatterns.iterator();
    }

    public List<Pair<List<String>, Long>> getPatterns() {
        return frequentPatterns;
    }

    public TopKStringPatterns merge(TopKStringPatterns pattern, int heapSize) {
        List<Pair<List<String>, Long>> patterns = Lists.newArrayList();
        Iterator<Pair<List<String>, Long>> myIterator = frequentPatterns.iterator();
        Iterator<Pair<List<String>, Long>> otherIterator = pattern.iterator();
        Pair<List<String>, Long> myItem = null;
        Pair<List<String>, Long> otherItem = null;
        for (int i = 0; i < heapSize; i++) {
            if (myItem == null && myIterator.hasNext()) {
                myItem = myIterator.next();
            }
            if (otherItem == null && otherIterator.hasNext()) {
                otherItem = otherIterator.next();
            }
            if (myItem != null && otherItem != null) {
                int cmp = myItem.getSecond().compareTo(otherItem.getSecond());
                if (cmp == 0) {
                    cmp = myItem.getFirst().size() - otherItem.getFirst().size();
                    if (cmp == 0) {
                        for (int j = 0; j < myItem.getFirst().size(); j++) {
                            cmp = myItem.getFirst().get(j).compareTo(otherItem.getFirst().get(j));
                            if (cmp != 0) {
                                break;
                            }
                        }
                    }
                }
                if (cmp <= 0) {
                    patterns.add(otherItem);
                    if (cmp == 0) {
                        myItem = null;
                    }
                    otherItem = null;
                } else if (cmp > 0) {
                    patterns.add(myItem);
                    myItem = null;
                }
            } else if (myItem != null) {
                patterns.add(myItem);
                myItem = null;
            } else if (otherItem != null) {
                patterns.add(otherItem);
                otherItem = null;
            } else {
                break;
            }
        }
        return new TopKStringPatterns(patterns);
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        frequentPatterns.clear();
        int length = in.readInt();
        for (int i = 0; i < length; i++) {
            List<String> items = Lists.newArrayList();
            int itemsetLength = in.readInt();
            long support = in.readLong();
            for (int j = 0; j < itemsetLength; j++) {
                items.add(in.readUTF());
            }
            frequentPatterns.add(new Pair<List<String>, Long>(items, support));
        }
    }

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeInt(frequentPatterns.size());
        for (Pair<List<String>, Long> pattern : frequentPatterns) {
            out.writeInt(pattern.getFirst().size());
            out.writeLong(pattern.getSecond());
            for (String item : pattern.getFirst()) {
                out.writeUTF(item);
            }
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        String sep = "";
        for (Pair<List<String>, Long> pattern : frequentPatterns) {
            sb.append(sep);
            sb.append(pattern.toString());
            sep = ", ";

        }
        return sb.toString();
    }

    public static TopKStringPatterns fromString(String objectStr) {
        String[] items = objectStr.split("\\), \\(");
        //item: 
        List<Pair<List<String>, Long>> data = new ArrayList<Pair<List<String>, Long>>();
        for (String item : items) {
            data.add(parsePair(item));
        }
        return new TopKStringPatterns(data);
    }

    /**
     * Parse Pair from string
     * 
     * @param item the format of item has 4 kinds:
     * full: ([85128498, 100405456],30) 
     * head: ([85128498, 100405456],30 
     * tail: [85128498, 100405456],30) 
     * others: [85128498, 100405456],30
     * 
     * @return
     */
    private static Pair<List<String>, Long> parsePair(String data) {
        int idx = data.lastIndexOf(",");
        String items0 = data.substring(0, idx);
        String items1 = data.substring(idx+1, data.length());
        String keyStr = null, valStr = null;
        
        if(items0.startsWith("(")){
            if(items1.endsWith(")")){ //full
                keyStr = items0.substring(2, items0.length()-1);
                valStr = items1.substring(0, items1.length()-1);
            }else{ //head
                keyStr = items0.substring(2, items0.length() - 1);
                valStr = items1;
            }
        }else if(items1.endsWith(")")){ //tail
            keyStr = items0.substring(1, items0.length() - 1);
            valStr = items1.substring(0, items1.length() -1);
        }else{
            keyStr = items0.substring(1, items0.length() -1);
            valStr = items1;
        }

        List<String> key = new ArrayList<String>();
        for (String str : keyStr.split(", ")) {
            key.add(str);
        }
        Long val = Long.parseLong(valStr);

        return new Pair<List<String>, Long>(key, val);
    }

    public static void main(String[] args) {
        String rawStr = "([224838055, 231915228, 238111132, 248528808, 250137860, 256404211, 46358, 224357645, 232491085],8)";
        TopKStringPatterns patterns = TopKStringPatterns.fromString(rawStr);
        System.out.println(patterns);
    }
}
