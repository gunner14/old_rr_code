package example;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

public class DecorateOne {

    private static final String DATA = "hello_world";

    static void genDataFiles() throws Exception {
        FileOutputStream out = new FileOutputStream("decorate_raw.txt");
        out.write(DATA.getBytes());
        out.flush();
        out.close();

        GZIPOutputStream gzipOut = new GZIPOutputStream(new FileOutputStream("decorate_gzip.txt"));
        gzipOut.write(DATA.getBytes());
        gzipOut.flush();
        gzipOut.close();
    }

    static void foo() throws Exception {
        FileInputStream in = new FileInputStream("decorate_raw.txt");
        byte[] buffer = new byte[10240];
        int len = in.read(buffer);
        System.out.println("foo() value: " + new String(buffer, 0, len));
        in.close();
    }

    static void goo() throws Exception {
        FileInputStream in = new FileInputStream("decorate_gzip.txt");
        byte[] buffer = new byte[10240];
        int len = in.read(buffer);
        System.out.println("goo() value: " + new String(buffer, 0, len));
        in.close();
    }

    static void hoo() throws Exception {
        InputStream in = new GZIPInputStream(new FileInputStream("decorate_gzip.txt"));
        byte[] buffer = new byte[10240];
        int len = in.read(buffer);
        System.out.println("hoo() value: " + new String(buffer, 0, len));
        in.close();
    }

    static void joo() throws Exception {
        InputStream in = new UpperCaseInputStream(new GZIPInputStream(new FileInputStream(
                "decorate_gzip.txt")));
        byte[] buffer = new byte[10240];
        int len = in.read(buffer);
        System.out.println("joo() value: " + new String(buffer, 0, len));
        in.close();

    }

    public static void main(String[] args) {
        try {
            //            genDataFiles();
            //            foo();
            //            goo();
            hoo();
            joo();
            // readLine
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("a.txt")));
            FileReader FileInputStream
            reader.readLine();
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class UpperCaseInputStream extends FilterInputStream {

    protected UpperCaseInputStream(InputStream in) {
        super(in);
    }

    public int read() throws IOException {
        int c = super.read();
        return (c == -1 ? c : Character.toUpperCase(c));
    }

    public int read(byte[] b, int offset, int len) throws IOException {
        int result = super.read(b, offset, len);
        for (int i = offset; i < offset + result; i++) {
            b[i] = (byte) Character.toUpperCase((char) b[i]);
        }
        return result;
    }

}
