package org.apache.mahout.fpm.pfpgrowth.extra;

import java.text.SimpleDateFormat;
import java.util.Date;

public class CommonUtils {

    public static final int HOUR = 3600;
    public static final int MIN = 60;
    public static final int W = 10000;

    public static final long K = 1024;
    public static final long M = 1024 * K;
    public static final long G = 1024 * M;

    public static final SimpleDateFormat SDF = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");

    /**
     * Format the milliseconds to the format: x hours, y mins, z sec
     * 
     * @param millis
     * @return
     */
    public static String toHumanReadableTime(long millis) {
        millis = millis / 1000;
        String result = "";
        int hour = (int) (millis / HOUR);
        int min = (int) ((millis - HOUR * hour) / MIN);
        int sec = (int) (millis - HOUR * hour - MIN * min);
        if (hour > 0) {
            result += hour + " hrs ";
        }
        if (min > 0) {
            result += min + " mins ";
        }
        result += sec + " sec";
        return result;
    }

    /**
     * Format the size to the format: aGB bMB cKB d
     * 
     * @param size
     * @return
     */
    public static String toHumanReadableSize(long size) {
        String result = "";
        int g = (int) (size / G);
        int m = (int) ((size - g * G) / M);
        int k = (int) ((size - g * G - m * M) / K);
        int left = (int) (size - g * G - m * M - k * K);

        if (g > 0) {
            result += g + "GB ";
        }
        if (m > 0) {
            result += m + "MB ";
        }
        if (k > 0) {
            result += k + "KB ";
        }
        result += left;
        return result;
    }

    /**
     * Extract the file name from the full file path
     * 
     * @param name
     * @return
     */
    public static String getFileName(String name) {
        int idx = name.lastIndexOf("/");
        if (idx == -1) {
            return name;
        }
        return name.substring(idx + 1, name.length() - 1);
    }

    /**
     * Print the message with timestamp
     * 
     * @param message message wants to output
     */
    public static void printWithTS(String message) {
        System.out.println(SDF.format(new Date()) + " " + message);
    }

    public static void main(String[] args) {
        printWithTS("test");
    }
}
