package com.xiaonei.platform.component.tools;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

public class StringUtil {
	
	public static <T> String join(final Collection<T> objs, final String delimiter) {
	  if (objs == null || objs.isEmpty())
	    return "";
	  Iterator<T> iter = objs.iterator();
	  StringBuffer buffer = new StringBuffer(iter.next().toString());
	  while (iter.hasNext())
	    buffer.append(delimiter).append(iter.next().toString());
	  return buffer.toString();
	}

	/**
	 * 切割长英文，便于前段显示
	 * @param content 要切割的内容
	 * @param cutLength	要切割的长度
	 * @return	切割后的内容
	 */
	private static String startTag = "<span>";
	private static String endTag = "</span><wbr /><span class=\"word_break\"></span>";
	private static String finishTag = "</span>";
	public static String cutLongEngWord(String content, int cutLength){
		if(content==null)
			return null;
		char[] array = content.toCharArray();
		StringBuffer sb = new StringBuffer(content);
		int startPos = 0;
		int count = 0;
		int insertCount = 0;
		boolean isTag = false;
		boolean isSpaChar = false;
		char ch;
		for(int i=0;i<array.length;i++){
			ch = array[i];
			if(ch=='<'){
				isTag = true;
				continue;
			}else if(ch=='>'){
				isTag = false;
			}
			if(isTag)
				continue;
			if(ch=='&'){
				isSpaChar = true;
				continue;
			}else if(ch==';'){
				isSpaChar = false;
			}
			if(isSpaChar)
				continue;
//			if(ch!=' '&&ch!='　'){
			++count;
//			}
//			else{
//				startPos = i+1;
//				count = 0;
//				continue;
//			}
			if(i==array.length -1){
				sb.insert(startPos+insertCount*(startTag.length()+endTag.length()), startTag);
				sb.insert(i+1+insertCount*(startTag.length()+endTag.length())+startTag.length(), finishTag);
				break;
			}
			else if(count>=cutLength){
				sb.insert(startPos+insertCount*(startTag.length()+endTag.length()), startTag);
				sb.insert(i+1+insertCount*(startTag.length()+endTag.length())+startTag.length(), endTag);
				insertCount++;
				startPos = i+1;
				count = 0;
			}
		}
		return sb.toString();
	}
	
	public static void main(String[] args) {
		String content = "为什么好友生日,还在显示几天前的. 错过一天也就算了,过了好几天的还有意义么.";
		System.out.println(cutLongEngWord(content,10));
	}


}
