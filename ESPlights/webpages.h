const char page_header_start[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>ESPlights</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.10">
  <meta name='mobile-web-app-capable' content='yes'>
)=====";

const char page_header_refresh[] PROGMEM = R"=====(
<meta http-equiv="refresh" content="10"; url="/">
)=====";

const char page_header_end[] PROGMEM = R"=====(
  <meta name='theme-color' content='#000'>
  <link rel='icon' sizes='256x256' href='https://esp.3dstar.ro/favicon.png'>
</head>
<body style="background-color:#000;color:#fff;text-align:center;">
)=====";

const char page_footer[] PROGMEM = R"=====(
<hr><br>
<a href="https://3dstar.ro/">3DStar</a> -
<a href="https://3dstar.ro/proiecte/esplights">ESPlights</a> -
<a href="https://github.com/cctweaker/ESPlights">GitHub</a>
<br><br><br>
</body>
</html>
)=====";

const char page_content_title[] PROGMEM = R"=====(
<h2>ESPlights</h2>
)=====";

const char page_content_not_found[] PROGMEM = R"=====(
<br><br><h3>Page not found<br><br>Click button or wait for page reload.</h3><br><br>
)=====";

const char page_content_restart[] PROGMEM = R"=====(
<br><br><h3>Device restart<br><br>Click button or wait for page reload.</h3><br><br>
)=====";

const char page_content_update[] PROGMEM = R"=====(
<br><br><h3>Performing firmware update...<br><br>
Click button or wait for page reload.</h3>
Software updates take about 30-40 seconds.<br><br>
)=====";

const char page_content_main_menu_button[] PROGMEM = R"=====(
<form action="/"><input type="submit" value="Main menu"></form>
)=====";

const char form_buttons[] PROGMEM = R"=====(
<input type="submit" value="Submit"><br><br><input type="submit" name="save" value="Save config"></form>
<br><form action="/"><input type="submit" value="Back"></form><br>
)=====";

const char html_hr[] PROGMEM = R"=====(
<hr>
)=====";

const char html_table_i[] PROGMEM = R"=====(
<table align="center"><tr>
)=====";

const char html_table_s[] PROGMEM = R"=====(
</tr></table>
)=====";

const char html_table_tr[] PROGMEM = R"=====(
</tr><tr>
)=====";

const char html_table_tr_hr[] PROGMEM = R"=====(
</tr><tr><td colspan="2"><hr></td></tr><tr>
)=====";

const char form_start[] PROGMEM = R"=====(
<form>
)=====";

const char form_text_field_string[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%s"><br><br>
)=====";

const char form_text_field_string_min[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%s" minlength="1"><br><br>
)=====";

const char form_text_field_int[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%d"><br><br>
)=====";

const char form_text_field_int_min[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%d" minlength="1"><br><br>
)=====";

const char form_text_field_hex4[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%04X"><br><br>
)=====";

const char form_text_field_hex6[] PROGMEM = R"=====(
%s<br><input type="text" name="%s" value="%06X"><br><br>
)=====";

const char form_select_field[] PROGMEM = R"=====(
%s<br><select name="%s">
)=====";

const char form_select_field_end[] PROGMEM = R"=====(
</select><br><br>
)=====";

const char form_option_field[] PROGMEM = R"=====(
<option value="%d">%s</option>
)=====";

const char form_option_field_selected[] PROGMEM = R"=====(
<option value="%d" selected>%s</option>
)=====";

const char form_yes_no[] PROGMEM = R"=====(
%s<br><select name="%s"><option value="1" %s>yes</option><option value="0" %s>no</option></select><br><br>
)=====";

const char form_add_del_field[] PROGMEM = R"=====(
<input type="submit" name="%sadd" value="Add"><input type="submit" name="%sdel" value="Remove"><br><br>
)=====";

const char form_light_field[] PROGMEM = R"=====(
Light %d channel: <input type="text" name="light%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_simple_field[] PROGMEM = R"=====(
Simple %d channel: <input type="text" name="simple%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_shutter_up_field[] PROGMEM = R"=====(
UP channel: <input type="text" name="shup%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_shutter_down_field[] PROGMEM = R"=====(
DOWN channel: <input type="text" name="shdw%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_shutter_tmt_field[] PROGMEM = R"=====(
Timeout: <input type="text" name="shtmt%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_timed_field[] PROGMEM = R"=====(
Channel: <input type="text" name="timed%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_timed_tmt_field[] PROGMEM = R"=====(
Timeout: <input type="text" name="timedtmt%d" value="%d" minlength="1" size="4"><br>
)=====";

const char form_timeout_select[] PROGMEM = R"=====(
Units: <select name="%s%d">
<option value="1" %s>millisecond(s)</option>
<option value="1000" %s>second(s)</option>
<option value="60000" %s>minute(s)</option>
<option value="3600000" %s>hour(s)</option>
<option value="86400000" %s>day(s)</option>
</select><br><br>
)=====";

const char form_gmac_field[] PROGMEM = R"=====(
<input type="text" name="gmac%d" value="%02X" minlength="1">%s
)=====";

const char form_umac_field[] PROGMEM = R"=====(
<input type="text" name="umac%d" value="%02X" minlength="1">%s
)=====";

const char form_kok_field[] PROGMEM = R"=====(
<input type="text" name="kok%d" value="%02X" minlength="1">
)=====";

const char form_key_field[] PROGMEM = R"=====(
<input type="text" name="key%d" value="%02X" minlength="1">
)=====";

const char form_end[] PROGMEM = R"=====(
</form>
)=====";

const char menu_entry[] PROGMEM = R"=====(
<form action="%s"><input type="submit" value="%s" %s></form><br>
)=====";

const char menu_entry_scan_wifi[] PROGMEM = R"=====(
<form><input type="submit" name="scan" value="Scan WiFi"></form><br>
)=====";

const char menu_entry_reload_page[] PROGMEM = R"=====(
<form><input type="submit" value="Reload page"></form><br>
)=====";

const char menu_entry_check_restart[] PROGMEM = R"=====(
onSubmit="if(!confirm(\'Are you sure you wish to restart the device?\')){return false;}"
)=====";

const char menu_entry_check_erase[] PROGMEM = R"=====(
onSubmit="if(!confirm(\'Are you sure you wish to erase all settings?\')){return false;}"
)=====";

const char menu_entry_check_update[] PROGMEM = R"=====(
onSubmit="if(!confirm(\'Are you sure you wish to update the firmware?\')){return false;}"
)=====";

const char littlefs_failure[] PROGMEM = R"=====(
LittleFS failure!
)=====";

const char file_write_failure[] PROGMEM = R"=====(
File write failure!
)=====";

const char file_write_success[] PROGMEM = R"=====(
File write success.
)=====";