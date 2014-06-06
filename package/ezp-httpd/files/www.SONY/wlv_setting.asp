<script language="javascript" type="text/javascript"> 
var F;

wlv_basic_objects[$0][$1] =
[ { name: 'wl$0_ssid$1_enable' },
  { name: 'wl$0_ssid$1_ssid' },
  { name: 'wl$0_ssid$1_hide' },
  { name: 'wl$0_ssid$1_wme_enable' },
  { name: 'wl$0_ssid$1_isolation_enable' },
  { name: 'wl$0_ssid$1_secmode' },
  { name: 'wl$0_ssid$1_key_index' },
  { name: 'wl$0_ssid$1_key1' },
  { name: 'wl$0_ssid$1_key2' },
  { name: 'wl$0_ssid$1_key3' },
  { name: 'wl$0_ssid$1_key4' },
  { name: 'wl$0_ssid$1_key' },
  { name: 'wl$0_ssid$1_crypto' },
  { name: 'wl$0_ssid$1_radius_ipaddr' },
  { name: 'wl$0_ssid$1_radius_port' },
  { name: 'wl$0_ssid$1_radius_key' },
  { name: 'wl$0_ssid$1_rekey_mode' },
  { name: 'wl$0_ssid$1_rekey_time_interval' },
  { name: 'wl$0_ssid$1_rekey_pkt_interval' }
];

wlv_local_basic_objects[$0][$1] =
[ { name: 'wl$0_ssid$1_ssid' },
  { name: 'wl$0_ssid$1_hide' },
  { name: 'wl$0_ssid$1_wme_enable' },
  { name: 'wl$0_ssid$1_isolation_enable' },
  { name: 'wl$0_ssid$1_secmode' },
  { name: 'wl$0_ssid$1_key_index' },
  { name: 'wl$0_ssid$1_key1' },
  { name: 'wl$0_ssid$1_key2' },
  { name: 'wl$0_ssid$1_key3' },
  { name: 'wl$0_ssid$1_key4' },
  { name: 'wl$0_ssid$1_key' },
  { name: 'wl$0_ssid$1_crypto' },
  { name: 'wl$0_ssid$1_radius_ipaddr' },
  { name: 'wl$0_ssid$1_radius_port' },
  { name: 'wl$0_ssid$1_radius_key' },
  { name: 'wl$0_ssid$1_rekey_mode' },
  { name: 'wl$0_ssid$1_rekey_time_interval' },
  { name: 'wl$0_ssid$1_rekey_pkt_interval' }
];

</script>
  <input type="hidden" name="wl$0_ssid$1_txrate" id="wl$0_ssid$1_txrate" 
    value='<% nvg_attr_get("wl$0_ssid$1_txrate","wl$0_basic_rule","$1","rate");%>'>

  <div class="setting">

  <div style="display: none;">
  <div class="label"><% lang("Wireless SSID"); %></div>
  <input type=radio name="wl$0_ssid$1_enable" id="wl$0_ssid$1_enable" value="1" onclick="wl_ssid_enable_disable(this.form, $0, $1)" 
    <%nvg_attr_match("wl$0_ssid$1_enable", "wl$0_basic_rule", "$1", "enable", "1", "checked");%>>
    <% lang("Enable"); %></input>
  <input type=radio name="wl$0_ssid$1_enable" id="wl$0_ssid$1_enable" value="0" onclick="wl_ssid_enable_disable(this.form, $0, $1)" 
    <%nvg_attr_match("wl$0_ssid$1_enable", "wl$0_basic_rule", "$1", "enable", "0", "checked");%>>
    <% lang("Disable"); %></input>
  <br/>
  </div>
  
  <div class="label"><% lang("SSID"); %></div>
  <input name="wl$0_ssid$1_ssid" id="wl$0_ssid$1_ssid" onchange="wl_valid_ssid(this)" value='<% nvg_attr_get("wl$0_ssid$1_ssid","wl$0_ssid_rule","$1","ssid");%>' style="border: 0px" readonly>
  <br/>

  <div style="display: none;">
  <div class="label"><% lang("Wireless SSID Broadcasting"); %></div>
  <input type=radio name="wl$0_ssid$1_hide" id="wl$0_ssid$1_hide" value="0" <%nvg_attr_match("wl$0_ssid$1_hide", "wl$0_basic_rule", "$1", "hidden", "0", "checked");%>><% lang("Enable"); %></input>
  <input type=radio name="wl$0_ssid$1_hide" id="wl$0_ssid$1_hide" value="1" <%nvg_attr_match("wl$0_ssid$1_hide", "wl$0_basic_rule", "$1", "hidden", "1", "checked");%>><% lang("Disable"); %></input>
  <br/>

  <div class="label"><font id="wifi_mul"><%lang("Wi-Fi Multimedia");%> (WMM)</font></div>
  <input type=radio name="wl$0_ssid$1_wme_enable" id="wl$0_ssid$1_wme_enable" value="1" <%nvg_attr_match("wl$0_ssid$1_wme_enable", "wl$0_basic_rule", "$1", "wme", "1", "checked");%>><font id="wl_ssid_wme_enable"><% lang("Enable"); %></font></input>
  <input type=radio name="wl$0_ssid$1_wme_enable" id="wl$0_ssid$1_wme_enable" value="0" <%nvg_attr_match("wl$0_ssid$1_wme_enable", "wl$0_basic_rule", "$1", "wme", "0", "checked");%>><font id="wl_ssid_wme_disable"><% lang("Disable"); %></font></input>
  <br/>

  <div class="label"><%lang("Wireless Isolation");%></div>
  <input type=radio name="wl$0_ssid$1_isolation_enable" id="wl$0_ssid$1_isolation_enable" value="1" <%nvg_attr_match("wl$0_ssid$1_isolation_enable", "wl$0_basic_rule", "$1", "isolation", "1", "checked");%>><font id="wl_ssid_isolation_enable"><% lang("Enable"); %></font></input>
  <input type=radio name="wl$0_ssid$1_isolation_enable" id="wl$0_ssid$1_isolation_enable" value="0" <%nvg_attr_match("wl$0_ssid$1_isolation_enable", "wl$0_basic_rule", "$1", "isolation", "0", "checked");%>><font id="wl_ssid_isolation_disable"><% lang("Disable"); %></font></input>
  <br/>
  </div>

  <div class="label"><% lang("Security type"); %></div>
  <select name="wl$0_ssid$1_secmode" id="wl$0_ssid$1_secmode" onchange="SelWL(this.form)">
  <option value="disabled" <%nvg_attr_match("wl$0_ssid$1_secmode", "wl$0_sec_rule", "$1", "sec_mode", "disabled", "selected");%>><font id="sec_disable"><% lang("Disable"); %></font></option>
  <option value="wep" <%nvg_attr_match("wl$0_ssid$1_secmode", "wl$0_sec_rule", "$1", "secmode", "wep", "selected");%>><font id="sec_wep"><% lang("WEP"); %></font></option>
  <option value="psk" <%nvg_attr_match("wl$0_ssid$1_secmode", "wl$0_sec_rule", "$1", "secmode", "psk", "selected");%>><font id="sec_psk"><% lang("WPA"); %></font></option>
  <option value="psk2" <%nvg_attr_match("wl$0_ssid$1_secmode", "wl$0_sec_rule", "$1", "secmode", "psk2", "selected");%>><font id="sec_psk2"><% lang("WPA2"); %></font></option>
  </select><br>
  <div class="label">&nbsp</div> <div style="color: red;">( <font id="recommend"><% lang("It's recommended to use WPA2 for security."); %></font> )</div>

  <% wlv_sec_show_setting($0, $1);%>

  </div>
<br/>

