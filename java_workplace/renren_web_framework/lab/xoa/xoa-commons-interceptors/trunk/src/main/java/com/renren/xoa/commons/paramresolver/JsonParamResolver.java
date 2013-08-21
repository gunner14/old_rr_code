package com.renren.xoa.commons.paramresolver;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.paramresolver.ParamMetaData;
import net.paoding.rose.web.paramresolver.ParamResolver;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.codehaus.jackson.map.ObjectMapper;

import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.commons.interceptors.ojmapping.ServerSideObjectMapperFactory;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-30 下午12:44:19
 */
public class JsonParamResolver implements ParamResolver {

	protected Log logger = LogFactory.getLog(this.getClass());
	
	private ObjectMapper mapper = ServerSideObjectMapperFactory.getInstance().getObjectMapper();
	
	@Override
	public Object resolve(Invocation inv, ParamMetaData metaData)
			throws Exception {
		String paramName = metaData.getParamName();
		Class<?> type = metaData.getParamType();
		String value = inv.getParameter(paramName);
		if (value == null) {
			return null;
		}
		return mapper.readValue(value, type);
	}

	@Override
	public boolean supports(ParamMetaData metaData) {
		return metaData.getAnnotation(JsonParam.class) != null;
	}
}
