package com.xiaonei.commons.interceptors.ojmapping;

import org.codehaus.jackson.map.DeserializationConfig;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.annotate.JsonSerialize;

/**
 * 封装获取ObjectMapper的过程
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-2 下午01:27:28
 */
public class ServerSideObjectMapperFactory {

	private static ServerSideObjectMapperFactory instance = new ServerSideObjectMapperFactory();

	private ObjectMapper mapper;

	public static ServerSideObjectMapperFactory getInstance() {
		return instance;
	}

	private ServerSideObjectMapperFactory() {
		initObjectMapper();
	}

	public ObjectMapper getObjectMapper() {
		return mapper;
	}

	private void initObjectMapper() {
		mapper = new ObjectMapper();
		//设置ObjectMapper只序列化非null的属性，这样可以节省流量
		mapper.getSerializationConfig().setSerializationInclusion(
				JsonSerialize.Inclusion.NON_NULL);
		//disable掉FAIL_ON_UNKNOWN_PROPERTIES属性，增强容错性；因为现在有从客户端发json到服务端，在服务端反解的情况了，
		//所以要加上这个容错选项。
		mapper.getDeserializationConfig().disable(
				DeserializationConfig.Feature.FAIL_ON_UNKNOWN_PROPERTIES);
		//TODO 设置为NON_DEFAULT的时候，序列化是对象会被偷换成另外的对象，有些Date类型的字段会变成null
		//查明这个原因
	}
}