package com.xiaonei.reg.guide.util;

import java.util.Random;

/*
 * Rand.java
 *
 * Created on 2009��3��22��, ����1:16
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 * 
 */
public class RandCodeUtil {
    private static int te1 = 0,r=0,temp=0,k=0,a=0;
    private static Random rnd;
    public static RandCodeUtil instance = new RandCodeUtil();
    static StringBuffer enc=new StringBuffer();


    public RandCodeUtil() {
        rnd=new Random();
    }

    public static int getRndInt(int min, int max) {
        temp=(int) System.currentTimeMillis();
        if(temp<0) {
            temp*=-1;
        }
         k = rnd.nextInt(temp);
        if (k == te1) {
            k = 1;
        }
        te1 = k;
        return (k % (max - min) + min);
    }

    public static Random getRnd() {

        return rnd;
    }
    
    /**
     * 产生随机的字符串,其中包括数字和字母
     * @param min 字符串最小长度
     * @param max 字符串最大长度
     * @return enc 随机字符串
     * <p>Example:getRandChar(8,9);//产生一个九位随机字符串包含字母和数字</p>
     * */
    public static String getRandChar(int min,int max){
        enc.delete(0, enc.length());
         r = getRndInt(min, max);
        for (;r > 0;) { // 产生原始字符串（随机）
             a = getRndInt(64, 126);
            if (((a>=48)&&(a<=57))||((a>=97)&&(a<=122))||((a>=65)&&(a<=90))) {//可见的ASCII字母,数字
                enc.append((char) a);
                r--;    
            }else{continue;}
            
        }
        return enc.toString();
    }

    /**
     * 产生随机数字
     * @param min 字串最小长度
     * @param max 字串最大长度
     * @return enc 随机字串
     * */
    public static String getRandNum(int min,int max){
        enc.delete(0, enc.length());
        r = getRndInt(min, max);
        for (;r > 0;) { // 产生原始字符串（随机）
                enc.append((char) getRndInt(48, 57));
                r--;            
        }
        return enc.toString();
    }
    public static void main(String[] args) {
        int i=0;

        String a;

        for(i=0; i++<1000000;){
            a=getRandNum(1, 2);
           System.out.println(a);
          
    }
        System.out.println("ok");
    }
}

