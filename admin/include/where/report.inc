<?php

# $Id: report.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>

	// see if a survey has been selected
	if(!empty($_GET['sid'])) {
		$sid = intval($_GET['sid']);
		if(empty($_GET['details']))
			$_GET['details'] = 1; // by default, show details in report

		/* check ACL to see if user is allowed to view
		 * _this_ survey */
		$srealm = auth_get_survey_realm($sid);
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($sid, $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdesign'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_('to access this form'))) {
			return;
		}
?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
<tr><td>
<?php
		$ret = survey_report($sid, $_GET['details']);
		if($ret != '')
			echo(mkerror($ret));
?>
</td></tr>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=report\">" . mb_('Go back to Report Menu') . "</a>\n"); ?><br>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
<?php
		return;
	}

// No SurveyId specified, so build a table of
// surveys to choose from ...

	$statusbad = (STATUS_DELETED);
	if($xoopsUser->isAdmin()) {
		$sql = "SELECT id,name,title,owner,realm FROM ".TABLE_SURVEY."
			WHERE NOT (status & $statusbad)
			ORDER BY id DESC";
	} else {
		$realms = array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pdesign']));
		$sql = "SELECT id,name,title,owner,realm FROM ".TABLE_SURVEY." WHERE (owner='".
			$xoopsUser->uid() ."' ||
			realm $realms)
			AND NOT (status & $statusbad)
			ORDER BY id DESC";
	}
	$result = mysql_query($sql);

?>
<h2><?php mb_echo('View Form Report'); ?></h2>
<?php mb_echo('Pick Form to View'); ?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php mb_echo('ID'); ?></th>
		<th align="left"><?php mb_echo('Name'); ?></th>
		<th align="left"><?php mb_echo('Title'); ?></th>
		<th align="left"><?php mb_echo('Owner'); ?></th>
		<th align="left"><?php mb_echo('Group'); ?></th>
	</tr>
<?php
    $bg = '';
	while(list($sid, $name, $title, $owner, $realm) = mysql_fetch_row($result)) {
		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><?php echo($sid); ?></td>
		<td><a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=report&sid=${sid}"); ?>">
			<?php echo($name); ?></a>
		</td>
		<td><?php echo($title); ?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
	</tr>
<?php
	}
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
