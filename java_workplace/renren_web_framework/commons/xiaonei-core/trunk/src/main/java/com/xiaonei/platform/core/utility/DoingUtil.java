package com.xiaonei.platform.core.utility;

import com.xiaonei.platform.core.opt.OpiConstants;

public class DoingUtil {
	

    /**
	 * 替换文本中的链接，换成可用的url
	 * @param content	要替换的内容
	 * @return	替换后的内容
	 */
	public static String replaceLink(String content){
		int linkStart = content.indexOf(START_TAG);
		int linkEnd = 0;
		String link = null;
		while(linkStart!=-1){
			linkEnd = content.indexOf(END_TAG,linkStart+1);
			if(linkEnd<=linkStart)
				break;
			link = content.substring(linkStart+"[link]".length(),linkEnd);
			link = "<a href=\""+link+"\" target='_blank'>"+link+"</a>";
			content = content.substring(0,linkStart)+link+content.substring(linkEnd+END_TAG.length());

			linkStart = content.indexOf(START_TAG,linkStart+1);
		}
		return content;
	}
	
	/**
	 * 替换状态中的表情和特殊标记
	 * @param content
	 * 
	 * @return
	 */
	public static String replaceIcon(String content){
		for(int i=0;i<DOING_ICON_SRC.length;i++){
				content = content.replaceAll(DOING_ICON_SRC[i], DOING_ICON_DES[i]);
		}
		return content;
	}
	

	private static final String[] DOING_ICON_SRC = {"\\((L|l)\\)","\\((P|p)\\)","\\((C|c)\\)","\\((B|b)\\)","\\((F|f)\\)","\\((T|t)\\)","\\((Y|y)\\)","\\((G|g)\\)","\\((D|d)\\)","\\((R|r)\\)","\\((W|w)\\)", "\\((3)\\)", "\\((H|h)\\)"};
	private static final String[] DOING_ICON_DES = {
		"<img src='"+OpiConstants.urlStatic+"/img/ems/love.gif' 		alt='爱' 				class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/bulb.gif' 		alt='绿色环保大使' 		class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/candle.gif' 	alt='为震区祈福' 			class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/gsilk.gif' 		alt='绿丝带' 				class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/fist.gif' 		alt='捍卫开心' 			class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/torch.gif' 		alt='点燃百年奥运的火炬' 	class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/yeah.gif' 		alt='中国必胜' 			class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/img/ems/good.gif' 		alt='中国金牌第一' 		class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/imgpro/icons/doritos_log.jpg' 		alt='多力多滋' 			class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/imgpro/icons/ico_rocket.gif' 		    alt='神七问天' 			class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/imgpro/icons/ico_spacewalker.gif' 	alt='我是宇航员' 			class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/imgpro/icons/3years.gif'                      alt='校内三周年'          class='status-em' style='width:12px;height:12px' />",
		 "<img src='"+OpiConstants.urlStatic+"/imgpro/icons/philips.jpg'                     alt='飞利浦乐享健康好生活'  class='status-em' style='width:12px;height:12px' />"
		 };

    public static final String START_TAG = "[link]";
    public static final String END_TAG = "[/link]";
	public static void main(String[] argc){
		System.out.println(replaceIcon("(R)(3)(r)"));
	}

}
