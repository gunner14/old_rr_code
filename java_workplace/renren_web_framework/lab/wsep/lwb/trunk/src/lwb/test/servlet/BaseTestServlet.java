package lwb.test.servlet;

import javax.servlet.http.HttpServlet;

public class BaseTestServlet  extends HttpServlet {

	private static final long serialVersionUID = 1L;

	public BaseTestServlet() {
		System.out.println(this.getClass().getName() + " is constructing...");
	}
	
}
