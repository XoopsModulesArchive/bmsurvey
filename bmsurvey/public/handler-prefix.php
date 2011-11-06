<?php

# $Id$

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

/* When using the authentication for responses you need to include
 * part of the script *before* your template so that the
 * HTTP Auth headers can be sent when needed.
 *
 * So, create your template file with this as the *first*
 * line:
 *   <?php $sid=<SID>; include('.../phpESP/public/handler-prefix.php'); ?>
 * then where you want to include the actual survey use this
 * line:
 *   <?php inlcude('.../phpESP/public/handler.php'); ?>
 *
 * For example, here is a very small template:
 *   <?php $sid=42; include('phpESP/public/handler-prefix.php'); ?>
 *   <html>
 *   <head><title>Example.com</title></head>
 *   <body>
 *   <table border="0" width="800">
 *   <tr><td colspan="2"><img src="example-banner.png"
 *       border="0" width="800" height="42"></td></tr>
 *   <tr><td><?php inlcude('example-sidebar.php'); ?></td>
 *       <td><?php include('phpESP/public/handler.php'); ?>
 *       </td>
 *   </tr>
 *   <tr><td colspan="2"><p align="right"><font size="-2">
 *       Copyright &copy; 2003. Example.com</font></p></td></tr>
 *   </table>
 *   </body>
 *   </html>
 *
 */

    if (defined('ESP-HANDLER-PREFIX'))
        return;

    define('ESP-HANDLER-PREFIX', true);

    if (!defined('ESP_BASE'))
        define('ESP_BASE', dirname(dirname(__FILE__)) .'/');

    require_once(ESP_BASE . '/conf.php');
    require_once(ESP_BASE . '/admin/phpESP.ini.php');
    require_once($ESPCONFIG['include_path']."/funcs".$ESPCONFIG['extension']);

    $GLOBALS['errmsg'] = '';

    if(isset($_GET['sid'])) {
        $GLOBALS['errmsg'] = "handler-prefix[54]".mkerror(_GT_Error_processing_survey_Security_violation);
        return;
    }

    if(isset($_GET['results']) || isset($_POST['results'])) {
        $GLOBALS['errmsg'] = "handler-prefix[59]".mkerror(_GT_Error_processing_survey_Security_violation);
        return;
    }

    if (isset($sid) && !empty($sid))
        $sid = intval($sid);
    else if (isset($_POST['sid']) && !empty($_POST['sid']))
        $sid = intval($_POST['sid']);

    if(!isset($sid) || empty($sid)) {
        $GLOBALS['errmsg'] = "handler-prefix[69]".mkerror( _GT_Survey_not_specified );
        return;
    }

    if(empty($_POST['userid'])) {
        // find remote user id (takes the first non-empty of the following)
        //  1. a GET variable named 'userid'
        //  2. the REMOTE_USER set by HTTP-Authentication
        //  3. the query string
        //  4. the remote ip address
        if (!empty($_GET['userid'])) {
            $_POST['userid'] = $_GET['userid'];
        } elseif(!empty($_SERVER['REMOTE_USER'])) {
            $_POST['userid'] = $_SERVER['REMOTE_USER'];
        } elseif(!empty($_SERVER['QUERY_STRING'])) {
            $_POST['userid'] = urldecode($_SERVER['QUERY_STRING']);
        } else {
            $_POST['userid'] = $_SERVER['REMOTE_ADDR'];
        }
    }

    if(empty($_POST['referer']))
        $_POST['referer'] = isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '';
	$_POST['referer'] = htmlspecialchars ( $_POST['referer'] , ENT_QUOTES );

    if (empty($_POST['rid']))
        $_POST['rid'] = '';
    else
        $_POST['rid'] = intval($_POST['rid']) ?
                intval($_POST['rid']) : '';

    if($ESPCONFIG['auth_response']) {
        // check for authorization on the survey
        require_once($ESPCONFIG['include_path']."/lib/espauth".$ESPCONFIG['extension']);
        $espuser = ''; $esppass = '';
        if (isset($_SERVER['PHP_AUTH_USER']))
            $espuser = $_SERVER['PHP_AUTH_USER'];
        if (isset($_SERVER['PHP_AUTH_PW']))
            $esppass = $_SERVER['PHP_AUTH_PW'];

        if(!survey_auth($sid, addslashes($espuser), addslashes($esppass))) return;

        if (auth_get_option('resume')) {
            $_POST['rid'] = auth_get_rid($sid, addslashes($espuser),
                    $_POST['rid']);

            if (!empty($_POST['rid']) && (empty($_POST['sec']) ||
                    intval($_POST['sec']) < 1))
            {
                $_POST['sec'] = response_select_max_sec($sid,
                        $_POST['rid']);
            }
        }
    }

    if (empty($_POST['sec']))
        $_POST['sec'] = 1;
    else
        $_POST['sec'] = (intval($_POST['sec']) > 0) ?
                intval($_POST['sec']) : 1;

    define('ESP-AUTH-OK', true);

?>
