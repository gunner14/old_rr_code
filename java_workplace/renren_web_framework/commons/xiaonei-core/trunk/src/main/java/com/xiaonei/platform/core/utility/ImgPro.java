package com.xiaonei.platform.core.utility;

//import java.io.File;
//import java.io.IOException;

import java.awt.Dimension;
import java.awt.Rectangle;
import java.io.File;
import java.io.FileInputStream;

import magick.CompositeOperator;
import magick.ImageInfo;
import magick.MagickException;
import magick.MagickImage;

/**
 * 
 * ImgPro.java
 * 
 * @author drizzt drizzt.wang@gmail.com 2006-7-26 下午10:52:31
 */
public class ImgPro {
	public static MagickImage regulate(MagickImage source, String destPathName)
			throws MagickException {
		int width = (int) source.getDimension().getWidth();
		int height = (int) source.getDimension().getHeight();
		if ((height + 0.0) / width > 1.5) {
			MagickImage image = null;
			MagickImage scaled = null;
			try {
				int newHeight = (int) Math.round(width * 1.5);
				Rectangle rect = new Rectangle(0, (height - newHeight) / 2,
						width, newHeight);
				ImageInfo info = null;
				info = new ImageInfo();

				scaled = source.cropImage(rect);
				scaled.setFileName(destPathName);
				// scaled.writeImage(info);
				// System.out.println("--------------regulate.scaled:width:"+width+"|height:"+height)
				// ;
				return scaled;
			} finally {
				if (source != null) {
					source.destroyImages();
				}

			}
		} else {
			// System.out.println("--------------regulate.source:width:"+width+"|height:"+height)
			// ;
			return source;
		}
	}

	/**
	 * 按照1：1.5宽高比裁切，如果进行了裁切，则原图对象被销毁
	 * 
	 * @param source
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulate(MagickImage source)
			throws MagickException {
		return regulate(source, true);
	}
	
	/**
	 * 按照1:1.5宽高比裁切，可以指定是否销毁原始对象
	 * 
	 * @param source
	 * @param destroySource
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulate(MagickImage source, boolean destroySource)
			throws MagickException {
		int width = (int) source.getDimension().getWidth();
		int height = (int) source.getDimension().getHeight();
		if ((height + 0.0) / width > 1.5) {
			MagickImage image = null;
			MagickImage scaled = null;
			try {
				int newHeight = (int) Math.round(width * 1.5);
				Rectangle rect = new Rectangle(0, (height - newHeight) / 2,
						width, newHeight);
				ImageInfo info = null;
				info = new ImageInfo();

				scaled = source.cropImage(rect);
				// scaled.setFileName(destPathName);
				// scaled.writeImage(info);
				// System.out.println("--------------regulate.scaled:width:"+width+"|height:"+height)
				// ;
				return scaled;
			} finally {
				if (source != null && destroySource) {
					source.destroyImages();
				}

			}
		} else {
			// System.out.println("--------------regulate.source:width:"+width+"|height:"+height)
			// ;
			return source;
		}
	}
	
	/**
	 * 给尺寸不足的边补白，source会被destroy
	 * 
	 * @param source
	 * @param minWidth
	 * @param minHeight
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulateWithMinLimit(MagickImage source, int minWidth, int minHeight)
		throws MagickException {
		return regulateWithMinLimit(source, minWidth, minHeight, true);
	}
	
	/**
	 * 给尺寸不足的边补白
	 * 
	 * @param source
	 * @param minWidth
	 * @param minHeight
	 * @param destroyImage
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulateWithMinLimit(MagickImage source, int minWidth, int minHeight, boolean destroyImage)
			throws MagickException {
		int width = (int) source.getDimension().getWidth();
		int height = (int) source.getDimension().getHeight();
		
		if (width < minWidth && height < minHeight) {
			MagickImage img = null;
			byte[] pixels = new byte[minWidth * minHeight * 4];
	        for (int i = 0; i < minWidth * minHeight; i++) {
	            pixels[4 * i] = (byte) 255;
	            pixels[4 * i + 1] = (byte) 255;
                pixels[4 * i + 2] = (byte) 255;
                pixels[4 * i + 3] = (byte) 0;
            }
            img = new MagickImage();
            img.constituteImage(minWidth, minHeight, "RGBA", pixels);
            img.compositeImage(CompositeOperator.AddCompositeOp, source, 
            		(minWidth - width) / 2, (minHeight - height) / 2);
            if (destroyImage) source.destroyImages();
            return img;
		} else if (width < minWidth) {
			return regulateWithRatio(source, minWidth, height, destroyImage);
		} else if (height < minHeight) {
			return regulateWithRatio(source, width, minHeight, destroyImage);
		}
		

		return source;
	}
	
	/**
	 * 按照限定宽度缩放，超过1.5高的部分会被切掉，如果宽度不足，不会按照该宽度的1.5倍高度裁切
	 * 而是限定宽度的1.5倍，这是和{@link #regulate(MagickImage)}不同的地方
	 * 
	 * @param source
	 * @param maxWidth
	 * @param maxHeight
	 * @param destroySource 是否销毁source对象
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulateWithMaxWidth(MagickImage source, int maxWidth, boolean destroySource) throws MagickException {
		int width = (int) source.getDimension().getWidth();
		int height = (int) source.getDimension().getHeight();
		

		if (height / width > 1.5) {
			if (width < maxWidth) {
				// 宽度比限定宽度小
				int y = (int) ((height - maxWidth * 1.5) / 2);
				int h = (int) (maxWidth * 1.5);
				if (h < height) {
					// 如果高度超过限定宽度的1.5倍
					Rectangle rect = new Rectangle(0, y, width, h);
					if (destroySource) source.destroyImages();
					return source.cropImage(rect);
				} else {
					return source;
				}
			}
		}
		MagickImage img = null;
		try{
			// 按比例处理
			img = regulate(source, false);
			return resizePhotoStep(img, img.getFileName(), maxWidth, (int)(maxWidth * 1.5));
		} finally {
			if (destroySource) source.destroyImages();
		}
	}
	
	/**
	 * 按照比例对尺寸不足的边补白，销毁source
	 * 
	 * @param source
	 * @param ratioWidth
	 * @param ratioHeight
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulateWithRatio(MagickImage source, int ratioWidth, int ratioHeight)
		throws MagickException {
		return regulateWithRatio(source, ratioWidth, ratioHeight, true);
	}
	
	/**
	 * 按照比例对尺寸不足的边补白
	 * 
	 * @param source
	 * @param ratioWidth
	 * @param ratioHeight
	 * @param destroySource
	 * @return
	 * @throws MagickException
	 */
	public static MagickImage regulateWithRatio(MagickImage source, int ratioWidth, int ratioHeight, boolean destroySource)
			throws MagickException {
		int width = (int) source.getDimension().getWidth();
		int height = (int) source.getDimension().getHeight();
		
		boolean okay = true;
		int newh = height, neww = width;
		int x = 0, y = 0;
		System.out.println(String.format("%d, %d", width, height));
		if ((width / (float)height) > (ratioWidth / (float)ratioHeight)) {
			// 比例不协调，宽
			newh = width * ratioHeight / ratioWidth;
			y = (newh - height) / 2;
			okay = false;
			System.out.println("w>h");
		} else if ((width / (float)height < ratioWidth / (float)ratioHeight)) {
			// 比例不协调，高
			neww = height * ratioWidth / ratioHeight;
			x = (neww - width) / 2;
			okay = false;
			System.out.println("h>w");
		}
		MagickImage img = null;
		if (!okay) {
			System.out.println(String.format("%d, %d", neww, newh));
			byte[] pixels = new byte[neww * newh * 4];
            for (int i = 0; i < neww * newh; i++) {
                pixels[4 * i] = (byte) 255;
                pixels[4 * i + 1] = (byte) 255;
                pixels[4 * i + 2] = (byte) 255;
                pixels[4 * i + 3] = (byte) 0;
            }
            img = new MagickImage();
            img.constituteImage(neww, newh, "RGBA", pixels);
            img.compositeImage(CompositeOperator.AddCompositeOp, source, x, y);
            if (destroySource) source.destroyImages();
            return img;
			
		} 
		
		return source;
	}
	
	public static void main(String[] args) throws Exception {
		File f = new File("d:/pic/head/901.jpg");
		byte[] blob = new byte[(int)f.length()];
		FileInputStream fis = new FileInputStream(f);
		fis.read(blob);
		MagickImage image = getMagickImage(blob);
		//image = regulateWithRatio(image, 3, 4);
		//image = regulateWithMinLimit(image, 50, 50);
		image = regulateWithMaxWidth(image, 100, false);
		image.setFileName("d:/pic/test2/dd3d.jpg");
		image.writeImage(new ImageInfo());
	}

	public static MagickImage resizePhotoStep(MagickImage source,
			String destPathName, int maxWidth, int maxHeight)
			throws MagickException {
		int width = 0;
		int height = 0;
		boolean change = true;
		width = (int) source.getDimension().getWidth();
		height = (int) source.getDimension().getHeight();
		if (maxWidth > width && maxHeight > height) {
			change = false;
		} else {
			if (width > 0 && height > 0) {
				if (height / width > maxHeight / maxWidth) {
					width = width * maxHeight / height;
					height = maxHeight;
				} else {
					height = height * maxWidth / width;
					width = maxWidth;
				}
			}
		}

		MagickImage scaled = null;

		// if (change) {
		scaled = source.zoomImage(width, height);

		// } else {
		// scaled = source;
		// }
		// System.out.println("------------change:"+change+"|width:"+width+"|height:"+height)
		// ;
		scaled.setFileName(destPathName);
		// scaled.writeImage(info);
		return scaled;

	}

	public static MagickImage setcomment(MagickImage image, String comment)
			throws MagickException {
		// ImageInfo info = null;
		// info = new ImageInfo();
		image.profileImage("*", null);
		image.setImageAttribute("comment", comment);
		// image.writeImage(info);
		return image;
	}

	public static void convertBmpToJpg(String srcPathName, String destPathName)
			throws MagickException {
		MagickImage image = null;
		try {
			ImageInfo info = null;
			info = new ImageInfo();
			image = new MagickImage(new ImageInfo(srcPathName + "[0]"));
			image.setFileName(destPathName);
			image.writeImage(info);
		} finally {
			if (image != null) {
				image.destroyImages();
			}
		}
	}

	public static void resize(String srcPathName, String destPathName,
			int maxWidth, int maxHeight) throws MagickException {
		int width = 0;
		int height = 0;
		boolean change = true;
		width = getWidth(srcPathName);
		height = getHeight(srcPathName);
		// if (maxWidth > width && maxHeight > height) {
		// change = false;
		// } else {
		if (width > 0 && height > 0) {
			if (height / width > maxHeight / maxWidth) {
				width = width * maxHeight / height;
				height = maxHeight;
			} else {
				height = height * maxWidth / width;
				width = maxWidth;
			}
		}
		// }

		MagickImage image = null;
		MagickImage scaled = null;
		try {
			ImageInfo info = null;
			info = new ImageInfo();
			image = new MagickImage(new ImageInfo(srcPathName + "[0]"));
			if (change) {

				scaled = image.scaleImage(width, height);
			} else {
				scaled = image;
			}
			scaled.setFileName(destPathName);
			scaled.writeImage(info);

		} finally {
			if (image != null) {
				image.destroyImages();
			}
			if (scaled != null) {
				scaled.destroyImages();
			}
		}
	}

	public static void resizePhoto(String srcPathName, String destPathName,
			int maxWidth, int maxHeight) throws MagickException {
		int width = 0;
		int height = 0;
		boolean change = true;
		width = getWidth(srcPathName);
		height = getHeight(srcPathName);
		if (maxWidth > width && maxHeight > height) {
			change = false;
		} else {
			if (width > 0 && height > 0) {
				if (height / width > maxHeight / maxWidth) {
					width = width * maxHeight / height;
					height = maxHeight;
				} else {
					height = height * maxWidth / width;
					width = maxWidth;
				}
			}
		}

		MagickImage image = null;
		MagickImage scaled = null;
		try {
			ImageInfo info = null;
			info = new ImageInfo();
			image = new MagickImage(new ImageInfo(srcPathName + "[0]"));
			if (change) {
				scaled = image.scaleImage(width, height);
			} else {
				scaled = image;
			}
			scaled.setFileName(destPathName);
			scaled.writeImage(info);
		} finally {
			if (image != null) {
				image.destroyImages();
			}
			if (scaled != null) {
				scaled.destroyImages();
			}
		}
	}

	public static void setcomment(String srcPathName, String comment)
			throws MagickException {
		ImageInfo info = null;
		info = new ImageInfo();
		MagickImage image = null;
		try {
			image = new MagickImage(new ImageInfo(srcPathName + "[0]"));
			image.profileImage("*", null);
			image.setImageAttribute("comment", comment);
			image.writeImage(info);
		} finally {
			if (image != null) {
				image.destroyImages();
			}
		}
	}

	public static boolean checkcomment(MagickImage source, String comment) {

		try {
			String cmt = "";
			cmt = source.getImageAttribute("comment");
			if (cmt != null) {
				if (cmt.startsWith(ImgPro.getCommentProfile()) && !comment.equals(cmt)) {
					return false;
				}
			}
		}catch(Exception e) {
			
		}
		return true;

	}

	public static boolean checkcomment(String srcPathName, String comment)
			throws MagickException {
		MagickImage image = null;
		try {
			image = new MagickImage(new ImageInfo(srcPathName + "[0]"));
			String cmt = "";
			cmt = image.getImageAttribute("comment");
			if (cmt != null) {
				if (cmt.startsWith(ImgPro.getCommentProfile()) && !comment.equals(cmt)) {
					return false;
				}
			}
			return true;
		} finally {
			if (image != null) {
				image.destroyImages();
			}
		}
	}

	public static void mask(String logoPath, String srcPathName,
			String destPathName, int location, int scale)
			throws MagickException {
		int width = getWidth(srcPathName);
		int height = getHeight(srcPathName);
		int x = 0, y = 0;
		int w, h;
		w = scale * 70 / 100;
		h = scale * 65 / 100;
		boolean lc = true;
		if (width < height) {
			switch (location) {
			case 0:
				lc = false;
				break;
			case 1:
				x = width / 4 - w;
				y = height / 8 - h / 2;
				break;
			case 2:
				x = width / 2 - w;
				y = height / 8 - h / 2;
				break;
			case 3:
				x = width * 3 / 4 - w;
				y = height / 8 - h / 2;
				break;
			case 4:
				x = width / 4 - w;
				y = height / 2 - h / 2;
				break;
			case 5:
				x = width / 2 - w;
				y = height / 2 - h / 2;
				break;
			case 6:
				x = width * 3 / 4 - w;
				y = height / 2 - h / 2;
				break;
			case 7:
				x = width / 4 - w;
				y = height * 7 / 8 - h / 2;
				break;
			case 8:
				x = width / 2 - w;
				y = height * 7 / 8 - h / 2;
				break;
			case 9:
				x = width * 3 / 4 - w;
				y = height * 7 / 8 - h / 2;
				break;
			}
		} else {
			switch (location) {
			case 0:
				lc = false;
				break;
			case 1:
				x = width / 7 - w;
				y = height / 6 - h / 2;
				break;
			case 2:
				x = width / 2 - w;
				y = height / 6 - h / 2;
				break;
			case 3:
				x = width * 6 / 7 - w;
				y = height / 6 - h / 2;
				break;
			case 4:
				x = width / 7 - w;
				y = height / 2 - h / 2;
				break;
			case 5:
				x = width / 2 - w;
				y = height / 2 - h / 2;
				break;
			case 6:
				x = width * 6 / 7 - w;
				y = height / 2 - h / 2;
				break;
			case 7:
				x = width / 7 - w;
				y = height * 5 / 6 - h / 2;
				break;
			case 8:
				x = width / 2 - w;
				y = height * 5 / 6 - h / 2;
				break;
			case 9:
				x = width * 6 / 7 - w;
				y = height * 5 / 6 - h / 2;
				break;
			}
		}
		if (x < 10) {
			x = 10;
		}
		if (x + w * 2 + 10 > width) {
			x = width - w * 2 - 10;
		}
		if (y < 10) {
			y = 10;
		}
		if (y + h + 10 > height) {
			y = height - h - 10;
		}
		if (lc) {
			ImageInfo info = new ImageInfo();
			MagickImage image = null;
			MagickImage mask = null;
			MagickImage dest = null;
			try {
				image = new MagickImage(new ImageInfo(srcPathName + "[0]"));
				mask = new MagickImage(new ImageInfo(logoPath));
				image.setFileName(destPathName);
				image.writeImage(info);
				dest = new MagickImage(new ImageInfo(destPathName));
				dest.compositeImage(CompositeOperator.AtopCompositeOp, mask, x,
						y);
				dest.setFileName(destPathName);
				dest.writeImage(info);
			} finally {
				if (image != null) {
					image.destroyImages();
				}
				if (mask != null) {
					mask.destroyImages();
				}
				if (dest != null) {
					dest.destroyImages();
				}
			}
		}
	}

	public static MagickImage getMagickImage(String src) throws MagickException {

		ImageInfo info = new ImageInfo(src + "[0]");
		return new MagickImage(info);

	}

	public static MagickImage getMagickImage(byte[] bt) throws MagickException {

		ImageInfo info = new ImageInfo("[0]");
		return new MagickImage(info, bt);

	}

	public static int getWidth(String src) throws MagickException {
		MagickImage magImage = null;
		try {
			ImageInfo info = new ImageInfo(src + "[0]");
			magImage = new MagickImage(info);
			Dimension imageDim = magImage.getDimension();
			return imageDim.width;
		} finally {
			if (magImage != null) {
				magImage.destroyImages();
			}
		}
	}

	public static int getHeight(String src) throws MagickException {
		MagickImage magImage = null;
		try {
			ImageInfo info = new ImageInfo(src + "[0]");
			magImage = new MagickImage(info);
			Dimension imageDim = magImage.getDimension();
			return imageDim.height;
		} finally {
			if (magImage != null) {
				magImage.destroyImages();
			}
		}
	}

	public static boolean shrinkWidth(String srcPathName, String destPathName,
			int maxWidth) throws MagickException {
		int width = getWidth(srcPathName);
		int height = getHeight(srcPathName);
		if (width <= maxWidth) {
			return false;
		}
		int y = height * maxWidth / width;
		MagickImage image = null;
		MagickImage scaled = null;
		try {
			ImageInfo info = new ImageInfo();
			image = new MagickImage(new ImageInfo(srcPathName));
			scaled = image.scaleImage(maxWidth, y);
			scaled.setFileName(destPathName);
			scaled.writeImage(info);
			return true;
		} finally {
			if (image != null) {
				image.destroyImages();
			}
			if (scaled != null) {
				scaled.destroyImages();
			}
		}
	}
	/**
	 * 此方法用来解决checkcomment里开心和校内使用不同名称的问题
	 * @return 'xiaonei' or 'renren'
	 */
	private static String getCommentProfile() {
		if (com.xiaonei.platform.core.opt.OpiConstants.isXiaonei()) {
			return "xiaonei";
		} else {
			return "renren";
		}
	}
	
	public static MagickImage resizePhoto(MagickImage source, int maxWidth, int maxHeight) throws MagickException{
        int width = (int) source.getDimension().getWidth();
        int height = (int) source.getDimension().getHeight();
        if (maxWidth > width && maxHeight > height)//no need to resize
                return source; 
        if (height / width > maxHeight / maxWidth) {
                width = width * maxHeight / height;
                height = maxHeight;
        } else {
                height = height * maxWidth / width;
                width = maxWidth; 
        }       
        MagickImage scaled = null;
        scaled = source.zoomImage(width, height);
        return scaled;
	}
	
	
	public static  MagickImage getFirstPhoto(byte[] blob, int maxWidth, int maxHeight) throws MagickException{
         ImageInfo info = new ImageInfo();
         info.setPing(true);
         MagickImage image = new MagickImage(info, blob);
         int columns = 0;
         int rows = 0;
         int width = (int) image.getDimension().getWidth();
         int height = (int) image.getDimension().getHeight();
         if (height / width > maxHeight / maxWidth) {
                 columns = width * maxHeight / height;
                 rows = maxHeight;
         } else {
                 rows = height * maxWidth / width;
                 columns = maxWidth;
         }
         image.destroyImages();
         MagickImage source = null;
         info.setPing(false);
         info.setMagick("JPG");
         if(width/2 < columns){//no need to resize
                 source = new MagickImage(info, blob);
                 if(columns >= width)
                         return source;
                 else{
                         MagickImage scaled = source.zoomImage(columns, rows);
                         source.destroyImages();
                         return scaled;
                 }
         }else{
                 info.setSize(columns+"x"+rows+"!");
                 source = new MagickImage(info, blob);
                 MagickImage scaled = source.zoomImage(columns, rows);
                 source.destroyImages();
                 return scaled;
         }

	 }
	
}
