package xce.tripod.example.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

@DAO (catalog = "user_basic")
//@DAO(catalog = "user_names_preloader")
public interface ExampleDAO {

    // @SQL("SELECT name FROM user_names WHERE id = :id")
    // String select(@SQLParam("id")int id);

    @SQL("SELECT name FROM user_audit WHERE id = :id")
    String select(@SQLParam("id") int id);

}
