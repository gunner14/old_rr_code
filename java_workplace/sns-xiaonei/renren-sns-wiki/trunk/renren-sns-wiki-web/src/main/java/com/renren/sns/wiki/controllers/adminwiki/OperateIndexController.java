package com.renren.sns.wiki.controllers.adminwiki;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.biz.feed.WikiFeedPhotoUtils;
import com.renren.sns.wiki.enums.OperateIndexType;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.interceptor.WikiHolder;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.admin.OperateIndex;
import com.renren.sns.wiki.model.admin.OperateIndexSlide;
import com.renren.sns.wiki.model.feed.WikiFeedPhoto;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.adminwiki.OperateIndexService;
import com.renren.sns.wiki.service.adminwiki.OperateIndexSlideService;

/**
 * 首页运营
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-20
 * 
 */
@Path("/operate-index")
public class OperateIndexController {

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private OperateIndexService operateIndexService;

    @Autowired
    private OperateIndexSlideService operateIndexSlideService;

    @Get("")
    public String operateIndex(Invocation inv) {
        inv.addModel("navName", "operateIndex");
        return "operate-index";
    }

    /**
     * slide运营
     * 
     * @return
     */
    @Get("/slide/page/${wikiTypeName:\\w+}")
    public String slide(Invocation inv, @Param("wikiTypeName") String wikiTypeName) {
        inv.addModel("navName", "slide");
        inv.addModel("wikiTypeName", wikiTypeName);

        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);
        //查询列表
        List<OperateIndexSlide> operateIndexSlideList = operateIndexSlideService.getList(wikiType);
        for (OperateIndexSlide operateIndexSlide : operateIndexSlideList) {
            int wikiId = operateIndexSlide.getWikiId();
            Wiki wiki = wikiService.getWikiById(wikiId);
            operateIndexSlide.setWiki(wiki);
        }
        inv.addModel("operateIndexSlideList", operateIndexSlideList);
        return "slide";
    }

    @Get("/slide/edit")
    public String editSlide(Invocation inv, @Param("wikiTypeName") String wikiTypeName,
            @Param("wikiId") int wikiId) {

        OperateIndexSlide operateIndexSlide = operateIndexSlideService
                .getOperateIndexSlideByWikiId(wikiId);
        inv.addModel("operateIndexSlide", operateIndexSlide);
        inv.addModel("wikiTypeName", wikiTypeName);

        return "edit-slide";
    }

    @Post("/slide/update")
    public String updateSlide(Invocation inv, @Param("wikiTypeName") String wikiTypeName,
            @Param("operateIndexSlide") OperateIndexSlide operateIndexSlide) {

        OperateIndexSlide bean = operateIndexSlideService
                .getOperateIndexSlideById(operateIndexSlide.getId());
        WikiFeedPhoto wikiFeedPhoto = new WikiFeedPhoto();
        wikiFeedPhoto.setLargeUrl(operateIndexSlide.getImageUrl());
        if (!bean.getImageUrl().equals(operateIndexSlide.getImageUrl())) {
            WikiFeedPhotoUtils.completePhotoWidthHeight(wikiFeedPhoto);
        } else {
            wikiFeedPhoto.setLargeWidth(bean.getImageWidth());
            wikiFeedPhoto.setLargeHeight(bean.getImageHeight());
        }

        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);

        operateIndexSlide.setWikiType(wikiType.getWikiType());
        operateIndexSlide.setImageWidth(wikiFeedPhoto.getLargeWidth());
        operateIndexSlide.setImageHeight(wikiFeedPhoto.getLargeHeight());

        operateIndexSlideService.updateOperateIndexSlide(operateIndexSlide);

        inv.addModel("wikiTypeName", wikiTypeName);

        return "r:http://wiki.renren.com/adminwiki/operate-index/slide/page/" + wikiTypeName;
    }

    @Get("/slide/delete")
    @Post("/slide/delete")
    public String deleteSlide(Invocation inv, @Param("wikiTypeName") String wikiTypeName,
            @Param("wikiId") int wikiId) {
        int count = operateIndexSlideService.deleteByWikiId(wikiId);
        return "r:http://wiki.renren.com/adminwiki/operate-index/slide/page/" + wikiTypeName;
    }

    /**
     * 添加slide
     * 
     * @param inv
     * @param wikiTypeName
     * @param wikiId
     * @param imageUrl
     * @param order
     * @return
     */
    @Post("/slide/add")
    public String addSlide(Invocation inv, @Param("wikiTypeName") String wikiTypeName,
            @Param("wikiId") int wikiId, @Param("imageUrl") String imageUrl,
            @Param("order") int order) {
        inv.addModel("navName", "slide");
        Wiki wiki = wikiHolder.getWiki();

        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);

        if (wiki == null) {
            inv.addModel("msg", "wiki不存在");
        } else if (wiki.getType() != wikiType.getWikiType()) {
            inv.addModel("msg", "wiki的类型和添加的类型不匹配");
        } else {

            //检查是否已经添加过
            if (operateIndexSlideService.contains(wikiId)) {
                inv.addModel("msg", "已经添加过");
            } else {

                //使用WikiFeedPhotoUtils处理图片的宽高
                WikiFeedPhoto wikiFeedPhoto = new WikiFeedPhoto();
                wikiFeedPhoto.setLargeUrl(imageUrl);

                WikiFeedPhotoUtils.completePhotoWidthHeight(wikiFeedPhoto);

                //添加一条数据 
                OperateIndexSlide operateIndexSlide = new OperateIndexSlide();
                operateIndexSlide.setWikiId(wikiId);
                operateIndexSlide.setWikiType(wikiType.getWikiType());
                operateIndexSlide.setImageUrl(imageUrl);
                operateIndexSlide.setImageWidth(wikiFeedPhoto.getLargeWidth());
                operateIndexSlide.setImageHeight(wikiFeedPhoto.getLargeHeight());
                operateIndexSlide.setOrder(order);

                int count = operateIndexSlideService.addOperateIndexSlide(operateIndexSlide);
                if (count > 0) {
                    inv.addModel("msg", "添加成功");
                } else {
                    inv.addModel("msg", "添加失败");
                }
            }

        }

        return "r:http://wiki.renren.com/adminwiki/operate-index/slide/page/" + wikiTypeName;
    }

    @Get("/${wikiTypeName:\\w+}/${operateType:\\w+}")
    public String operateList(Invocation inv, @Param("wikiTypeName") String wikiTypeName,
            @Param("operateType") String operateType) {
        inv.addModel("navName", "operateIndex");

        inv.addModel("wikiTypeName", wikiTypeName);
        inv.addModel("operateType", operateType);

        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);
        OperateIndexType operateIndexType = OperateIndexType.getEnum(operateType);

        wrapOperateIndexList(inv, wikiType, operateIndexType);

        return "operate-index";
    }

    @Post("/add")
    public String operateAddWiki(Invocation inv, @Param("wikiId") int wikiId,
            @Param("order") int order, @Param("wikiTypeName") String wikiTypeName,
            @Param("operateType") String operateType) {
        inv.addModel("navName", "operateIndex");

        Wiki wiki = wikiHolder.getWiki();

        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);
        OperateIndexType operateIndexType = OperateIndexType.getEnum(operateType);

        if (wiki == null) {
            inv.addModel("msg", "wiki不存在");
        } else if (wiki.getType() != wikiType.getWikiType()) {
            inv.addModel("msg", "wiki的类型和添加的类型不匹配");
        } else if (operateIndexType == null) {
            inv.addModel("msg", "没有要填充的类型");
        } else {

            boolean exist = operateIndexService.contains(wikiId, operateIndexType);
            if (exist) {
                inv.addModel("msg", "已经存在");
            } else {

                //添加一条数据 
                OperateIndex operateIndex = new OperateIndex();
                operateIndex.setOperateType(operateIndexType.getValue());
                operateIndex.setOrder(order);
                operateIndex.setWikiId(wikiId);
                operateIndex.setWikiType(wikiType.getWikiType());

                int count = operateIndexService.addOperateIndex(operateIndex);
                if (count > 0) {
                    inv.addModel("msg", "添加成功");
                } else {
                    inv.addModel("msg", "添加失败");
                }
            }
        }

        wrapOperateIndexList(inv, wikiType, operateIndexType);

        inv.addModel("wikiTypeName", wikiTypeName);
        inv.addModel("operateType", operateType);

        return "operate-index";
    }

    /**
     * 删除wiki
     * 
     * @param inv
     * @param wikiId
     * @param order
     * @param wikiTypeName
     * @param operateType
     * @return
     */
    @Get("/delete")
    @Post("/delete")
    public String deleteWiki(Invocation inv, @Param("wikiId") int wikiId,
            @Param("wikiTypeName") String wikiTypeName, @Param("operateType") String operateType) {

        inv.addModel("navName", "operateIndex");

        WikiType wikiType = WikiType.getEnumByName(wikiTypeName);
        OperateIndexType operateIndexType = OperateIndexType.getEnum(operateType);

        operateIndexService.delete(wikiId, operateIndexType);

        wrapOperateIndexList(inv, wikiType, operateIndexType);

        return "operate-index";
    }

    private void wrapOperateIndexList(Invocation inv, WikiType wikiType,
            OperateIndexType operateIndexType) {
        if (wikiType == null || operateIndexType == null) {
            inv.addModel("msg", "参数不正确");
        } else {
            List<OperateIndex> operateIndexList = operateIndexService.getWikiIdList(wikiType,
                    operateIndexType);
            for (OperateIndex operateIndex : operateIndexList) {
                Wiki wiki2 = wikiService.getWikiById(operateIndex.getWikiId());
                operateIndex.setWiki(wiki2);
            }
            inv.addModel("operateIndexList", operateIndexList);
        }
    }
}
