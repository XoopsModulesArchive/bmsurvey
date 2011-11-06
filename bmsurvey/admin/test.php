<?php

// $Id$

// Written by James Flemer
// <jflemer@alum.rpi.edu>

/* phpESP System Information */

include '../../../include/cp_header.php';
if(
	(!defined('XOOPS_ROOT_PATH')) || 
	(!is_object($xoopsUser)) || 
	(!$xoopsUser->isAdmin()) ){
	exit();
}
xoops_cp_header();

include 'adminmenu.php';
echo "<div style='float: left; width:100%;'>";


//session_start();
if (!isset($_SESSION['esp_counter']))
    $_SESSION['esp_counter'] = 0;
$_SESSION['esp_counter']++;

function _pass($str)
{
    echo '<font color="green">' . htmlspecialchars($str) . '</font>';
}

function _fail($str)
{
    echo '<font color="red">' . htmlspecialchars($str) . '</font>';
}

function check_string($have, $want)
{
    if (strcasecmp($have, $want) == 0)
        _pass($have);
    else
        _fail($have);
}

function check_bool($have, $want)
{
    $val = $have ? 'Yes' : 'No';
    if ($have == $want)
        _pass($val);
    else
        _fail($val);
}

function check_extension($ext)
{
    if (!isset($GLOBALS['php_extensions'])) {
        $GLOBALS['php_extensions'] =
                array_map('strtolower', get_loaded_extensions());
    }
    
    if (in_array(strtolower($ext), $GLOBALS['php_extensions']))
        _pass('Yes');
    else
        _fail('No');
}

function check_version()
{
    if (!function_exists('version_compare')) {
        _fail(PHP_VERSION);
        return;
    }
    if (version_compare(PHP_VERSION, '4.1.0', 'ge'))
        _pass(PHP_VERSION);
    else
        _fail(PHP_VERSION);
}

?>
<table><tbody align="left">
  <tr><th>PHP Information</th></tr>
  <tr><td><ul>
    <li>Version: <?php check_version(); ?></li>
    <li>OS: <?php _pass(PHP_OS); ?></li>
    <li>SAPI: <?php check_string(php_sapi_name(), 'apache'); ?></li>
    <li>register_globals: <?php check_bool(ini_get('register_globals'), false); ?></li>
    <li>magic_quotes_gpc: <?php check_bool(ini_get('magic_quotes_gpc'), false); ?></li>
    <li>magic_quotes_runtime: <?php check_bool(ini_get('magic_quotes_runtime'), false); ?></li>
    <li>safe_mode: <?php check_bool(ini_get('safe_mode'), false); ?></li>
    <li>open_basedir: <?php check_string(ini_get('open_basedir'), ''); ?></li>
  </ul></td></tr>
  
  <tr><th>PHP Extensions</th></tr>
  <tr><td><ul>
    <li>dBase: <?php check_extension('dbase'); ?></li>
    <li>GD: <?php
        check_extension('gd');
        if (function_exists('gd_info')) {
            $gdinfo = gd_info();
            echo " -- ${gdinfo['GD Version']}";
        }
    ?></li>
    <li>GNU Gettext: <?php check_extension('gettext'); ?></li>
    <li>LDAP: <?php check_extension('ldap'); ?></li>
    <li>MySQL: <?php check_extension('mysql'); ?></li>
    <li>PHP Extension Dir (compiled): <?php _pass(PHP_EXTENSION_DIR); ?></li>
    <li>PHP Extension Dir (run time): <?php _pass(ini_get('extension_dir')); ?></li>
  </ul></td></tr>

  <tr><th>phpESP Settings</th></tr>
  <tr><td><ul>
    <li>Expected ESP_BASE: <?php _pass(dirname(dirname(__FILE__)) .'/'); ?></li>
    <li>Expected base_url: <?php _pass('http://' . $_SERVER['HTTP_HOST'] . dirname(dirname($_SERVER['REQUEST_URI'])) . '/'); ?></li>
    <li><b>Loading phpESP.ini.php ...</b><br />
      <?php require_once('phpESP.ini.php'); ?></li>
    <li>ESP_BASE: <?php
        if ((ESP_BASE == dirname(__FILE__) . '/../') || (ESP_BASE == dirname(dirname(__FILE__)) .'/'))
            _pass(ESP_BASE);
        else
            _fail(ESP_BASE);
    ?></li>
    <li>base_url: <?php check_string($ESPCONFIG['base_url'], 'http://' . $_SERVER['HTTP_HOST'] . dirname(dirname($_SERVER['REQUEST_URI'])) . '/'); ?></li>
    <li>Version: <?php _pass($ESPCONFIG['version']); ?></li>
    <li>Debug: <?php check_bool($ESPCONFIG['DEBUG'], false); ?></li>
  </ul></td></tr>

  <tr><th>phpESP Language Settings</th></tr>
  <tr><td><ul>
    <li>GNU Gettext: <?php check_string(
            ($ESPCONFIG['gettext'] ? 'Real' : 'Emulated'), 'Real'); ?></li>
    <li>default_lang: <?php _pass($ESPCONFIG['default_lang']); ?></li>
    <li>current lang: <?php _pass($ESPCONFIG['lang']); ?></li>
    <li>available langs: <?php _pass(implode(', ', esp_getlocales())); ?><br />
      (<?php _pass(implode(', ', array_keys(esp_getlocale_map()))); ?>)
    </li>
    <li>GNU Gettext test: <?php
        esp_setlocale('en_US');
        check_string( _GT_Gettext_Test_Failed , 'Passed'); ?></li>
    <li>Catalog Open Test: <?php
        $ret = fopen($ESPCONFIG['locale_path'] . '/en_US/LC_MESSAGES/messages.mo', 'r');
        check_bool($ret !== false, true);
        fclose($ret);
    ?></li>
  </ul></td></tr>

  <tr><th>PHP Session Test</th></tr>
  <tr><td><ul>
    <li>session.save_path: <?php
        if (eregi( '^win(.*)',PHP_OS) && (substr(ini_get('session.save_path'), 0, 1) == '/'))
            _fail(ini_get('session.save_path'));
        else
            _pass(ini_get('session.save_path'));
    ?></li>
    <li>Counter: <?php echo $_SESSION['esp_counter']; ?></li>
  </ul></td></tr>
    
</tbody></table></dev>
<?php
xoops_cp_footer();
?>
