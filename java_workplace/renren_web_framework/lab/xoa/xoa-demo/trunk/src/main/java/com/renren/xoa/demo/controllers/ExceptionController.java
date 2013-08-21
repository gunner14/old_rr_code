package com.renren.xoa.demo.controllers;

import javax.management.RuntimeErrorException;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

public class ExceptionController {

	@Get
	@Put
	@Post
	@Delete
	public Object index(Invocation inv) {
		boolean flag = true;
		if (flag) {
			throw new RuntimeException("This is an exception tests");
		}
		return null;
	}
}
