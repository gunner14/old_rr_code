/**
 * 
 */
package com.xiaonei.click.handler.dispatch;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.xiaonei.click.admin.ReloadAble;
import com.xiaonei.click.business.work.Boss;
import com.xiaonei.click.business.work.Job;
import com.xiaonei.click.configure.Config;
import com.xiaonei.click.configure.Config.Key;
import com.xiaonei.click.constants.Constants;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class RequestDispatcher implements ReloadAble {

    private static volatile RequestDispatcher _instance = null;

    private Map<Conditions, Class<Job<?>>> conditions = new HashMap<Conditions, Class<Job<?>>>();

    private RequestDispatcher() {

        try {
            JSONArray ja = new JSONArray(Config.getInstance().get(Config.Key.REQUEST_FILTER, this)
                    .toString());
            conditions = reConfigure(ja);
        } catch (JSONException e) {
            throw new RuntimeException(e);
        }

    }

    public static RequestDispatcher getInstance() {
        if (_instance == null) {
            synchronized (RequestDispatcher.class) {

                if (_instance == null) {
                    _instance = new RequestDispatcher();
                }
            }
        }

        return _instance;
    }

    public void dispatch(String uri, Map<String, String> param) {
        for (Entry<Conditions, Class<Job<?>>> entry : conditions.entrySet()) {
            if (entry.getKey().check(uri)) {
                try {
                    Job<?> job = entry.getValue().newInstance();
                    job.init(param);
                    Boss.getInstance().submit(job);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            }
        }
    }

    @Override
    public void reload(Key k) {
        if (k.equals(Config.Key.REQUEST_FILTER)) {
            try {
                JSONArray ja = new JSONArray(Config.getInstance().get(Config.Key.REQUEST_FILTER,
                        null).toString());
                conditions = reConfigure(ja);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    }

    @SuppressWarnings("unchecked")
    private Map<Conditions, Class<Job<?>>> reConfigure(JSONArray ja) {
        Map<Conditions, Class<Job<?>>> newConditions = new HashMap<Conditions, Class<Job<?>>>();
        try {
            for (int i = 0; i < ja.length(); i++) {
                JSONObject joinner = ja.getJSONObject(i);
                Conditions urlCondition = new Conditions();
                String scheme = joinner.optString(Constants.SCHEME);
                if (scheme != null && scheme.length() > 0) {
                    urlCondition.add(new SchemeCondition(scheme));
                }
                String domain = joinner.optString(Constants.DOMAIN);
                if (domain != null && domain.length() > 0) {
                    urlCondition.add(new DomainCondition(domain));
                }
                String uri = joinner.optString(Constants.URI);
                if (uri != null && uri.length() > 0) {
                    urlCondition.add(new URICondition(uri));
                }

                if (urlCondition.size() > 0) {

                    String actionsClass = joinner.getString(Constants.ACTIONS);
                    newConditions.put(urlCondition, (Class<Job<?>>) Class.forName(actionsClass));
                }
            }
        } catch (JSONException e) {
            throw new RuntimeException(Config.Key.REQUEST_FILTER.toString()
                    + " format error.use old config", e);
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(Config.Key.REQUEST_FILTER.toString()
                    + " format error.use old config", e);
        }
        return newConditions;
    }

    @Override
    public void reloadAll() {
    }
}
