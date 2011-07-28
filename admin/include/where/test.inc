<!--</td></tr></table></form></td></tr></table>-->
<?php

# $Id: test.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	/* get the sid ... if any */
	if(!empty($_GET['sid']))
	    $sid = intval($_GET['sid']);

	if(!empty($_POST['sid']))
		$sid = intval($_POST['sid']);

    $bg = '';
	/* sid selected for testing */
	if(!empty($sid)) {
		$sql = "SELECT status,owner,realm,theme FROM ".TABLE_SURVEY." WHERE id=${sid}";
		$result = mysql_query($sql);
		list($status,$owner,$realm,$theme) = mysql_fetch_row($result);
		mysql_free_result($result);

		/* check ACL to see if user is allowed to test
		 * _this_ survey */
		$realms = array_intersect(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pdesign']);
		if($_SESSION['acl']['superuser'] != 'Y' &&
				$owner != $xoopsUser->uid() &&
				!in_array($realm, $realms) &&
				!auth_no_access(_('to access this survey'))) {
			return;
		}
		if (($status & STATUS_TEST) &&
			!($status & (STATUS_DONE | STATUS_DELETED | STATUS_ACTIVE))) {
			/* cleared for testing ... was that enough tests? */
			/* make the css style available during the testing.
			 * this should really go into the head section ...
			 * but should still work */
			/*
			if (!empty($theme))
				echo('<link rel="stylesheet" href="' .
						$GLOBALS['ESPCONFIG']['css_url'].$theme . '" type="text/css">');
			*/
			$xoopsTpl->assign('test_title',
				mb_('Testing Survey...').mb_('SID')." = $sid");
			/* set things up for the handler to take over ... */
			$test = true;
			unset($_GET['sid']);
			unset($_POST['sid']);
			$_POST['userid'] = 'test';
			define('ESP-HANDLER-PREFIX', true);
			define('ESP-AUTH-OK', true);
			$_POST['sec'] = empty($_POST['sec']) ? 1 : intval($_POST['sec']);
			$_POST['rid'] = empty($_POST['rid']) ? 0 : intval($_POST['rid']);
			/*
			** display form
			*/
			include($ESPCONFIG['handler']);

			//undefine('ESP-HANDLER-PREFIX');
			//undefine('ESP-AUTH-OK');
			$xoopsTpl->assign('view_results',
				"<a href=\"".$ESPCONFIG['ME']."?where=results&sid=".$sid."&test=1\">".
				mb_('View Results')."</a>");
			$xoopsTpl->assign('go_management',
				"<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" 
				. mb_('Go back to Management Interface') . "</a>"); 
			$xoopsOption['template_main'] = 'bmsurvey_test.html';
			mysql_free_result($result);
			return;
		}
	}
	/* no survey selected ... show a menu to choose from */
	/* check with ACL for allowed surveys */
	/* load names and titles of all surveys available to
	 * _this_ user */
	$statusbad = (STATUS_DONE | STATUS_DELETED | STATUS_ACTIVE);
	$statusok = STATUS_TEST;
	if($xoopsUser->isAdmin()) {
		$sql = "SELECT id,name,title,owner,realm FROM ".TABLE_SURVEY." WHERE (status & $statusok) AND NOT (status & $statusbad)
			ORDER BY id DESC";
	} else {
		$realms = array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pdesign']));
		$sql = "SELECT id,name,title,owner,realm FROM ".TABLE_SURVEY." WHERE (status & $statusok) AND NOT (status & $statusbad) AND
			(owner = '". $xoopsUser->uid() ."' || realm $realms)
			ORDER BY id DESC";
	}
	$result = mysql_query($sql);

?>
<h2><?php mb_echo('Test a Survey'); ?></h2>
<?php mb_echo('Pick Survey to Test'); ?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php mb_echo('ID'); ?></th>
		<th align="left"><?php mb_echo('Name'); ?></th>
		<th align="left"><?php mb_echo('Title'); ?></th>
		<th align="left"><?php mb_echo('Owner'); ?></th>
		<th align="left"><?php mb_echo('Group'); ?></th>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<th align="left"><?php mb_echo('Export'); ?></th>
	</tr>
<?php
	while(list($sid,$name,$title,$owner,$realm) = mysql_fetch_row($result)) {

		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><?php echo($sid); ?></td>
		<td><?php echo($name); ?></td>
		<td><?php echo($title); ?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
		<td><a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=test&sid=${sid}"); ?>"><?php mb_echo('Test'); ?></a></td>
		<td><a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=results&test=1&sid=${sid}"); ?>"><?php mb_echo('Results'); ?></a></td>
		<td><a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=export&type=csv&test=1&sid=${sid}"); ?>"><?php mb_echo('CSV'); ?></a>
		    (<a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=download&type=csv&test=1&sid=${sid}"); ?>"><?php mb_echo('download'); ?></a>)
			<!-- / <a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=export&type=dbf&test=1&sid=${sid}"); ?>"><?php mb_echo('DBF'); ?></a> -->
		</td>
	</tr>
<?php
	}
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
