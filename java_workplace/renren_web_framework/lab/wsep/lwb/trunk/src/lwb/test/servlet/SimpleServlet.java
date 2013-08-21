package lwb.test.servlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Enumeration;

import javax.servlet.ServletException;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class SimpleServlet extends BaseTestServlet {

	private static final long serialVersionUID = 1L;

	public static final String CRLF = "\r\n";
	
	@SuppressWarnings("unchecked")
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
	
		String method = request.getMethod();
		String protocol = request.getProtocol();
		String uri = request.getRequestURI();
		String queryString = request.getQueryString();
		if (queryString != null) {
			uri += "?" + queryString;
		}
		
		response.setContentType("text/plain; charset=UTF-8");
		PrintWriter out = response.getWriter();
		
		out.append(method);
		out.append(' ');
		out.append(uri);
		out.append(' ');
		out.append(protocol);
		out.append(CRLF);
		
		Enumeration<String> headerNames = request.getHeaderNames();
		while (headerNames.hasMoreElements()) {
			String headerName = headerNames.nextElement();
			Enumeration<String> headers = request.getHeaders(headerName);
			while (headers.hasMoreElements()) {
				String headerValue = headers.nextElement();
				out.append(headerName);
				out.append(": ");
				out.append(headerValue);
				out.append(CRLF);
			}
		}
		
		out.append(CRLF);
		ServletInputStream is = request.getInputStream();
		byte[] buffer = new byte[1024];
		int len = 0;
		while ((len = is.read(buffer, 0, buffer.length)) != -1) {
			out.append(new String(buffer, 0, len));
		}
		out.flush();
	}

	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		doGet(req, resp);
	}
	
}
