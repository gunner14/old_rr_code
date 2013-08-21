package lwb.test.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletInputStream;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class StreamTestServlet extends BaseTestServlet {

	private static final long serialVersionUID = 1L;

	public static final String CRLF = "\r\n";
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
	
		response.setContentType("text/plain; charset=UTF-8");
		
		ServletInputStream is = request.getInputStream();
		ServletOutputStream os = response.getOutputStream();
		int c;
		while ( (c = is.read()) != -1) {
			os.write(c);
		}
		os.flush();	
	}

	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		doGet(req, resp);
	}
	
}
