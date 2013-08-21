package com.xiaonei.commons.jade.impl.xceadapter.datasource;

import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.w3c.dom.Element;

import com.xiaonei.commons.jade.impl.util.XMLUtils;

/**
 * 负责从配置项生成对应的路由对象。
 * 
 * @author han.liao
 */
public class XceRouterLoader {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(XceRouterLoader.class);

    // 路由的名称
    public static final String DIRECT = "direct";

    public static final String ROUND = "round";

    public static final String RANGE = "range";

    public static final String HASH = "hash";

    public static final String HEX_HASH = "hex-hash";

    public static XceRouter fromProps(String prefix, Map<String, String> props) {

        String mode = props.get(prefix);

        if (mode != null) {

            // .by-column
            String column = props.get(prefix + ".by-column");

            // .partitions
            String partitions = props.get(prefix + ".partitions");

            // .target-pattern
            String pattern = props.get(prefix + ".target-pattern");

            if (HASH.equalsIgnoreCase(mode)) {

                return createHashRouter(column, pattern, partitions);

            } else if (RANGE.equalsIgnoreCase(mode)) {

                return createRangeRouter(column, pattern);

            } else if (DIRECT.equalsIgnoreCase(mode)) {

                return createDirectRouter(column, pattern);

            } else if (ROUND.equalsIgnoreCase(mode)) {

                return createRoundRouter(pattern, partitions);

            } else if (HEX_HASH.equalsIgnoreCase(mode)) {

                return createHexHashRouter(column, pattern, partitions);
            }
        }

        return null;
    }

    public static XceRouter fromXML(Element element) {

        for (Element child : XMLUtils.getChildren(element)) {

            // <by-column>
            String column = XMLUtils.getChildText(child, "by-column");

            // <partitions>
            String partitions = XMLUtils.getChildText(child, "partitions");

            // <target-pattern>
            String pattern = XMLUtils.getChildText(child, "target-pattern");

            if (HASH.equalsIgnoreCase(child.getTagName())) {

                return createHashRouter(column, pattern, partitions);

            } else if (RANGE.equalsIgnoreCase(child.getTagName())) {

                return createRangeRouter(column, pattern);

            } else if (DIRECT.equalsIgnoreCase(child.getTagName())) {

                return createDirectRouter(column, pattern);

            } else if (ROUND.equalsIgnoreCase(child.getTagName())) {

                return createRoundRouter(pattern, partitions);

            } else if (HEX_HASH.equalsIgnoreCase(child.getTagName())) {

                return createHexHashRouter(column, pattern, partitions);
            }
        }

        return null;
    }

    private static HashXceRouter createHashRouter(String column, String pattern, String partitions) {

        // 检查所需的属性
        if (column == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hash' must have 'by-column' property.");
            }

            return null;
        }

        if (partitions == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hash' must have 'partitions' property.");
            }

            return null;
        }

        if (pattern == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hash' must have 'target-pattern' property.");
            }

            return null;
        }

        try {
            int count = Integer.parseInt(partitions);

            // 输出日志
            if (logger.isDebugEnabled()) {
                logger.debug("Creating router 'hash' [by-column = " + column + ", partitions = "
                        + count + ", target-pattern = " + pattern + ']');
            }

            return new HashXceRouter(column, pattern, count);

        } catch (NumberFormatException e) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hash' property 'partitions' must be number.");
            }

            return null;
        }
    }

    private static RoundXceRouter createRoundRouter(String pattern, String partitions) {

        if (partitions == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'round' must have 'partitions' property.");
            }

            return null;
        }

        if (pattern == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'round' must have 'target-pattern' property.");
            }

            return null;
        }

        try {
            int count = Integer.parseInt(partitions);

            // 输出日志
            if (logger.isDebugEnabled()) {
                logger.debug("Creating router 'round' [partitions = " + count
                        + ", target-pattern = " + pattern + ']');
            }

            return new RoundXceRouter(pattern, count);

        } catch (NumberFormatException e) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'round' property 'partitions' must be number.");
            }

            return null;
        }
    }

    private static DirectXceRouter createDirectRouter(String column, String pattern) {

        if (pattern == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'direct' must have 'target-pattern' property.");
            }

            return null;
        }

        // 输出日志
        if (logger.isDebugEnabled()) {
            if (column != null) {
                logger.debug("Creating router 'direct' [by-column = " + column
                        + ", target-pattern = " + pattern + ']');
            } else {
                logger.debug("Creating router 'direct' [target-pattern = " + pattern + ']');
            }
        }

        return new DirectXceRouter(column, pattern);
    }

    private static RangeXceRouter createRangeRouter(String column, String pattern) {

        // 检查所需的属性
        if (column == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'range' must have 'by-column' property.");
            }

            return null;
        }

        if (pattern == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'range' must have 'target-pattern' property.");
            }

            return null;
        }

        // 输出日志
        if (logger.isDebugEnabled()) {
            logger.debug("Creating router 'range' [by-column = " + column + ", target-pattern = "
                    + pattern + ']');
        }

        return new RangeXceRouter(column, pattern);
    }

    private static HexHashXceRouter createHexHashRouter(String column, String pattern,
            String partitions) {

        // 检查所需的属性
        if (column == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hex-hash' must have 'by-column' property.");
            }

            return null;
        }

        if (partitions == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hex-hash' must have 'partitions' property.");
            }

            return null;
        }

        if (pattern == null) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hex-hash' must have 'target-pattern' property.");
            }

            return null;
        }

        try {
            int count = Integer.parseInt(partitions);

            // 输出日志
            if (logger.isDebugEnabled()) {
                logger.debug("Creating router 'hex-hash' [by-column = " + column
                        + ", partitions = " + count + ", target-pattern = " + pattern + ']');
            }

            return new HexHashXceRouter(column, pattern, count);

        } catch (NumberFormatException e) {

            // 输出日志
            if (logger.isErrorEnabled()) {
                logger.error("Router 'hex-hash' property 'partitions' must be number.");
            }

            return null;
        }
    }
}
