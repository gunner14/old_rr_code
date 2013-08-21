package com.xiaonei.platform.component.gossip.service;

import java.beans.Introspector;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.AnnotationBeanNameGenerator;

public class PackageAnnotationBeanNameGenerator extends AnnotationBeanNameGenerator {

	private static Log logger = LogFactory.getLog(PackageAnnotationBeanNameGenerator.class);

	@Override
	protected String buildDefaultBeanName(BeanDefinition definition) {

		String name = Introspector.decapitalize(definition.getBeanClassName()); // 以类的完整名作为beanName

		if (logger.isDebugEnabled()) {

			logger.debug("build default bean name '" + name + "' for definition " + definition);

		}

		return name;

	}
}