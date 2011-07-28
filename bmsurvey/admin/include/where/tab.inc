<?php

# $Id: tab.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

/* This function is a hack to handle input-type=image tags.
 * (e.g.  <input type="image" name="tab" src="clickme.gif"> )
 * You cannot set a 'value' attribute in such a tag. When you
 * click on an image, browsers create two form variables based
 * on the 'name' attribute: <name>_x & <name>_y, the coordinates
 * of where the user clicked on the image.
 *
 * To make this work with my 'where=<module>' wrapper, this
 * function checks for certain variables of the form "blah_x"
 * and "blah_y" and includes 'blah'.
 */

	/*
	session_register('survey_id');
	session_register('new_survey');
	session_register('last_tab');
	session_register('curr_q');
	*/
	/* Use this hack to work around how PHP handles session vars.
	 * When Register_Globals is on, the value of the global $foo
	 * is saved, and $_SESSION['foo'] is ignored.
	 * When Register_Globals is off, the global $foo is ignored
	 * and $_SESSION['foo'] is saved.
	 * By making $_SESSION['foo'] a reference to $foo
	 * when reg_glob is on, we can use $_SESSION uniformly
	 * from here on, regardless of the INI settings.
	 */
	if(ini_get('register_globals')) {
		$_SESSION['survey_id']  = &$survey_id;
		$_SESSION['new_survey'] = &$new_survey;
		$_SESSION['last_tab']   = &$last_tab;
		$_SESSION['curr_q']     = &$curr_q;
	}
	
	global $errstr;
	
	if(@$_GET['where'] == 'new') {
		$_SESSION['survey_id']  = '';
		$_SESSION['new_survey'] = '';
		$_SESSION['last_tab']   = '';
		$_SESSION['curr_q']     = '';
	}		
	
	if(!empty($_GET['newid'])) {
		$_SESSION['survey_id']  = intval($_GET['newid']);
		$_POST['tab']           = 'general';
		$_SESSION['last_tab']   = '';
		$_SESSION['new_survey'] = false;
	}

	if(empty($_SESSION['survey_id'])) {
		$_SESSION['survey_id']  = '';
		$_SESSION['new_survey'] = true;
	}

	if(empty($_SESSION['new_survey']))
		$_SESSION['new_survey'] = empty($_SESSION['survey_id']);

	/* check ACL to see if user is allowed to design
	 * _this_ survey */
	if(!empty($_SESSION['survey_id'])) {
		$srealm = auth_get_survey_realm($_SESSION['survey_id']);
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($_SESSION['survey_id'], $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdesign'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_GT_to_access_this_form)) {
			return;
		}
	}

	/* check user's ACL to see if they have rights to
	 * edit/create _any_ survey */
	if($_SESSION['acl']['superuser'] != 'Y' &&
			count($_SESSION['acl']['pdesign']) < 1 &&
			!auth_no_access(_GT_to_access_this_form)) {
		return;
	}
		

	if(empty($ESPCONFIG['tabs'])) {
		echo('<b>'. _GT_No_tabs_defined . '</b>');
		return;
	}
	
	$tab = &$_POST['tab'];

/* XXX I18n do not translate */
	if(stristr($tab,'continue')) {
		switch($_SESSION['last_tab']) {
			case 'general':
				$tab='questions';
				break;
			case 'questions':
				$tab='order';
				break;
			case 'order':
				$tab='preview';
				break;
			default:
				$tab='general';
		}
	}
/* XXX I18n do not translate */

	if(empty($tab)) {
		foreach($ESPCONFIG['tabs'] as $tab) {
			$x = @$_POST["${tab}_x"];
			$y = @$_POST["${tab}_y"];
			if(!empty($x) && !empty($y)) {
				break;
			}
			$tab = $_SESSION['last_tab'];
		}
	}
	if(empty($tab))
		$tab = 'general';
	$tab = strtolower(ereg_replace(' +','_',$tab));
	if(!ereg('^[A-Za-z0-9_]+$',$tab))	// Valid chars are [A-Za-z0-9_]
		$tab = 'general';
	if(!file_exists($GLOBALS['ESPCONFIG']['include_path'].'/tab/'.$tab.$GLOBALS['ESPCONFIG']['extension']))
		$tab = 'general';
	if(!file_exists($GLOBALS['ESPCONFIG']['include_path'].'/tab/'.$tab.$GLOBALS['ESPCONFIG']['extension'])) {
		echo('<b>'. _GT_Unable_to_open_include_file. '</b>');
		exit;
	}
	
	$errstr = '';
	$copy_question = 0;
	$updated = survey_update($_SESSION['survey_id'],$tab,$_SESSION['last_tab'],$copy_question);
?>
<h2>Survey Design</h2>
<?php 
if(!empty($errstr)) echo(mkerror($errstr)."<br>\n"); ?>
<font size="+2">
<a href="<?php echo($GLOBALS['ESPCONFIG']['ME'] ."?where=help"); ?>" target="_blank"><?php echo(_GT_Help); ?></a>
</font>

<input type="hidden" name="where" value="tab">

<table border="0" cellspacing="0" cellpadding="0" align="center" width="95%">
  <tr align="left" valign="bottom">
    <td>
	  <table border="0" cellspacing="0" cellpadding="0">
		<tr>
		  <td><input type="image" style="border:none" alt="<?php echo(_GT_General); ?>"   name="general"   src="<?php echo($ESPCONFIG['image_url']); ?>general_<?php $a='d'; if($tab == 'general') $a='a'; echo($a); ?>.gif" ></td>
		  <td><input type="image" style="border:none" alt="<?php echo(_GT_Questions); ?>" name="questions" src="<?php echo($ESPCONFIG['image_url']); ?>questions_<?php $a='d'; if($tab == 'questions') $a='a'; echo($a); ?>.gif" ></td>
		  <td><input type="image" style="border:none" alt="<?php echo(_GT_Order); ?>"     name="order"     src="<?php echo($ESPCONFIG['image_url']); ?>order_<?php $a='d'; if($tab == 'order') $a='a'; echo($a); ?>.gif" ></td>
		  <td><input type="image" style="border:none" alt="<?php echo(_GT_Preview); ?>"   name="preview"   src="<?php echo($ESPCONFIG['image_url']); ?>preview_<?php $a='d'; if($tab == 'preview') $a='a'; echo($a); ?>.gif" ></td>
		  <td><input type="image" style="border:none" alt="<?php echo(_GT_Finish); ?>"    name="finish"    src="<?php echo($ESPCONFIG['image_url']); ?>finish_<?php $a='d'; if($tab == 'finish') $a='a'; echo($a); ?>.gif" ></td>
		</tr>
	  </table>
    </td>
  </tr>
  <tr valign="top" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>">
    <td>
	<?php
	//echo "Tab include:".$GLOBALS['ESPCONFIG']['include_path'].'/tab/'.$tab.$GLOBALS['ESPCONFIG']['extension'];
	include($GLOBALS['ESPCONFIG']['include_path'].'/tab/'.$tab.$GLOBALS['ESPCONFIG']['extension']);
	$_SESSION['last_tab'] = $tab;
	?>
    </td>
  </tr>
</table>
<?php 
if($tab=='finish') {echo('&nbsp;'); return;} 
if($_SESSION['new_survey']) { 
	echo(_GT_Click_cancel_to_cancel); 
	echo '<br><input type="submit" name="where" value="Cancel">';
	if($tab=='preview') { 
		echo '<input type="submit" name="tab" value="Finish">';
		return; 
	}
	echo '&nbsp;<input type="submit" name="tab" value="Continue &gt;">';
	return; 
}
?>
<p><?php echo(_GT_The_survey_title_and_other); ?><br>
<a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=help"); ?>" target="_blank"><?php echo(_GT_Click_here_to_open_the_Help_window); ?></a></p>
<input type="submit" name="tab" value="General">
<input type="submit" name="tab" value="Questions">
<input type="submit" name="tab" value="Order">
<input type="submit" name="tab" value="Preview">
<input type="submit" name="tab" value="Finish">
