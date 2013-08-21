/*
 * Created on 2004-2-5
 *
 * Huangjing
 */
package com.xiaonei.platform.core.utility;

import java.awt.Color;
import java.awt.Font;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.Date;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import net.jmge.gif.Gif89Encoder;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * @author Huang Jing
 */
public class RandImageServlet extends HttpServlet {
	public final String MOL_RANDOM = "MOL_RANDOM_TOKEN";
	//因为主工程去session运动，但又担心其他应用需要使用session,所以增加了这个开关值
	private boolean needSession = true;
	
	/* (non-Javadoc)
	 * @see javax.servlet.http.HttpServlet#doGet(javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException {
		String postfix = request.getParameter("post");
		
		response.setContentType("image/gif");
		
		BufferedImage bi = new BufferedImage(42, 16, BufferedImage.TYPE_3BYTE_BGR);
		bi.getGraphics().setFont(new Font("Arial", Font.BOLD, 20));
		bi.getGraphics().setColor(Color.WHITE);
		
		String rand = String.valueOf(MathUtils.randomInt(10))
			+ MathUtils.randomInt(10)
			+ MathUtils.randomInt(10)
			+ MathUtils.randomInt(10);
		
		bi.getGraphics().drawString(rand, 4, 12);
		Gif89Encoder enc = new Gif89Encoder(bi);
		
		if(this.needSession)
		{
		HttpSession session = request.getSession(true);
		session.setAttribute(MOL_RANDOM + postfix, rand);
		}
		MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_icode);
		String cacheRand = (String)mcm.get(MOL_RANDOM + postfix);
		if(cacheRand!=null&&!cacheRand.equals("")){
			mcm.set(MOL_RANDOM + postfix,rand,new Date(60*30*1000));
		}else
			mcm.add(MOL_RANDOM + postfix,rand,new Date(60*30*1000));

		enc.encode(response.getOutputStream());
	}

	/* (non-Javadoc)
	 * @see javax.servlet.http.HttpServlet#doPost(javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse)
	 */
	protected void doPost(HttpServletRequest arg0, HttpServletResponse arg1)
		throws ServletException, IOException {
		this.doGet(arg0, arg1);
	}

	/* (non-Javadoc)
	 * @see javax.servlet.http.HttpServlet#service(javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse)
	 */
	protected void service(HttpServletRequest arg0, HttpServletResponse arg1)
		throws ServletException, IOException {
		this.doGet(arg0, arg1);
	}

	@Override
	public void init(ServletConfig config) throws ServletException
	{
		super.init(config);
		String ns = config.getInitParameter("needSession");
		if("0".equals(ns) || "false".equals(ns))
		{
			this.needSession = false;
		}
	}

}
