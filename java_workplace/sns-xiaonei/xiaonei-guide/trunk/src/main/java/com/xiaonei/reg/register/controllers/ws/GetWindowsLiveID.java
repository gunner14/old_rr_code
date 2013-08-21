package com.xiaonei.reg.register.controllers.ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class GetWindowsLiveID {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			System.out.println(requestContacts("EwC4ARAnAAAUWkziSC7RbDJKS1VkhugDegv7L0eAAP5R4iszIEjodn7tCm%2faO1EPI2ftIrrJXk8cJQhCZyDOel02IcIM4towf4SFyogfd%2bZyNxx2SvSxG4%2biicbAQ%2biSYBaIh84xkXTBKylQIqdFKOALAX5H6ALb%2fobkOeVQJu329eVRlJrWoijuC9WHEyvBljPxw6sxYgxMU%2bcT1jWYA2YAAAgBKRNVeRWs2QgBruY8B1XkMqbiptF0DOhP%2fTwV3kOHgo8swM5VlT7ysOwytEtDx2Zy6oIiYn%2byHsy83zYi7nEH8Zt7C%2baKczComjHqC9Ax9PH9Qjlk7xbmyoX7VDxe5WR%2fsQ34906zqJ7r8CSng8Dw6RGcODpC%2f0462SoOAWq4sQU9B7DZMPTywJn9Y6odiCz8y%2freqES4zlwEsdUyzAp5O%2bO9o0GouAoJYIOS2CYCj%2fh0yWJeSlu8Nu6Zx2P4JD%2fRXCtLB9yVKplaIPmrKkRbI70AY3jO2Xrt4kpIOzElXMzXfhdSWm7XSqJooYzRTl1rAcF5%2fXbeUmc67aoyGD5ODIlzPKzlWMaR0kVc%2fVqhXeTzAAA%3d"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static String requestContacts(String accessToken) throws IOException {
		StringBuilder sb = new StringBuilder();
		try{
			URL url = new URL("http://apis.live.net/V4.1/cid-3e21cdffe9bd5ba8/Profiles");
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setRequestProperty("Accept", "application/json");
			conn.setRequestProperty("Content-Type", "application/json");
			conn.setRequestProperty("Authorization", "WRAP access_token=" + accessToken);
			conn.setAllowUserInteraction(true);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setUseCaches(false);
			conn.setDefaultUseCaches(false);
			conn.setRequestMethod("GET");
			conn.getInputStream();
			BufferedReader rd = new BufferedReader(new InputStreamReader(conn.getInputStream()));
			String line;
			while ((line = rd.readLine()) != null) {
				sb.append(line);
			}
			rd.close();
		} catch (MalformedURLException e){
			e.printStackTrace();
			throw e;
		} catch (IOException e){
			e.printStackTrace();
			throw e;
		}
		return sb.toString();
	}

}
