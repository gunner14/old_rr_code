package com.xiaonei.platform.component.tools;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileFilter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class CompressHtml
{
	public static void main(String[] args)
	{
		if (args.length != 1)
		{
			System.out.println("args:\t" + args);
			System.out
					.println("Usage: /opt/j2sdk/bin/java -cp .:/opt/staging/www/WEB-INF/lib/xiaonei-component-0.1.jar com.xiaonei.platform.component.tools.CompressHtml path");
			System.exit(1);
			return;
		}
		fileFilter(new File(args[0]));
	}

	public static void fileFilter(File file)
	{
		file.listFiles(new FileFilter()
		{
			public boolean accept(File f)
			{
				if (f.isDirectory())
				{
					fileFilter(f);
				} else
				{
					String name = f.getName();

					if (f.isFile()
							&& (name.endsWith(".inc") || name.endsWith(".jsp")))
					{
						String fileName = f.getAbsolutePath();
						System.out.println("Compressing .... " + fileName);

						List<String> lineList = readFileToList(f);
						if (lineList != null)
						{// 执行压缩

							List<String> compressedList = compress(lineList);
							if (compressedList != null)
							{// 还有内容,则写入tmp文件，然后重命名
								File tmp = new File(fileName + ".tmp");
								if (write(tmp, compressedList))
								{// 写入成功
									f.renameTo(new File(fileName + ".bak"));
									tmp.renameTo(new File(fileName));
								} else
								{// 写入失败

								}
							} else
							{// 没有则保持不变

							}
						}
					}
				}
				return false;
			}
		});
	}

	private final static String lineSplit = "\n";

	/**
	 * 压缩文件
	 * 
	 * @param sourceList
	 * @return
	 */
	private static List<String> compress(List<String> sourceList)
	{
		List<String> compressedList = new ArrayList<String>(50);
		// 去掉<-- -->和<%-- --%>
		// 去掉所有c:set,
		for (int i = 0; i < sourceList.size(); i++)
		{
			String curent = sourceList.get(i);
			if (i + 1 < sourceList.size())
			{
				String next = sourceList.get(i + 1);
				if (matchEnd(curent) || matchBegin(next))
				{
					compressedList.add(curent);
					continue;
				}
			}

			compressedList.add(curent + lineSplit);
		}

		return compressedList;
	}

	private static boolean matchEnd(String curent)
	{
		if (curent.endsWith("%>"))
			return true;
		if (curent.endsWith("</c:if>") || curent.endsWith("</c:choose>")
				|| curent.endsWith("</c:when>") || curent.endsWith("</c:set>")
				|| curent.endsWith("</c:otherwise>")
				|| curent.endsWith("</c:forEach>")
				|| curent.endsWith("<c:otherwise>")
				|| curent.endsWith("<c:choose>"))
		{
			return true;
		}

		return false;
	}

	private static boolean matchBegin(String next)
	{
		if (next.startsWith("<%"))
			return true;

		if (next.startsWith("<c:if") || next.startsWith("<c:choose")
				|| next.startsWith("<c:when") || next.startsWith("<c:set")
				|| next.startsWith("<c:otherwise>")
				|| next.startsWith("<c:forEach") || next.startsWith("</c:if>")
				|| next.startsWith("</c:choose>")
				|| next.startsWith("</c:when>") || next.startsWith("</c:set>")
				|| next.startsWith("</c:otherwise>")
				|| next.startsWith("</c:forEach>"))
			return true;

		return false;
	}

	/**
	 * 写入临时文件
	 * 
	 * @param tmpFile
	 * @return
	 */
	private static boolean write(File tmpFile, List<String> compressedList)
	{
		BufferedWriter bw = null;
		try
		{
			bw = new BufferedWriter(new FileWriter(tmpFile));
			for (String line : compressedList)
			{
				bw.write(line);
			}

			return true;
		} catch (IOException e)
		{
			e.printStackTrace();
		} catch (Exception e2)
		{
			e2.printStackTrace();
		} finally
		{
			if (bw != null)
			{
				try
				{
					bw.close();
				} catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		}

		return false;
	}

	/**
	 * 将文件读取成List
	 * 
	 * @param file
	 * @return
	 */
	private static List<String> readFileToList(File f)
	{
		BufferedReader fr = null;
		try
		{
			fr = new BufferedReader(new FileReader(f));
			List<String> lineList = new ArrayList<String>(50);

			String lineOne = null;
			while ((lineOne = fr.readLine()) != null)
			{// 进行一次替换，并加入到lineList中
				// 替换tab和空格
				// lineOne = lineOne.replaceAll("\t", "");

				while (lineOne.indexOf(" ") == 0 || lineOne.indexOf("\t") == 0)
				{
					lineOne = lineOne.substring(1);
				}

				if (!"".equals(lineOne))
				{// 非空则加入到列表中
					lineList.add(lineOne);
				}
			}

			return lineList;
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			if (fr != null)
			{
				try
				{
					fr.close();
				} catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		}
		return null;
	}
}
