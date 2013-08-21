package sns.renren.chong.controllers.tableManager;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.annotations.LoginRequired;
import sns.renren.chong.beans.LoginConfigBean;
import sns.renren.chong.beans.TableEditedBean;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.services.TableManagerService;

/**
 * 该controller --对应-- login_config表格管理页面
 * 
 * @author wenhong.mo@renren-inc.com
 */
@LoginRequired
@Path("")
public class TableManagerController {
	
	private static final int PAGE_SIZE = 15;
	@Autowired
	private TableManagerService tableManagerService;
	
	/**
	 * 处理url：/tableManager/showpage
	 * 显示页面 
	 * 当currentPage： -100  最后一页；1 第一页；小于1 调整为1
	 */
	public String showPage(Invocation inv, @Param("pattern")String pattern, @Param("currentPage")int currentPage) {
		//search..
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		int sumRecords = 0;
		int sumPages = 0;
		int curPage = 1;
		
		List<LoginConfigBean> content = null;
		if (pattern == null) {
			
			sumRecords = tableManagerService.getLoginConfigRecordsSum();
			sumPages = getSumPages(PAGE_SIZE, sumRecords);
			curPage = getCurrentPage(currentPage, sumPages);	
			content = tableManagerService.getSpecifiedPageContentFromLoginConfig(curPage, PAGE_SIZE);		
		} else {
			
			sumRecords = tableManagerService.getLoginConfigMatchRecordsSum(pattern);
			sumPages = getSumPages(PAGE_SIZE, sumRecords);
			curPage = getCurrentPage(currentPage, sumPages);
			content = tableManagerService.getSpecifiedPageContentFromLoginConfigMatch(pattern, curPage, PAGE_SIZE);
		}
		
		inv.addModel("content", content);
		inv.addModel("currentPage", curPage);
		inv.addModel("sumPages", sumPages);
		inv.addModel("host", host);
		inv.addModel("pattern", pattern);
		
		return "tableManager-showpage";
	}
	
	/**
	 * 处理url: post - /tableManager/actionNextPage
	 * 重定向..
	 */
	@Post("actionNextPage")
	public String actionNextPage(Invocation inv, @Param("pattern")String pattern, @Param("currentPage")int currentPage ) {
		int tagPage = currentPage + 1;
		return "r:/tableManager/showPage?currentPage=" + tagPage + "&pattern=" + pattern ;
	}
	
	/**
	 * 处理url: post - /tableManager/actionPreviousPage
	 * 重定向..
	 */
	@Post("actionPreviousPage")
	public String actionPreviousPage(Invocation inv, @Param("pattern")String pattern, @Param("currentPage")int currentPage) {
		int tagPage = currentPage - 1;
		return "r:/tableManager/showPage?currentPage=" + tagPage + "&pattern=" + pattern ;
	}
	
	/**
	 * 处理url: post - /tableManager/actionSpecifiedPage
	 * 重定向..
	 */
	@Post("actionSpecifiedPage")
	public String actionSpecifiedPage(Invocation inv, @Param("pattern")String pattern, @Param("specifiedPage")int specifiedPage) {
		int tagPage = specifiedPage;
		return "r:/tableManager/showPage?currentPage=" + tagPage + "&pattern=" + pattern;	
	}

	/**
	 * 处理url: post - /tableManager/actionModify
	 * 修改login_config的特定记录,如果成功将操作信息添加到tableEditedHistory
	 * 重定向..
	 */
	@Post("actionModify")
	public String actionModify(Invocation inv,
							   @Param("configKey")String configKey, @Param("newValue")String newValue,
							   @Param("oldValue")String oldValue, @Param("currentPage")int currentPage) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		LoginConfigBean loginConfigBean = new LoginConfigBean(configKey, oldValue);
		tableManagerService.modifyLoginConfigRecord(loginConfigBean, newValue, host.getName());
		return "r:/tableManager/showPage";
	}
	
	/**
	 * 处理url: post - /tableManager/actionAdd
	 * 添加login_config的记录,如果成功将操作信息添加到tableEditedHistory
	 * 重定向..
	 */
	@Post("actionAdd")
	public String actionAdd(Invocation inv, @Param("newKey")String newKey, @Param("newValue")String newValue) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		LoginConfigBean loginConfigBean = new LoginConfigBean(newKey, newValue);
		if (!tableManagerService.addLoginConfigRecord(loginConfigBean, host.getName())) {
			return "tableManager-add-fail";
		}
		return "r:/tableManager/showPage?currentPage=-100";
	}
	
	/**
	 * 处理url: post - /tableManager/actionDelete
	 * 删除login_config的记录,如果成功将操作信息添加到tableEditedHistory
	 * 重定向..
	 */
	@Post("actionDelete")
	public String actionDelete(Invocation inv,
							   LoginConfigBean loginConfigBean,
							   @Param("currentPage")int currentPae) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		tableManagerService.deleteLoginConfigRecord(loginConfigBean, host.getName());
		return "r:/tableManager/showPage";
	}
		
	/**
	 * 处理url: post - /tableManager/showAllHistory
	 * 查看所有编辑历史(tableEditedHistory表), 记录按指定方式排序
	 */
	@Get("showAllHistory")
	public String showAllHistory(Invocation inv, 
								 @Param("sortPattern")String sortPattern,
								 @Param("keyPattern")String keyPattern) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		List<TableEditedBean> histories = null;
		if(keyPattern == null) {
			if (sortPattern == null || "key".equals(sortPattern)) {
				histories = this.tableManagerService.getKeySortHistory();
			} else if ("time".equals(sortPattern)) {
				histories = this.tableManagerService.getTimeSortHistory();
			}
		} else {
			histories = this.tableManagerService.getHistoryMatchKeyPattern(keyPattern);
		}
		inv.addModel("host", host);
		inv.addModel("histories", histories);
		
		return "tableManager-showAllHistory";
	}
	
	/**
	 * 处理url: post - /tableManager/showSpecifiedKeyHistory
	 * 查看匹配的记录的历史编辑记录(tableEditedHistory表)
	 */
	@Get("showSpecifiedKeyHistory")
	public String showSpecifiedKeyHistory(Invocation inv, LoginConfigBean bean) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		List<TableEditedBean> histories = tableManagerService.getSpecifiedKeyHistory(bean.getConfigKey());
		inv.addModel("host", host);
		inv.addModel("histories", histories);
		inv.addModel("loginConfigBean", bean);
		return "tableManager-showSpecifiedKeyHistory";
	}
	
	/**
	 * 处理url: post - /tableManager/actionModifyInKeyHistory
	 * 修改指定记录，如果成功将操作信息添加到tableEditedHistory
	 * 重定向：/tableManager/showSpecifiedKeyHistory?query
	 */
	@Post("actionModifyInKeyHistory")
	public String actionModifyInKeyHistory(Invocation inv, LoginConfigBean bean, @Param("newValue")String newValue){
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		String query = "?configKey=" + bean.getConfigKey() + "&configValue=" + newValue;
		//LoginConfigBean loginConfigBean = new LoginConfigBean(configKey, oldValue);
		tableManagerService.modifyLoginConfigRecord(bean, newValue, host.getName());
		return "r:/tableManager/showSpecifiedKeyHistory" + query;
	}
	
	
	private int getCurrentPage(int page, int sumPages) {
		if (page < 1 && page != -100) {
			return 1;
		} else if (page == -100 || page > sumPages) {
			return sumPages;
		}
		return page;
	}
	
	private int getSumPages(int pageSize, int sumRecords) {
		return (sumRecords / pageSize) + ((sumRecords % pageSize) == 0 ? 0 : 1);
	}
	
}
