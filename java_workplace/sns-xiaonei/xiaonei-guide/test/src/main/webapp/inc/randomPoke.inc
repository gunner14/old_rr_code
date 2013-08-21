<%@ page pageEncoding="UTF-8" %>
<script type="text/javascript">
  function doRandomPoke() {
    var url = '/randompoke.do';
    var pars = '';
    var myAjax = new Ajax.Updater(
                 {success:'randomPokeTo'}, 
                 url, 
                 { method: 'get', parameters: pars, onFailure: hello}
            );
  }
  function hello() {
    alert("hello");
  }
</script>

