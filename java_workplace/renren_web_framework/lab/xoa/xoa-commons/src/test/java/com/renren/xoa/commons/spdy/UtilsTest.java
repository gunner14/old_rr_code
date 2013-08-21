package com.renren.xoa.commons.spdy;

import static org.junit.Assert.*;

import java.util.Random;

import org.junit.Test;

/**
 * {@link Utils}的测试用例
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-30 下午05:38:32
 */
public class UtilsTest {

	@Test
    public void testByteIntConvert() {
    	//测试int byte互转的代码
		for (int n = Integer.MIN_VALUE; n < Integer.MIN_VALUE + 1000; n ++) {
			int n1 = n;
			byte[] bs = Utils.intToByte(n1);
			int n2 = Utils.bytesToInt(bs);
			assertEquals(n1, n2);
			/*if (n1 != n2) {
				System.out.println(n);
				System.out.println(n1);
				System.out.println(n2);
				break;
			}*/
		}
		
		for (int n = -1000; n < 1000; n ++) {
			int n1 = n;
			byte[] bs = Utils.intToByte(n1);
			int n2 = Utils.bytesToInt(bs);
			assertEquals(n1, n2);
			/*if (n1 != n2) {
				System.out.println(n);
				System.out.println(n1);
				System.out.println(n2);
				break;
			}*/
		}
		
		for (int n = Integer.MAX_VALUE - 1000; n < Integer.MAX_VALUE; n ++) {
			int n1 = n;
			byte[] bs = Utils.intToByte(n1);
			int n2 = Utils.bytesToInt(bs);
			assertEquals(n1, n2);
			/*if (n1 != n2) {
				System.out.println(n);
				System.out.println(n1);
				System.out.println(n2);
				break;
			}*/
		}
    }
	
	@Test
	public void testBytesMerge() {
		String s1 = "fasdfsadfasd";
		String s2 = "fsadfsdaffasdf";
		String s3 = "fasfasdfasd";
		byte[] bytes = Utils.mergeBytes(s1.getBytes(), s2.getBytes(), s3.getBytes());
		assertArrayEquals(bytes, (s1 + s2 + s3).getBytes());
	}
	
	@Test
	public void tesetBytesToIntWithOffset() {
		
		Random random = new Random();
		int count = 1000;
		while (count-- > 0) {
			int n = random.nextInt(Integer.MAX_VALUE);
			byte[] bytes = Utils.intToByte(n);
			
			int offset = random.nextInt(10);
			
			byte[] bytes2 = new byte[4 + offset + 100];
			System.arraycopy(bytes, 0, bytes2, offset, 4);
			
			int n2 = Utils.bytesToInt(bytes2, offset, 4);
			assertEquals(n, n2);
		}
	}
	
	@Test
    public void testByteIntConvert2() {
    	//测试int byte互转的代码
		for (int n = Integer.MIN_VALUE; n < Integer.MIN_VALUE + 1000; n ++) {
			int n1 = n;
			byte[] bs = Utils.intToByte(n1);
			int n2 = Utils.bytesToInt(bs, 0, 4);
			assertEquals(n1, n2);
			/*if (n1 != n2) {
				System.out.println(n);
				System.out.println(n1);
				System.out.println(n2);
				break;
			}*/
		}
		
		for (int n = -1000; n < 1000; n ++) {
			int n1 = n;
			byte[] bs = Utils.intToByte(n1);
			int n2 = Utils.bytesToInt(bs, 0, 4);
			assertEquals(n1, n2);
			/*if (n1 != n2) {
				System.out.println(n);
				System.out.println(n1);
				System.out.println(n2);
				break;
			}*/
		}
		
		for (int n = Integer.MAX_VALUE - 1000; n < Integer.MAX_VALUE; n ++) {
			int n1 = n;
			byte[] bs = Utils.intToByte(n1);
			int n2 = Utils.bytesToInt(bs, 0, 4);
			assertEquals(n1, n2);
			/*if (n1 != n2) {
				System.out.println(n);
				System.out.println(n1);
				System.out.println(n2);
				break;
			}*/
		}
    }
}
