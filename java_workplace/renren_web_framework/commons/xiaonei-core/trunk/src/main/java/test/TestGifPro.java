package test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;

import com.xiaonei.platform.core.utility.photo.gif.GifPro;

public class TestGifPro {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		
		
		
		File f = new File("d:/pic/gif/static");
		File[] list = f.listFiles();
		
		int count = 0;
		for (File ff : list) {
			
			byte[] blob = new byte[(int)ff.length()];
			InputStream is = new FileInputStream(ff);
			is.read(blob);
			
			FileOutputStream fos = new FileOutputStream("d:/test2/"+count+".gif");
			
			GifPro gifPro = new GifPro(blob);
			System.out.println(gifPro.getStatusCode());
			
			byte[] gif = new byte[3]; //gifPro.process(500, 500, true);
			
			
			if (gif != null)
				fos.write(gif);
			
			System.out.println(String.format("%d %s", count, ff.getAbsoluteFile()));
			
			count++;
		}

	}

}
