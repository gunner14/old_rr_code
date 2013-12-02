package xce.tripod;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class IOStreamUtil {

    static Scanner input = new Scanner(System.in);

    static String getString() {
        return input.nextLine().trim();
    }

    static char getChar() {
        return getString().toCharArray()[0];
    }

    static void print(Object message) {
        System.out.println(message);
    }

    static void makeSureWithPromt(Object message) {
        if (message != null) {
            print(message);
        }
        print("确定?(Y/N)");
        char makesure = getChar();
        if (makesure != 'Y' && makesure != 'y') {
            IOStreamUtil.print("退出..");
            System.exit(1);
        }
    }

    static String readFile(String filePath) throws IOException {
        StringBuffer sb = new StringBuffer();
        BufferedReader in = new BufferedReader(new FileReader(filePath));
        String s;
        while ((s = in.readLine()) != null) {
            sb.append(s).append("\n");
        }
        in.close();
        return sb.toString();
    }

    static void writeFile(String filePath, String message) throws IOException {
        FileWriter fw = new FileWriter(filePath);
        fw.write(message, 0, message.length());
        fw.flush();
        fw.close();
    }

}
