package com.xiaonei.platform.core.menu;

import org.junit.Assert;
import org.junit.Test;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-29 下午10:48:12
 */
public class FrontEndMenuProcessorTest {

	private FrontEndMenuProcessor processor = new FrontEndMenuProcessor();
	
	
	@Test
	public void testNoneRecentAndNonePresent() {
		
		int[] recentAppIds = {};
		int[] needPresentAppIds = {};
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		Assert.assertEquals(0, union.length);
	}
	
	@Test
	public void testNoneRecent() {
		
		int[] recentAppIds = {};
		int[] needPresentAppIds = {81,82,83};
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		Assert.assertArrayEquals(needPresentAppIds, union);
	}
	
	@Test
	public void testNoneRecent2() {
		
		int[] recentAppIds = {};
		int[] needPresentAppIds = new int[FrontEndMenuProcessor.DISPLAY_THRESHOLD];
		for (int i = 0; i < needPresentAppIds.length; i++) {
			needPresentAppIds[i] = 80 + i;
		}
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		Assert.assertArrayEquals(needPresentAppIds, union);
	}
	
	@Test
	public void testNonePresent() {
		
		int[] recentAppIds = {11,12,13};
		int[] needPresentAppIds = {};
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		Assert.assertArrayEquals(recentAppIds, union);
	}
	
	@Test
	public void testNonePresent2() {
		
		int[] recentAppIds = new int[FrontEndMenuProcessor.DISPLAY_THRESHOLD];
		for (int i = 0; i < recentAppIds.length; i++) {
			recentAppIds[i] = 10 + i;
		}
		int[] needPresentAppIds = {};
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		Assert.assertArrayEquals(recentAppIds, union);
	}
	
	@Test
	public void testUnion1() {
		
		int[] recentAppIds = new int[FrontEndMenuProcessor.DISPLAY_THRESHOLD];
		for (int i = 0; i < recentAppIds.length; i++) {
			recentAppIds[i] = 10 + i;
		}
		int[] needPresentAppIds = {81,82,83};
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		
		int[] expected = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,81,82,83};
		
		Assert.assertArrayEquals(expected, union);
	}
	
	@Test
	public void testUnion2() {
		
		int[] recentAppIds = {10,11,12,13,14,15,81,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
		int[] needPresentAppIds = {81,82,83};
		
		int[] union = processor.union(recentAppIds, needPresentAppIds);
		
		int[] expected = {10,11,12,13,14,15,81,16,17,18,19,20,21,22,23,24,25,26,27,28,82,83};
		Assert.assertArrayEquals(expected, union);
	}
	
}
