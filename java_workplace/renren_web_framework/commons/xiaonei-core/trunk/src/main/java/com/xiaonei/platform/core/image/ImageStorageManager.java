package com.xiaonei.platform.core.image;

import java.io.File;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import magick.ImageInfo;
import magick.MagickException;
import magick.MagickImage;

import com.xiaonei.platform.core.file.FileFactory;
import com.xiaonei.platform.storage.Allocator;
import com.xiaonei.platform.storage.Constants;
import com.xiaonei.platform.storage.Group;
import com.xiaonei.platform.storage.Node;

public class ImageStorageManager {
	private static ImageStorageManager instance = new ImageStorageManager();

	public static ImageStorageManager getInstance() {
		return instance;
	}

	private ImageStorageManager() {
	}

	public ImageStorageModel processPhoto(int userId, byte[] image) {
		return process(userId, image, Constants.NODE_TYPE_NORMAL);
	}

	public ImageStorageModel processPortrait(int userId, byte[] image) {
		return process(userId, image, Constants.NODE_TYPE_HEAD);
	}

	private ImageStorageModel process(int userId, byte[] image, int type) {
		MagickImage source = null;
		MagickImage large = null;
		MagickImage main = null;
		MagickImage head = null;
		MagickImage tiny = null;
		try {
			Group group = Allocator.getInstance().allocateGroup(type);
			if (group == null) {
				System.out.println("ERROR: Can not get writable storage type "
						+ type);
				return new ImageStorageModel(
						ImageStorageModel.STATUS_SYSTEM_BUSY);
			}
			List<Node> node = group.getNodes();
			if (node == null || node.size() == 0) {
				System.out.println("ERROR: Can not get writable storage type "
						+ type);
				return new ImageStorageModel(
						ImageStorageModel.STATUS_SYSTEM_BUSY);
			}

			source = new MagickImage(new ImageInfo(), image);
			String userMark = "" + userId;
			String imageMark = source.getImageAttribute("comment");
			if (imageMark != null && !userMark.equals(imageMark)) {
				return new ImageStorageModel(
						ImageStorageModel.STATUS_ALREADY_EXIST);
			}
			source.profileImage("*", null);
			source.setImageAttribute("comment", userMark);

			large = processImage(source, 600);
			main = processImage(large, 200);
			head = processImage(main, 100);
			tiny = processImage(head, 75);

			String rootPath = node.get(0).getPath();
			String location = "";
			if (type == Constants.NODE_TYPE_NORMAL) {
				location = "/album";
			}
			Calendar cl = Calendar.getInstance();
			String dirs = location + "/"
					+ FileFactory.getInstance().getDiv1(cl) + "/"
					+ FileFactory.getInstance().getDiv2(cl) + "/"
					+ FileFactory.getInstance().getDiv3(cl) + "/";
			String filename = "_" + FileFactory.getInstance().getRandName()
					+ ".jpg";
			String largePath = dirs + ImageStorageModel.KEY_SIZE_LARGE
					+ filename;
			String mainPath = dirs + ImageStorageModel.KEY_SIZE_MAIN + filename;
			String headPath = dirs + ImageStorageModel.KEY_SIZE_HEAD + filename;
			String tinyPath = dirs + ImageStorageModel.KEY_SIZE_TINY + filename;
			File file = new File(rootPath + dirs);
			if (!file.exists())
				file.mkdirs();

			ImageInfo out = new ImageInfo();
			out.setQuality(90);
			FileFactory.getInstance().writeFile(rootPath + largePath,
					large.imageToBlob(out));
			FileFactory.getInstance().writeFile(rootPath + mainPath,
					main.imageToBlob(out));
			FileFactory.getInstance().writeFile(rootPath + headPath,
					head.imageToBlob(out));
			FileFactory.getInstance().writeFile(rootPath + tinyPath,
					tiny.imageToBlob(out));

			Map<String, String> map = new HashMap<String, String>();
			map.put(ImageStorageModel.KEY_SIZE_LARGE, group.getName()
					+ largePath);
			map
					.put(ImageStorageModel.KEY_SIZE_MAIN, group.getName()
							+ mainPath);
			map
					.put(ImageStorageModel.KEY_SIZE_HEAD, group.getName()
							+ headPath);
			map
					.put(ImageStorageModel.KEY_SIZE_TINY, group.getName()
							+ tinyPath);
			return new ImageStorageModel(ImageStorageModel.STATUS_OK, map);
		} catch (Exception e) {
			e.printStackTrace();
			return new ImageStorageModel(ImageStorageModel.STATUS_FAILED);
		} finally {
			if (large != null)
				large.destroyImages();
			if (main != null)
				main.destroyImages();
			if (head != null)
				head.destroyImages();
			if (tiny != null)
				tiny.destroyImages();
		}

	}

	private MagickImage processImage(MagickImage image, int maxSize)
			throws MagickException {
		int width = image.getDimension().width;
		int height = image.getDimension().height;
		if (width > height) {
			if (width > maxSize)
				return image.scaleImage(maxSize, maxSize * height / width);
		} else {
			if (height > maxSize)
				return image.scaleImage(maxSize * width / height, maxSize);
		}
		return image;
	}
}
