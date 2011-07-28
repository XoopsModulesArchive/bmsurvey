<?php
# $Id: handler.php,v 1.1.1.1 2005/08/10 12:14:04 yoshis Exp $
// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>
/* When using the authentication for responses you need to include
 * part of the script *before* your template so that the
 * HTTP Auth headers can be sent when needed.
 *
 * See the handler-prefix.php file for details.
 */
global $xoopsTpl;
global $_POST;
global $_SERVER;

	if (!defined('ESP_BASE')) define('ESP_BASE', dirname(dirname(__FILE__)) .'/');
	require_once(ESP_BASE . '/admin/phpESP.ini.php');
	require_once($ESPCONFIG['include_path']."/funcs".$ESPCONFIG['extension']);
	require_once($ESPCONFIG['handler_prefix']);
	if(!defined('ESP-AUTH-OK')) {
		if (!empty($GLOBALS['errmsg'])) echo("handler.php[17]".$GLOBALS['errmsg']);
		return;
	}
    //esp_init_db();
	if (empty($_POST['referer'])) $_POST['referer'] = '';
	// show results instead of show survey
	// but do not allow getting results from URL or FORM
	if(isset($results) && $results) {
		// small security issue here, anyone could pick a QID to crossanalyze
		survey_results($sid,$precision,$totals,$qid,$cids);
		return;
	}
	// else draw the survey
	$sql = "SELECT status, name FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	$result = mysql_query($sql);
    if ($result && mysql_num_rows($result) > 0){
    	list ($status, $name) = mysql_fetch_row($result);
    }else
        $status = 0;

	if($status & ( STATUS_DONE | STATUS_DELETED )) {
		echo("handler.php[38]".mkerror(_('Error processing survey: Survey is not active.')));
		return;
	}
	if(!($status & STATUS_ACTIVE)) {
		if(!(isset($test) && $test && ($status & STATUS_TEST))) {
			echo("handler.php[43]".mkerror(_('Error processing survey: Survey is not active.')));
			return;
		}
	}

    if ($_POST['referer'] == $ESPCONFIG['autopub_url'])
        $_POST['referer'] .= "?name=$name";

	$num_sections = survey_num_sections($sid);

	$msg = '';
	//$action = 'http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
	$action = $_SERVER['PHP_SELF'];
	if (!empty($_SERVER['QUERY_STRING']))
		$action .= "?" . $_SERVER['QUERY_STRING'];
	if(!empty($_POST['submit'])) {
		$msg = response_check_required($sid,$_POST['sec']);
		if(empty($msg)) {
            if ($ESPCONFIG['auth_response'] && auth_get_option('resume'))
                response_delete($sid, $_POST['rid'], $_POST['sec']);
			$_POST['rid'] = response_insert($sid,$_POST['sec'],$_POST['rid']);
			response_commit($_POST['rid']);
			response_send_email($sid,$_POST['rid']);
			$msg = goto_thankyou($sid,$_POST['referer']);
			if (!$msg) $msg=_MD_BMSURVEY_THANKS_ENTRY;
			redirect_header($_POST['referer'],2,$msg);	//$referer
			return;
		}
	}

	if(!empty($_POST['resume']) && $ESPCONFIG['auth_response'] && auth_get_option('resume')) {
        response_delete($sid, $_POST['rid'], $_POST['sec']);
		$_POST['rid'] = response_insert($sid,$_POST['sec'],$_POST['rid']);
        if ($action == $ESPCONFIG['autopub_url'])
    		goto_saved("$action?name=$name");
        else
            goto_saved($action);
		return;
	}

	if(!empty($_POST['next'])) {
		$msg = response_check_required($sid,$_POST['sec']);
		if(empty($msg)) {
            if ($ESPCONFIG['auth_response'] && auth_get_option('resume'))
                response_delete($sid, $_POST['rid'], $_POST['sec']);
			$_POST['rid'] = response_insert($sid,$_POST['sec'],$_POST['rid']);
			$_POST['sec']++;
		}
	}
	
	if (!empty($_POST['prev']) && $ESPCONFIG['auth_response'] && auth_get_option('navigate')) {
		if(empty($msg)) {
            if (auth_get_option('resume'))
                response_delete($sid, $_POST['rid'], $_POST['sec']);
			$_POST['rid'] = response_insert($sid,$_POST['sec'],$_POST['rid']);
			$_POST['sec']--;
		}
	}
    
    if ($ESPCONFIG['auth_response'] && auth_get_option('resume'))
        response_import_sec($sid, $_POST['rid'], $_POST['sec']);

	$xoopsTpl->assign('formheader', array(
		'action' => $action,
		'referer' => htmlspecialchars($_POST['referer']),
		'userid' => $_POST['userid'],
		'sid' => $sid,
		'rid' => $_POST['rid'],
		'sec' => $_POST['sec'])
	);
	/*
	** Display form body. Call Render with Smarty
	*/
	$xoopsTpl->assign('formbody',survey_render_smarty($sid,$_POST['sec'],$msg));

	if ($ESPCONFIG['auth_response']) {
		if (auth_get_option('navigate') && $_POST['sec'] > 1) { 
			$xoopsTpl->assign('auth_response','<input type="submit" name="prev" value="Previous Page">');
		}
		if (auth_get_option('resume')) {
			$xoopsTpl->assign('auth_response','<input type="submit" name="resume" value="Save">');
		}
	}
	if($_POST['sec'] == $num_sections)	{
		$xoopsTpl->assign('formfooter', array('name' =>'submit','value'=>_MD_SUBMIT_SURVEY));	// mb_('Submit Survey')
 	} else {
		$xoopsTpl->assign('formfooter', array('name' =>'next','value'=>_MD_NEXT_PAGE));	//mb_('Next Page')
	}
?>
