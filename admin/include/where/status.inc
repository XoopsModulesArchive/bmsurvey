<?php

# $Id: status.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	if(!empty($_GET['sid']))
		$sid = intval($_GET['sid']);

	$bg = '';

	/* operation selected ... */
	if(!empty($_GET['op'])) {
		$sql = "SELECT status,owner,realm FROM ".TABLE_SURVEY." WHERE id='${sid}'";
		$result = mysql_query($sql);
		if((list($old_status, $owner, $realm) = mysql_fetch_row($result))) {
			$access = false;
			$err = false;
			$status = $old_status;
			// trying to perform some operation
			switch(strtolower($_GET['op'])) {
				case 'c':	// Clear
					/* only _superuser_s can do this */
					if($xoopsUser->isAdmin()) {
						$access = true;
					} else {
						$access = false;
					}
					$status = 0;
					break;
				case 't':	// test
					/* only the owner or a group editor+design */
					if($owner == $xoopsUser->uid() ||
						in_array($realm, array_intersect(
							$_SESSION['acl']['pall'],
							$_SESSION['acl']['pdesign']))) {
						$access = true;
					}
					$status = $old_status | STATUS_TEST;
					if($old_status & ( STATUS_DELETED | STATUS_DONE | STATUS_ACTIVE ) )
						$err = true;
					break;
				case 'm':	// Edit
					/* only the owner or a group editor+design */
					if($owner == $xoopsUser->uid() ||
						in_array($realm, array_intersect(
							$_SESSION['acl']['pall'],
							$_SESSION['acl']['pdesign']))) {
						$access = true;
					}
					$status = $old_status & ~STATUS_TEST;
					if($old_status & ( STATUS_DELETED | STATUS_DONE | STATUS_ACTIVE ) )
						$err = true;
					else
						response_delete_all($sid);
					break;
				case 'a':	// activate
					/* only the owner+stauts or a group editor+status */
					if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}
					$status = STATUS_ACTIVE;	//$old_status | STATUS_ACTIVE;
					if($old_status & ( STATUS_DELETED | STATUS_DONE ) )
						$err = true;
					else
						response_delete_all($sid);
					break;
				case 'e':	// End
					/* only the owner+stauts or a group editor+status */
					if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}
					$status = $old_status | STATUS_DONE;
					if($old_status & STATUS_DELETED )
						$err = true;
					break;
				case 'd':	// Delete
					/* only the owner+stauts or a group editor+status */
					if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}
					$status = $old_status | STATUS_DELETED;
					break;
				case 'r':	// Re Activate
					/* only the owner+stauts or a group editor+status */
					if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}
					$status = STATUS_ACTIVE;	//$old_status | STATUS_ACTIVE;
					if(!$old_status & ( STATUS_DELETED | STATUS_DONE ) )
						$err = true;
					echo $status.$err;
					break;
			}
			/* superuser overrides all */
			if($xoopsUser->isAdmin()) $access = true;
			$sql = "UPDATE ".TABLE_SURVEY." SET status='${status}' WHERE id='${sid}'";
			if($access || auth_no_access(_('to access this survey'))) {
				if(!$err) {
					mysql_query($sql);
				} else {
					mkwarn(_('Can not set survey status.'));
					mkerror(_('Status') .': '. $old_status);
				}
			}
		}
	}
?>
<h2><?php mb_echo('Survey Status'); ?></h2>
<div align="left">

<ul>
<li><?php mb_echo('<b>Test</b> transitions a survey into testing mode. At which point you may perform a live test by taking the survey, and viewing the results. You will not be able to make any further changes to the survey once you have switched to test mode.'); ?></li>
<li><?php mb_echo('<b>Activate</b> transitions a survey into active mode. In this mode the survey is open for production use, and may be put online. This will clear any results from testing mode (if any). No further editing of survey is allowed.'); ?></li>
<li><?php mb_echo('<b>End</b> transitions a survey into ended mode. In this mode, no edits are possible, no users may take the survey (it is inactive), but results are still viewable from the results menu.'); ?></li>
<li><?php mb_echo('<b>Archive</b> removes this survey. It is still stored in the database, but no further interaction is allowed. You may <b>not</b> view the results of an archived survey.'); ?></li>
</ul>
</div>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>&nbsp;/&nbsp;\n"); ?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=purge\">" . mb_('Purge Surveys') . "</a>\n"); ?>
<table border="0" align="center" cellspacing="0" cellpadding="4" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php mb_echo('ID'); ?></th>
		<th align="left"><?php mb_echo('Name'); ?></th>
		<th align="left"><?php mb_echo('Title'); ?></th>
		<th align="left"><?php mb_echo('Owner'); ?></th>
		<th align="left"><?php mb_echo('Group'); ?></th>
		<th align="left"><?php mb_echo('Status'); ?></th>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
<?php
	/* load names and titles of all surveys available to
	 * _this_ user */
	if($xoopsUser->isAdmin()) {
		$sql = "SELECT id,name,title,status,owner,realm FROM ".TABLE_SURVEY." ORDER BY id DESC";
	} else {
		$realms = array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				array_merge(
					$_SESSION['acl']['pall'],
					$_SESSION['acl']['pdesign'])));
		$sql = "SELECT id,name,title,status,owner,realm
			FROM ".TABLE_SURVEY." WHERE (owner = ".
			$xoopsUser->uid()." || realm $realms) ORDER BY id DESC";
			//$xoopsUser->uid() ."' || realm $realms) ORDER BY id DESC";
	}
	$result = mysql_query($sql);

	$realms = array_intersect(
			$_SESSION['acl']['pstatus'],
			array_merge(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pstatus']));

	while(list($sid,$name,$title,$status,$owner,$realm) = mysql_fetch_row($result)) {
		$stat = mb_('Editing');
		$test = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=t&sid=${sid}\">". mb_('Test') ."</a>";
		$act  = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=a&sid=${sid}\">". mb_('Activate') ."</a>";
		$done = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=e&sid=${sid}\">". mb_('End') ."</a>";
		$del  = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=d&sid=${sid}\">". mb_('Archive') ."</a>";

		if($status & STATUS_DELETED) {
			$stat = mb_('Archived');
			$test = $act = $done = $del = '&nbsp;';
			continue;
		} elseif($status & STATUS_DONE) {
			$stat = mb_('Ended');
			$test = $act = $done = '&nbsp;';
		} elseif($status & STATUS_ACTIVE) {
			$stat = mb_('Active');
			$test = $act = '&nbsp;';
		} elseif($status & STATUS_TEST) {
			$stat = mb_('Testing');
			$done = '&nbsp;';
			$test = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=m&sid=${sid}\">". mb_('Edit') ."</a>";
		} else {
			$done = '&nbsp;';
		}

		/* whack things back to permissions set by ACL
		 * for everyone _not_ superuser */
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!in_array($realm, $realms)) {
			$act  = '&nbsp;';
			$done = '&nbsp;';
			$del  = '&nbsp;';
		}

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
		<td><?php echo($stat); ?></td>
		<td><?php echo($test); ?></td>
		<td><?php echo($act); ?></td>
		<td><?php echo($done); ?></td>
		<td><?php echo($del); ?></td>
	</tr>
<?php
	}
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
