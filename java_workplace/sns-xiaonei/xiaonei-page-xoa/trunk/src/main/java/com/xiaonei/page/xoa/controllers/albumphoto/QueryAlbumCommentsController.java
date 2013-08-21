package com.xiaonei.page.xoa.controllers.albumphoto;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.album.dto.AlbumDTO;
import com.xiaonei.album.model.Album;
import com.xiaonei.album.service.AlbumService;
import com.xiaonei.page.service.album.AlbumPhotoService;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

@Path("qac")
public class QueryAlbumCommentsController {

    @Autowired
    private AlbumPhotoService albumPhotoService;

    @Autowired
    private AlbumService albumService;
    
    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Get
    public String index(@Param("albumId")long albumId,@Param("ownerId")int ownerId) {
        
        Album album=albumService.getAlbum(albumId, ownerId);
        
        AlbumDTO albumDTO=new AlbumDTO();
        albumDTO.setAlbum(album);
        try {
            albumService.getAlbumWithComments(albumDTO);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        logger.info(albumDTO.getJson());
        
        return "@";
    }
}
