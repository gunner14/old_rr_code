package com.xiaonei.platform.core.utility.photo;
//
import java.awt.Dimension;
import java.io.File;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import magick.ColorspaceType;
import magick.ImageInfo;
import magick.InterlaceType;
import magick.MagickImage;

import org.apache.commons.lang.RandomStringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import xce.imagemagickd.Image;
import xce.imagemagickd.ResizeConfig;
import xce.imagemagickd.client.ImageMagickdAdapter;
import xce.imagemagickd.client.ResizeFailedException;

import com.xiaonei.platform.core.file.FileFactory;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.utility.ImgPro;
import com.xiaonei.platform.core.utility.photo.GroupException;
import com.xiaonei.platform.storage.Allocator;
import com.xiaonei.platform.storage.Group;
import com.xiaonei.platform.storage.Node;
import com.xiaonei.xce.log.TimeCost;

public class PhotoManager {
	
	private Log logger = LogFactory.getLog(PhotoManager.class);
	
	public static String SIZENAME_LARGE = "large";

	public static String SIZENAME_MAIN = "main";

	public static String SIZENAME_HEAD = "head";

	public static String SIZENAME_TINY = "tiny";
	
	/** large图宽度 */
	public static String SIZE_LARGE_WIDTH = "large_width";
	/** large图高度 */
	public static String SIZE_LARGE_HEIGHT = "large_height";
	
	public static String NAME_PREFIX_PHOTO = "p_";
	/*blog没用这个类*/
	public static String NAME_PREFIX_BLOG = "b_";
	public static String NAME_PREFIX_EVENT = "e_";
	public static String NAME_PREFIX_GROUP = "g_";
	public static String NAME_PREFIX_SCHOOL = "s_";
	public static String NAME_PREFIX_TRIBE = "t_"; 
    
	public static PhotoManager instance =new PhotoManager() ;
	public static PhotoManager getInstance(){
		return instance ;
	}
	public PhotoManager(){
		
	}
	
	/**
	 * 路径层次太深，请使用{@link #processHead(MagickImage, int, String)}
	 * 
	 * @param source
	 * @param type
	 * @return
	 * @throws GroupException
	 * @throws Exception
	 */
	@Deprecated 
	public Map processHead(MagickImage source,int type) throws GroupException, Exception {
		MagickImage regulate = null;
		MagickImage large = null;
		MagickImage main = null;
		MagickImage head = null;
		MagickImage tiny = null;
		Group gp=Allocator.getInstance().allocateGroup(type) ;
		if(gp==null)
			throw new GroupException("no group found");
		List<Node> nd = gp.getNodes() ;
		Calendar cl = Calendar.getInstance();
		String rootPath = OpiConstants.tempTmpPath ;
		String sfx = FileFactory.getInstance().getRandName();
		 
		String dir ="pic001/"+ FileFactory.getInstance().getDiv1(cl) + "/"
				+ FileFactory.getInstance().getDiv2(cl) + "/"
				+ FileFactory.getInstance().getDiv3(cl);
		
		String largeName =  this.SIZENAME_LARGE + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String mainName = this.SIZENAME_MAIN + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String headName = this.SIZENAME_HEAD + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String tinyName = this.SIZENAME_TINY + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		/*
		 * System.out.println(rootPath+largePath) ;
		 * System.out.println(rootPath+mainPath) ;
		 * System.out.println(rootPath+headPath) ;
		 * System.out.println(rootPath+tinyPath) ;
		 */
		try {
			regulate = ImgPro.regulate(source);
			large = ImgPro.resizePhotoStep(regulate, rootPath + largeName, 600,
					900);
			main = ImgPro.resizePhotoStep(large, rootPath + mainName, 200, 300);
			head = ImgPro.resizePhotoStep(main, rootPath + headName, 100, 150);
			tiny = ImgPro.resizePhotoStep(head, rootPath + tinyName, 50, 75);
			ImageInfo out = new ImageInfo();
			out.setQuality(90);
			out.setInterlace(InterlaceType.PlaneInterlace); // interlace
			out.setColorspace(ColorspaceType.sRGBColorspace); // for TIFF jpeg
			out.setMagick("JPG"); // FIX somefile.bmp.jpg
			byte[] largeBt=large.imageToBlob(out);
			byte[] mainBt = main.imageToBlob(out);
			byte[] headBt = head.imageToBlob(out);
			byte[] tinyBt = tiny.imageToBlob(out);
			
			if(nd ==null)
				return null ;
			for(int i=0;i<nd.size();i++){
				String path = nd.get(i).getPath()+"/"+dir ;
				File f = new File(path) ;
				if(!f.exists())
					f.mkdirs() ;
				String largeFPath = path+"/"+largeName ;
				String mainFPath = path+"/"+mainName ;
				String headFPath = path+"/"+headName ;
				String tinyFPath = path+"/"+tinyName ;
				FileFactory.getInstance().writeFile(largeFPath, largeBt) ;
				FileFactory.getInstance().writeFile(mainFPath, mainBt) ;
				FileFactory.getInstance().writeFile(headFPath, headBt) ;
				FileFactory.getInstance().writeFile(tinyFPath, tinyBt) ;
				
			}
			Map mp =new HashMap() ;
			String dbPath = gp.getName()+"/"+dir ;
			mp.put(this.SIZENAME_LARGE, dbPath+"/"+largeName) ;
			mp.put(this.SIZENAME_MAIN, dbPath+"/"+mainName) ;
			mp.put(this.SIZENAME_HEAD, dbPath+"/"+headName) ;
			mp.put(this.SIZENAME_TINY, dbPath+"/"+tinyName) ;
			Dimension d = large.getDimension();
			mp.put(SIZE_LARGE_HEIGHT, d.height);
			mp.put(SIZE_LARGE_WIDTH, d.width);
			//System.out.println(mp) ;
			return mp ;
			
		} catch (Exception e) {
			throw e;
		} finally {
			if (tiny != null)
				tiny.destroyImages();
			if (head != null)
				head.destroyImages();
			if (main != null)
				main.destroyImages();
			if (large != null)
				large.destroyImages();

			if (regulate != null)
				regulate.destroyImages();

		}
		
	}
	
	/**
	 * 处理一组图像
	 * 
	 * @param source
	 * @param type
	 * @param namePrefix p: album, b: blog, e: event, g: groupalbum, s: schoolalbum, t:tribe
	 * @return
	 * @throws GroupException
	 * @throws Exception
	 */
	public Map processHead(MagickImage source,int type, String namePrefix) throws GroupException, Exception {
		TimeCost time = TimeCost.begin(getClass());
		MagickImage regulate = null;
		MagickImage large = null;
		MagickImage main = null;
		MagickImage head = null;
		MagickImage tiny = null;
		Group gp=Allocator.getInstance().allocateGroup(type) ;
		if(gp==null)
			throw new GroupException("no group found");
		List<Node> nd = gp.getNodes() ;
		if(nd ==null)
			return null ;
		//Calendar cl = Calendar.getInstance();
		String rootPath = OpiConstants.tempTmpPath ;
		String sfx = FileFactory.getInstance().getRandName();
		 
		/*String dir ="pic001/"+ FileFactory.getInstance().getDiv1(cl) + "/"
				+ FileFactory.getInstance().getDiv2(cl) + "/"
				+ FileFactory.getInstance().getDiv3(cl);*/
		// 使用短路径
		String dir = FileFactory.getInstance().getPathShort();
		
		String largeName =  namePrefix + this.SIZENAME_LARGE + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String mainName = namePrefix + this.SIZENAME_MAIN + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String headName = namePrefix + this.SIZENAME_HEAD + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String tinyName = namePrefix + this.SIZENAME_TINY + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		/*
		 * System.out.println(rootPath+largePath) ;
		 * System.out.println(rootPath+mainPath) ;
		 * System.out.println(rootPath+headPath) ;
		 * System.out.println(rootPath+tinyPath) ;
		 */
		try {
			time.endStep("start regulate");
			regulate = ImgPro.regulate(source);
			time.endStep("end regulate");
			large = ImgPro.resizePhotoStep(regulate, rootPath + largeName, 600,
					900);
			time.endStep("resize large");
			main = ImgPro.resizePhotoStep(large, rootPath + mainName, 200, 300);
			time.endStep("resize main");
			head = ImgPro.resizePhotoStep(main, rootPath + headName, 100, 150);
			time.endStep("resize head");
			tiny = ImgPro.resizePhotoStep(head, rootPath + tinyName, 50, 75);
			time.endStep("resize tiny");
			ImageInfo out = new ImageInfo();
			out.setQuality(90);
			out.setInterlace(InterlaceType.PlaneInterlace); // interlace
			out.setColorspace(ColorspaceType.sRGBColorspace); // for TIFF jpeg
			out.setMagick("JPG"); // FIX somefile.bmp.jpg
			time.endStep("start get large stream");
			byte[] largeBt=large.imageToBlob(out);
			time.endStep("end get large stream");
			byte[] mainBt = main.imageToBlob(out);
			time.endStep("end get main stream");
			byte[] headBt = head.imageToBlob(out);
			time.endStep("end get head stream");
			byte[] tinyBt = tiny.imageToBlob(out);
			time.endStep("end get tiny stream");
			

			for(int i=0;i<nd.size();i++){
				String path = nd.get(i).getPath()+"/"+dir ;
				File f = new File(path) ;
				if(!f.exists())
					f.mkdirs() ;
				String largeFPath = path+"/"+largeName ;
				String mainFPath = path+"/"+mainName ;
				String headFPath = path+"/"+headName ;
				String tinyFPath = path+"/"+tinyName ;
				
				FileFactory.getInstance().writeFile(largeFPath, largeBt) ;
				time.endStep("end write large");
				FileFactory.getInstance().writeFile(mainFPath, mainBt) ;
				time.endStep("end write main");
				FileFactory.getInstance().writeFile(headFPath, headBt) ;
				time.endStep("end write head");
				FileFactory.getInstance().writeFile(tinyFPath, tinyBt) ;
				time.endStep("end write tiny");
				
			}
			Map mp =new HashMap() ;
			String dbPath = gp.getName()+"/"+dir ;
			mp.put(this.SIZENAME_LARGE, dbPath+"/"+largeName) ;
			mp.put(this.SIZENAME_MAIN, dbPath+"/"+mainName) ;
			mp.put(this.SIZENAME_HEAD, dbPath+"/"+headName) ;
			mp.put(this.SIZENAME_TINY, dbPath+"/"+tinyName) ;
			//System.out.println(mp) ;
			Dimension d = large.getDimension();
			mp.put(SIZE_LARGE_HEIGHT, d.height);
			mp.put(SIZE_LARGE_WIDTH, d.width);
			return mp ;
			
		} catch (Exception e) {
			throw e;
		} finally {
			if (tiny != null)
				tiny.destroyImages();
			if (head != null)
				head.destroyImages();
			if (main != null)
				main.destroyImages();
			if (large != null)
				large.destroyImages();

			if (regulate != null)
				regulate.destroyImages();
			time.endFinally();
		}
		
	}
	
	/**
	 * 本地处理一组图像，调用Jmagick
	 * 
	 * @param source
	 * @param type
	 * @param namePrefix p: album, b: blog, e: event, g: groupalbum, s: schoolalbum, t:tribe
	 * @return
	 * @throws GroupException
	 * @throws Exception
	 */
	public Map localProcessHead(byte[] blob,int type, String namePrefix) throws GroupException, Exception {
		TimeCost time = TimeCost.begin(getClass());
		MagickImage large = null;
		MagickImage main = null;
		MagickImage head = null;
		MagickImage tiny = null;
		Group gp=Allocator.getInstance().allocateGroup(type) ;
		if(gp==null)
			throw new GroupException("no group found");
		List<Node> nd = gp.getNodes() ;
		if(nd ==null)
			return null ;
		//Calendar cl = Calendar.getInstance();
		String rootPath = OpiConstants.tempTmpPath ;
		String sfx = FileFactory.getInstance().getRandName();
		 
		/*String dir ="pic001/"+ FileFactory.getInstance().getDiv1(cl) + "/"
				+ FileFactory.getInstance().getDiv2(cl) + "/"
				+ FileFactory.getInstance().getDiv3(cl);*/
		// 使用短路径
		String dir = FileFactory.getInstance().getPathShort();
		
		String largeName =  namePrefix + this.SIZENAME_LARGE + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String mainName = namePrefix + this.SIZENAME_MAIN + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String headName = namePrefix + this.SIZENAME_HEAD + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String tinyName = namePrefix + this.SIZENAME_TINY + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		/*
		 * System.out.println(rootPath+largePath) ;
		 * System.out.println(rootPath+mainPath) ;
		 * System.out.println(rootPath+headPath) ;
		 * System.out.println(rootPath+tinyPath) ;
		 */
		try {
			time.endStep("get first photo");
			large = ImgPro.getFirstPhoto(blob, 600, 900);
			large.setFileName(rootPath + largeName);
			time.endStep("resize large");
			main = ImgPro.resizePhoto(large, 200, 300);
			main.setFileName(rootPath + mainName);
			time.endStep("resize main");
			head = ImgPro.resizePhoto(main, 100, 150);
			head.setFileName(rootPath + headName);
			time.endStep("resize head");
			tiny = ImgPro.resizePhoto(head, 50, 75);
			tiny.setFileName(rootPath + tinyName);
			time.endStep("resize tiny");
			ImageInfo out = new ImageInfo();
			out.setQuality(90);
			out.setInterlace(InterlaceType.PlaneInterlace); // interlace
			out.setColorspace(ColorspaceType.sRGBColorspace); // for TIFF jpeg
			out.setMagick("JPG"); // FIX somefile.bmp.jpg
			time.endStep("start get large stream");
			byte[] largeBt=large.imageToBlob(out);
			time.endStep("end get large stream");
			byte[] mainBt = main.imageToBlob(out);
			time.endStep("end get main stream");
			byte[] headBt = head.imageToBlob(out);
			time.endStep("end get head stream");
			byte[] tinyBt = tiny.imageToBlob(out);
			time.endStep("end get tiny stream");
			

			for(int i=0;i<nd.size();i++){
				String path = nd.get(i).getPath()+"/"+dir ;
				File f = new File(path) ;
				if(!f.exists())
					f.mkdirs() ;
				String largeFPath = path+"/"+largeName ;
				String mainFPath = path+"/"+mainName ;
				String headFPath = path+"/"+headName ;
				String tinyFPath = path+"/"+tinyName ;
				
				FileFactory.getInstance().writeFile(largeFPath, largeBt) ;
				time.endStep("end write large");
				FileFactory.getInstance().writeFile(mainFPath, mainBt) ;
				time.endStep("end write main");
				FileFactory.getInstance().writeFile(headFPath, headBt) ;
				time.endStep("end write head");
				FileFactory.getInstance().writeFile(tinyFPath, tinyBt) ;
				time.endStep("end write tiny");
				
			}
			Map mp =new HashMap() ;
			String dbPath = gp.getName()+"/"+dir ;
			mp.put(this.SIZENAME_LARGE, dbPath+"/"+largeName) ;
			mp.put(this.SIZENAME_MAIN, dbPath+"/"+mainName) ;
			mp.put(this.SIZENAME_HEAD, dbPath+"/"+headName) ;
			mp.put(this.SIZENAME_TINY, dbPath+"/"+tinyName) ;
			//System.out.println(mp) ;
			Dimension d = large.getDimension();
			mp.put(SIZE_LARGE_HEIGHT, d.height);
			mp.put(SIZE_LARGE_WIDTH, d.width);
			return mp ;
			
		} catch (Exception e) {
			throw e;
		} finally {
			if (tiny != null){
				tiny.destroyImages();
				tiny = null;
			}
			if (head != null){
				head.destroyImages();
				head = null;
			}
			if (main != null){
				main.destroyImages();
				main = null;
			}
			if (large != null){
				large.destroyImages();
				large = null;
			}
			time.endFinally();
		}
		
	}
	
	/**
	 * 第一张图比例通过后后续的图无需验证
	 */
	private static ResizeConfig RC_LARGE = new ResizeConfig(-1, 3, 600, -1, 600, 900);
	private static ResizeConfig RC_MAIN = new ResizeConfig(-1, -1, 200, -1, 200, 300);
	private static ResizeConfig RC_HEAD = new ResizeConfig(-1, -1, 100, -1, 100, 150);
	private static ResizeConfig RC_TINY = new ResizeConfig(-1, -1, 50, -1, 50, 75);
	
	/**
	 * 使用远程服务处理缩图
	 * 
	 * ResizeConfig(-1, 3, 780, -1, 600, 900)
	 * 比如这个
	 * -1 是说 宽/高 不限制
	 * 3 是说 高/宽 最大是3
	 * 780, -1 是说 竖着的图，最宽780，高度不限。
	 * 600, 900 是说 横着的图，最宽600，最高900。
	 * 如果比例限制不满足，直接就返回异常。
	 * 如果宽和高都有限制了，那都必须满足。
	 * 在满足所有条件的情况下，让图尽量的大。

	 * 
	 * @param blob
	 * @param type
	 * @param namePrefix
	 * @return
	 * @throws GroupException
	 * @throws Exception
	 */
	public Map processHead(byte[] blob,int type, String namePrefix) throws GroupException, ResizeFailedException, Exception {
		Image large = null;
		Image main = null;
		Image head = null;
		Image tiny = null;
		Group gp = Allocator.getInstance().allocateGroup(type) ;
		if(gp == null)
			throw new GroupException("no group found");
		List<Node> nd = gp.getNodes() ;
		if (nd == null) return null ;
		//Calendar cl = Calendar.getInstance();
		String rootPath = OpiConstants.tempTmpPath ;
		String sfx = FileFactory.getInstance().getRandName();
		 
		/*String dir ="pic001/"+ FileFactory.getInstance().getDiv1(cl) + "/"
				+ FileFactory.getInstance().getDiv2(cl) + "/"
				+ FileFactory.getInstance().getDiv3(cl);*/
		// 使用短路径
		String dir = FileFactory.getInstance().getPathShort();
		
		String largeName =  namePrefix + this.SIZENAME_LARGE + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String mainName = namePrefix + this.SIZENAME_MAIN + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String headName = namePrefix + this.SIZENAME_HEAD + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String tinyName = namePrefix + this.SIZENAME_TINY + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		/*
		 * System.out.println(rootPath+largePath) ;
		 * System.out.println(rootPath+mainPath) ;
		 * System.out.println(rootPath+headPath) ;
		 * System.out.println(rootPath+tinyPath) ;
		 */
		TimeCost time = TimeCost.begin(getClass());
		try {
			if (logger.isDebugEnabled()) {
				logger.debug("resize images using remote service");
			}
			large = ImageMagickdAdapter.getInstance().resize(blob, RC_LARGE);
			time.endStep("end resize large remote");
			main = ImageMagickdAdapter.getInstance().resize(large.data, RC_MAIN);
			time.endStep("end resize main remote");
			head = ImageMagickdAdapter.getInstance().resize(main.data, RC_HEAD);
			time.endStep("end resize head remote");
			tiny = ImageMagickdAdapter.getInstance().resize(head.data, RC_TINY);
			time.endStep("end resize tiny remote");
		} catch (Exception e) {
			if (e instanceof ResizeFailedException) {
				throw e;
			} else {
				logger.warn("remote resize failed, use local cpu");
				return processHead(ImgPro.getMagickImage(blob), type, namePrefix);
			}
		}

		for(int i = 0; i < nd.size(); i++){
			String path = nd.get(i).getPath()+"/"+dir ;
			File f = new File(path) ;
			if(!f.exists())
				f.mkdirs() ;
			String largeFPath = path+"/"+largeName ;
			String mainFPath = path+"/"+mainName ;
			String headFPath = path+"/"+headName ;
			String tinyFPath = path+"/"+tinyName ;
			FileFactory.getInstance().writeFile(largeFPath, large.data) ;
			time.endStep("end write large file in remote");
			FileFactory.getInstance().writeFile(mainFPath, main.data) ;
			time.endStep("end write main file in remote");
			FileFactory.getInstance().writeFile(headFPath, head.data) ;
			time.endStep("end write head file in remote");
			FileFactory.getInstance().writeFile(tinyFPath, tiny.data) ;
			time.endStep("end write tiny file in remote");
			
		}
		Map mp =new HashMap() ;
		String dbPath = gp.getName()+"/"+dir ;
		mp.put(this.SIZENAME_LARGE, dbPath+"/"+largeName) ;
		mp.put(this.SIZENAME_MAIN, dbPath+"/"+mainName) ;
		mp.put(this.SIZENAME_HEAD, dbPath+"/"+headName) ;
		mp.put(this.SIZENAME_TINY, dbPath+"/"+tinyName) ;
		mp.put(this.SIZE_LARGE_WIDTH, large.width);
		mp.put(this.SIZE_LARGE_HEIGHT, large.height);
		//System.out.println(mp) ;
		time.endFinally();
		return mp ;
	}
}

