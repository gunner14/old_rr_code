package lwb.test.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletInputStream;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import util.Counter;

public class BuffStreamTestServlet extends BaseTestServlet {

	private static final long serialVersionUID = 1L;

	public static final String CRLF = "\r\n";
	
	private Counter counter = new Counter(this.getClass().getSimpleName());
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
	
		response.setContentType("text/plain; charset=UTF-8");
		
		ServletInputStream is = request.getInputStream();
		ServletOutputStream os = response.getOutputStream();
		
		byte[] buffer = new byte[2048];
	
		int len;
		while ((len = is.read(buffer)) != -1) {
			os.write(buffer, 0, len);
		}
		
		os.flush();
		
		counter.incr();
	}

	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		doGet(req, resp);
	}
	
}
