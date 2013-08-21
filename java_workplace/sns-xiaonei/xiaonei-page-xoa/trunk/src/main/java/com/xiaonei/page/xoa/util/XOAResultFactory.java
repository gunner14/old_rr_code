package com.xiaonei.page.xoa.util;

import com.xiaonei.page.xoa.model.commons.PageXOAResult;

/**
 * 提供普遍的XOA结果返回工厂类
 * 
 * @author yi.li@renren-inc.com since 2012-2-28
 * 
 */
public class XOAResultFactory {

    private static final PageXOAResult SUCC = getResult(PageResultEnum.SUCC);

    /**
     * 根据{@link PageResultEnum}返回一个{@link PageXOAResult}对象
     * 
     * @param type
     * @see PageXOAResult
     * @see PageResultEnum
     */
    public static PageXOAResult getResult(PageResultEnum type) {
        PageXOAResult result = new PageXOAResult();
        result.setCode(type.getCode());
        result.setMsg(type.getMsg());
        return result;
    }

    /**
     * 返回一个code为{@link PageResultEnum.UNKNOWN}的{@link PageXOAResult}结果，
     * 其中结果中的msg为传入的参数msg
     * 
     * @param msg
     * @return
     */
    public static PageXOAResult getUnknownExceptionResult(String msg) {
        PageXOAResult result = new PageXOAResult();
        result.setCode(PageResultEnum.UNKNOWN.getCode());
        result.setMsg(msg);
        return result;
    }

    /**
     * 返回执行成功的结果
     * 
     * @return
     */
    public static PageXOAResult getSuccessResult() {
        return SUCC;
    }

    /**
     * <p>
     * 获取一个自定义的结果，结果{@link PageXOAResult}中的code为传入参数code，msg为传入参数的msg
     * 
     * <p>
     * 注意：code不能小于等于100，100一下是系统code，自定义code必须>100
     * 
     * @param code
     * @param msg
     * @return
     * @throws IllegalArgumentException
     */
    public static PageXOAResult getlCustomResult(int code, String msg)
            throws IllegalArgumentException {
        if (code <= 100) {
            throw new IllegalArgumentException("custom code must be greater than 100");
        }

        PageXOAResult result = new PageXOAResult();
        result.setCode(code);
        result.setMsg(msg);
        return result;
    }
}
