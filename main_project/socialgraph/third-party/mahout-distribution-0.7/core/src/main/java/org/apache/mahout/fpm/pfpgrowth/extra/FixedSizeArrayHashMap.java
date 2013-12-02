package org.apache.mahout.fpm.pfpgrowth.extra;
import java.util.HashMap;
import java.util.Map;

/**
 * memory-aware hash map
 * @author xiang.zhang 
 * last updated: 2012/10/21
 */
public class FixedSizeArrayHashMap {

    private final float OVERLOAD_FACTOR = 1.5f;
    public final int NOT_USED_MARK = Integer.MIN_VALUE;
    public final int KEY_NOT_FOUND_MARK = Integer.MIN_VALUE + 1;

    private int size;
    private int[] keyArr;
    private int[] valArr;
    private int used = 0;
    private Map<Integer, Integer> conflictMap;

    public FixedSizeArrayHashMap(int size) {
        this.size = size;
        int acutalSize = (int) (size * OVERLOAD_FACTOR);
        keyArr = new int[acutalSize];
        valArr = new int[acutalSize];
        for (int i = 0; i < acutalSize; i++) {
            keyArr[i] = NOT_USED_MARK;
        }
        conflictMap = new HashMap<Integer, Integer>(100, 0.9f);
    }

    public void put(int key, int val) {
        int idx = getArrayIdx(key);
        if (keyArr[idx] == NOT_USED_MARK) {
            //System.out.printf("add <%d, %d> to arr[%d]%n", key, val, idx);
            keyArr[idx] = key;
            valArr[idx] = val;
            used++;
        } else {
            //System.out.printf("%d used, move <%d, %d> to conflictMap%n", idx, key, val);
            conflictMap.put(key, val);
        }
    }

    public int get(int key) {
        int idx = getArrayIdx(key);

        if (keyArr[idx] == key) {
            return valArr[idx];
        } else if (conflictMap.containsKey(key)) {
            return conflictMap.get(key);
        }

        return KEY_NOT_FOUND_MARK;
    }

    public boolean containsKey(int key) {
        if (get(key) != KEY_NOT_FOUND_MARK) {
            return true;
        }
        return false;
    }

    public float getLoadFactor() {
        return 1.0f * used / size;
    }

    public int getConflictSize() {
        return conflictMap.size();
    }

    private int getArrayIdx(int key) {
        return indexFor(key, (int) (size * OVERLOAD_FACTOR));
    }

    private int indexFor(int h, int length) {
        //return h & (length - 1);
        return h % (length - 1);
    }

    public int getUsed() {
        return used;
    }

    public int getSize() {
        return size;
    }
    
    public String toSummaryString(){
        return String.format("size:%d, used:%d, loadFactor:%.3f, numConflict:%d", 
                size, used, getLoadFactor(), getConflictSize());
    }

}
