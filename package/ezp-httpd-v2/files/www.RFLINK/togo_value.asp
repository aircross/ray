<html>
<head></head>
<body>
<%togo_show_wan_proto();%>
<input type="hidden" name="drive_name" value="<%nv_attr_get("drive_rule", "0", "name");%>" />
<input type="hidden" name="camera_user" value="<%nv_attr_get("camera_rule", "0", "user");%>" />
<input type="hidden" name="fw_version" value="<%nv_get("fw_version");%>" />
<input type="hidden" name="custom_fw_version" value="<%nv_get("custom_fw_version");%>" />
<input type="hidden" name="model" value="<%nv_get("model");%>" />
<input type="hidden" name="wan0_ipaddr" value="<%nv_get("wan0_ipaddr");%>" />
<input type="hidden" name="wan0_dns" value="<%nv_get("wan0_dns");%>" />
<input type="hidden" name="wan0_gateway" value="<%nv_get("wan0_gateway");%>" />
<input type="hidden" name="wan0_mask" value="<%nv_get("wan0_mask");%>" />
<input type="hidden" name="lan0_ipaddr" value="<%nv_get("lan0_ipaddr");%>" />
<input type="hidden" name="lan0_mask" value="<%nv_get("lan0_mask");%>" />
<input type="hidden" name="wl0_sec_rule" value="<%nv_get("wl0_sec_rule");%>" />
<input type="hidden" name="ssid" value="<%nv_attr_get("wl0_ssid_rule","0","ssid");%>" />
<input type="hidden" name="w_enable" value="<%nv_attr_get("countdown_rule","0","w_enable");%>" />
<input type="hidden" name="w_countdown" value="<%nv_attr_get("countdown_rule","0","w_countdown");%>" />
<input type="hidden" name="p_enable" value="<%nv_attr_get("countdown_rule","0","p_enable");%>" />
<input type="hidden" name="p_countdown" value="<%nv_attr_get("countdown_rule","0","p_countdown");%>" />
<input type="hidden" name="wl0_sec_rule" value="<%nv_get("wl0_sec_rule");%>"/>
</body>
