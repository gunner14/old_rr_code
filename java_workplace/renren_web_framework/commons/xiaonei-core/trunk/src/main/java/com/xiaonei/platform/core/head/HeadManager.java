package com.xiaonei.platform.core.head;

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

import com.xiaonei.platform.core.file.FileFactory;
import com.xiaonei.platform.core.utility.ImgPro;
import com.xiaonei.platform.storage.Allocator;
import com.xiaonei.platform.storage.Constants;
import com.xiaonei.platform.storage.Group;
import com.xiaonei.platform.storage.Node;


public class HeadManager {
	public static String HEADNAME_LARGE = "large";

	public static String HEADNAME_MAIN = "main";

	public static String HEADNAME_HEAD = "head";

	public static String HEADNAME_TINY = "tiny";

	private static HeadManager instance;
	static {
		instance = new HeadManager();
	}

	public static HeadManager getInstance() {
		return instance;
	}

	public Map processHead(MagickImage source) throws Exception {
		MagickImage regulate = null;
		MagickImage large = null;
		MagickImage main = null;
		MagickImage head = null;
		MagickImage tiny = null;
		//Calendar cl = Calendar.getInstance();
		 Group gp=
		 Allocator.getInstance().allocateGroup(Constants.NODE_TYPE_HEAD) ;
		 List<Node> nd = gp.getNodes() ;
		 String rootPath =nd.get(0).getPath()+"/" ;
		//String rootPath = Globals.imgPath2 + Globals.storageContext
		//		+ Globals.photoContext;
		String sfx = FileFactory.getInstance().getRandName();
		String sfx_large = FileFactory.getInstance().getRandName();
			
		/* String dir
		 =FileFactory.getInstance().getDiv1(cl)+"/"+FileFactory.getInstance().getDiv2(cl)+"/"+FileFactory.getInstance().getDiv3(cl)
		 ;*/
		String dir = FileFactory.getInstance().getPathShort();
		//String dir = FileFactory.getInstance().getDiv1(cl) + "/"
		//		+ FileFactory.getInstance().getDiv2(cl) + "/"
		//		+ FileFactory.getInstance().getDiv3(cl);
		File f = new File(rootPath + dir);
		if (!f.exists()) {
			// System.out.println("---------create:"+f.getAbsolutePath()) ;
			f.mkdirs();
		}
		String largePath = dir + "/" + this.HEADNAME_LARGE + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx_large + ".jpg";
		String mainPath = dir + "/" + this.HEADNAME_MAIN + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String headPath = dir + "/" + this.HEADNAME_HEAD + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		String tinyPath = dir + "/" + this.HEADNAME_TINY + "_" + RandomStringUtils.randomAlphanumeric(4) + "_" + sfx + ".jpg";
		/*
		 * System.out.println(rootPath+largePath) ;
		 * System.out.println(rootPath+mainPath) ;
		 * System.out.println(rootPath+headPath) ;
		 * System.out.println(rootPath+tinyPath) ;
		 */
		try {
			regulate = ImgPro.regulate(source);
			large = ImgPro.resizePhotoStep(regulate, rootPath + largePath, 600,
					900);
			main = ImgPro.resizePhotoStep(large, rootPath + mainPath, 200, 300);
			head = ImgPro.resizePhotoStep(main, rootPath + headPath, 100, 150);
			tiny = ImgPro.resizePhotoStep(head, rootPath + tinyPath, 50, 75);
			ImageInfo out = new ImageInfo();
			out.setQuality(90);
			out.setInterlace(InterlaceType.PlaneInterlace); // interlace
			out.setColorspace(ColorspaceType.sRGBColorspace); // for TIFF jpeg
			out.setMagick("JPG"); // FIX somefile.bmp.jpg
			
			large.writeImage(out);
			main.writeImage(out);
			head.writeImage(out);
			tiny.writeImage(out);
			Map<String, String> mp = new HashMap();
			mp.put(this.HEADNAME_LARGE, nd.get(0).getGroup()+"/"+largePath);
			mp.put(this.HEADNAME_MAIN, nd.get(0).getGroup()+"/"+mainPath);
			mp.put(this.HEADNAME_HEAD, nd.get(0).getGroup()+"/"+headPath);
			mp.put(this.HEADNAME_TINY, nd.get(0).getGroup()+"/"+tinyPath);
			/* 不再监控上传空文件行为
			try {
				if(head.imageToBlob(new ImageInfo()).length < 1024) {
					HeadMonitorDAO.getInstance().insert(0, mp.get(HEADNAME_HEAD), source.imageToBlob(new ImageInfo()));
				}
			} catch(Exception e) {
				e.printStackTrace();
			}*/
			return mp;
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
	 * 如果确定任何源码和下发的jar包不在调用此方法，可以删除此方法，<br>
	 * 替代的是调用HeadUrlUtil中的getHeadFullUrl方法
	 * @param pattern
	 * @return
	 */
	public String getHeadFullUrl(String pattern) {
		return HeadUrlUtil.getHeadFullUrl(pattern);
	}

}
