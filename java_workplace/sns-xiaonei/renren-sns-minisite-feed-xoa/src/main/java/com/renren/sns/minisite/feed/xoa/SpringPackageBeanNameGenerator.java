package com.renren.sns.minisite.feed.xoa;

import java.beans.Introspector;

import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.AnnotationBeanNameGenerator;

public class SpringPackageBeanNameGenerator extends AnnotationBeanNameGenerator {

    @Override
    protected String buildDefaultBeanName(BeanDefinition definition) {
        String name = Introspector.decapitalize(definition.getBeanClassName());
        return name;
    }

}
