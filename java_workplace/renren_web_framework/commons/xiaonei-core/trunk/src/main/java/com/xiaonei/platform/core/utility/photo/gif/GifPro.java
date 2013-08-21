package com.xiaonei.platform.core.utility.photo.gif;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.UnsupportedEncodingException;


/**
 * Gif文件处理
 * 使用构造器加载一次图像，每次处理使用解析好的数据进行缩放
 * 
 * <pre>
 * 对所有的gif文件添加注释，目前暂为校内+userId
 * 0x21
 * 0xfe
 * datalen
 * data0
 * data1
 * data2
 * ..
 * datan
 * 0x00
 * </pre>
 */
public class GifPro {

	private GifDecoder decoder;
	private boolean loadOk = false;
	private transient byte[] blob;
	private int statusCode;
	
	/**
	 * 
	 * @param blob 需要处理的数据
	 */
	public GifPro(byte[] blob) {
		decoder = new GifDecoder();
		this.blob = blob;
		statusCode = decoder.read(new ByteArrayInputStream(blob)); 
		if (statusCode == 0) {
			loadOk = true;
		}
	}
	
	public int getStatusCode() {
		return statusCode;
	}

	/**
	 * 转换文件尺寸<br/>
	 * 
	 * <font color="red">此包对单帧gif支持有问题，读取时可能出错，如果返回null，请使用JMagick API尝试读取并且转换为jpg格式文件</font>
	 * 
	 * 
	 * 
	 * 
	 * @param maxWidth 限定宽度
	 * @param maxHeight 限定高度
	 * @param regulate 是否按照1.5:1的yx比裁切
	 * @param comment 注释
	 * @return 处理成功返回gif内容，失败返回null
	 */
	public byte[] process(int maxWidth, int maxHeight, boolean regulate, String comment)  {

		if (loadOk) {
			// 缩放后的文件
			ByteArrayOutputStream bos = new ByteArrayOutputStream(blob.length);

			AnimatedGifEncoder e = new AnimatedGifEncoder();
			e.start(bos);
			e.setRepeat(decoder.getLoopCount());
			e.setDispose(decoder.dispose);
			/*if (decoder.transparency) {
				e.setTransparent(new Color(decoder.act[decoder.transIndex]));
			}*/
			
			// FIXME 标记检查和添加标记
			try {
				if (decoder.comment != null) {
					System.out.println(decoder.comment.length);
					System.out.println(new String(decoder.comment, "iso-8859-1"));
				}
			} catch (UnsupportedEncodingException e1) {
				e1.printStackTrace();
			}
			
			if (comment != null) e.setComment(comment.getBytes());
			
			// 调整比例后从缩图切下的起点，宽和高
			int x = 0, y = 0, width = decoder.width, height = decoder.height, origHeight = decoder.height, resizeHeight = decoder.height;
			// 是否需要裁切
			boolean crop = false;
			// 从Y方向裁切的尺寸(原始尺寸)
			int yDiff = 0;
			// 是否需要改尺寸
			boolean resize = false;

			// 裁切
			if (regulate) {
				if ((height + 0.0) / width > 1.5) {
					// 高宽比大于1.5的部分都要切掉
					int newHeight = (int) Math.round(width * 1.5);
					yDiff = height - newHeight;
					crop = true;
					height = newHeight; 
				}
			}
			
			// 尺寸，不考虑裁切，按照裁切后的尺寸计算(如果裁切)，后面处理时会在y方向加上适当偏移
			if (width > maxWidth || height > maxHeight) {
				if (width > 0 && height > 0) {
					resize = true;
					if (height / (float)width > maxHeight / (float)maxWidth) {
						width = (int) (width * maxHeight / (float) height);
						height = maxHeight;
					} else {
						height = (int) (height * maxWidth / (float) width);
						width = maxWidth;
					}
					resizeHeight = height;
				}
			}
			
			// 缩放和裁切兼有，计算实际的开始位置(y)
			if (resize && yDiff > 0) {
				// 新的高度和不裁切时新高度的差
				int diff = (height * yDiff) / (origHeight - yDiff);
				resizeHeight = height + diff;
				y = diff / 2;
			}

			/*System.out.println(String.format("图像尺寸：%dx%d，是否裁切：%s，是否缩放：%s，缩放后尺寸：%dx%d，缩放取尺寸：%d,%d,%dx%d",
				d.width, d.height,
				crop, resize,
				width, height,
				x, y,
				width, resizeHeight)
			);*/
			
			// 逐祯缩放
			int n = decoder.getFrameCount();
			for (int i = 0; i < n; i++) {
				BufferedImage frame = decoder.getFrame(i);
				
				if (resize) {
					Image img = frame.getScaledInstance(width, resizeHeight, Image.SCALE_SMOOTH);
					frame = new BufferedImage(width, resizeHeight, BufferedImage.TYPE_3BYTE_BGR);
					Graphics g = frame.getGraphics();
					g.setColor(Color.WHITE);
					g.fillRect(0, 0, width, resizeHeight); // 白背景色
					g.drawImage(img, 0, 0, null);
					g.dispose();
				} else {
					// 添加白色背景
					BufferedImage tmp = new BufferedImage(width, resizeHeight, BufferedImage.TYPE_3BYTE_BGR);
					Graphics g = tmp.getGraphics();
					g.setColor(Color.WHITE);
					g.fillRect(0, 0, width, resizeHeight); // 白背景色
					g.drawImage(frame, 0, 0, null);
					g.dispose();
					frame = tmp;
				}
				// 处理下比例
				if (crop) frame = frame.getSubimage(x, y, width, height);
				
				e.addFrame(frame) ;
				e.setDelay(decoder.getDelay(i));
			}
			e.finish();
			if (n > 0) return bos.toByteArray();
		}
		return null;
	}

}
