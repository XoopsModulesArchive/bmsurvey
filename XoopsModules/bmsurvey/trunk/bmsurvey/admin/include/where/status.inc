<?php

# $Id: status.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>
// ----
// For Xoops
//   $owner = Suevey owner xoops user id
//   $realm = Permitted xoops group id at survey edit
//   $checkstatus = Check permission of status change by module admin
function xoopsGroupStatus($owner,$realm,$checkstatus=0){
	global $xoopsUser,$xoopsModuleConfig;

	if ( $xoopsUser->isAdmin() || $owner == $xoopsUser->uid() ) return TRUE;
    $groups = $xoopsUser->getGroups();
    if (in_array($realm ,$groups)){
    	if (!$checkstatus || ( $checkstatus && $xoopsModuleConfig['PSTATUS'])){
			return TRUE;
    	}
    }
	return FALSE;
}
//
// Main proc start
//
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
					/*if($owner == $xoopsUser->uid() ||
						in_array($realm, array_intersect(
							$_SESSION['acl']['pall'],
							$_SESSION['acl']['pdesign']))) {
						$access = true;
					}*/
					$access = xoopsGroupStatus($owner,$realm);
					$status = $old_status | STATUS_TEST;
					if($old_status & ( STATUS_DELETED | STATUS_DONE | STATUS_ACTIVE ) )
						$err = true;
					break;
				case 'm':	// Edit
					/* only the owner or a group editor+design */
					/*if($owner == $xoopsUser->uid() ||
						in_array($realm, array_intersect(
							$_SESSION['acl']['pall'],
							$_SESSION['acl']['pdesign']))) {
						$access = true;
					}*/
					$access = xoopsGroupStatus($owner,$realm);
					$status = $old_status & ~STATUS_TEST;
					if($old_status & ( STATUS_DELETED | STATUS_DONE | STATUS_ACTIVE ) )
						$err = true;
					else
						response_delete_all($sid);
					break;
				case 'a':	// activate
					/* only the owner+stauts or a group editor+status */
					/*if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}*/
					$access = xoopsGroupStatus($owner,$realm,1);
					$status = STATUS_ACTIVE;	//$old_status | STATUS_ACTIVE;
					if($old_status & ( STATUS_DELETED | STATUS_DONE ) )
						$err = true;
					else
						response_delete_all($sid);
					break;
				case 'e':	// End
					/* only the owner+stauts or a group editor+status */
					/*if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}*/
					$access = xoopsGroupStatus($owner,$realm,1);
					$status = $old_status | STATUS_DONE;
					if($old_status & STATUS_DELETED )
						$err = true;
					break;
				case 'd':	// Delete
					/* only the owner+stauts or a group editor+status */
					/*if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}*/
					$access = xoopsGroupStatus($owner,$realm,1);
					$status = $old_status | STATUS_DELETED;
					break;
				case 'r':	// Re Activate
					/* only the owner+stauts or a group editor+status */
					/*if(in_array($realm, $_SESSION['acl']['pstatus']) &&
						($owner == $xoopsUser->uid() ||
						in_array($realm, $_SESSION['acl']['pall']))) {
						$access = true;
					}*/
					$access = xoopsGroupStatus($owner,$realm,1);
					$status = STATUS_ACTIVE;	//$old_status | STATUS_ACTIVE;
					if(!$old_status & ( STATUS_DELETED | STATUS_DONE ) )
						$err = true;
					break;
			}
			/* superuser overrides all */
			if($xoopsUser->isAdmin()) $access = true;
			$sql = "UPDATE ".TABLE_SURVEY." SET status='${status}' WHERE id='${sid}'";
			if($access || auth_no_access(_GT_to_access_this_survey)) {
				if(!$err) {
					mysql_query($sql);
				} else {
					mkwarn(_GT_Can_not_set_survey_status);
					mkerror(_GT_Status .': '. $old_status);
				}
			}
		}
	}
?>
<h2><?php echo(_GT_Survey_Status); ?></h2>
<div align="left">

<ul>
<li><?php echo(_GT_Test_transitions); ?></li>
<li><?php echo(_GT_Activate_transitions); ?></li>
<li><?php echo(_GT_End_transitions); ?></li>
<li><?php echo(_GT_Archive_removes); ?></li>
</ul>
</div>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>&nbsp;/&nbsp;\n"); ?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=purge\">" . _GT_Purge_Surveys . "</a>\n"); ?>
<table border="0" align="center" cellspacing="0" cellpadding="4" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php echo(_GT_ID); ?></th>
		<th align="left"><?php echo(_GT_Name); ?></th>
		<th align="left"><?php echo(_GT_Title); ?></th>
		<th align="left"><?php echo(_GT_Owner); ?></th>
		<th align="left"><?php echo(_GT_Group); ?></th>
		<th align="left"><?php echo(_GT_Status); ?></th>
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
			FROM ".TABLE_SURVEY." WHERE (owner ='".
			$xoopsUser->uid()."' || realm $realms) ORDER BY id DESC";
	}
	$result = mysql_query($sql);

	$realms = array_intersect(
			$_SESSION['acl']['pstatus'],
			array_merge(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pstatus']));

	while(list($sid,$name,$title,$status,$owner,$realm) = mysql_fetch_row($result)) {
		$stat = _GT_Editing;
		$test = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=t&sid=${sid}\">". _GT_Test ."</a>";
		$act  = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=a&sid=${sid}\">". _GT_Activate ."</a>";
		$done = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=e&sid=${sid}\">". _GT_End ."</a>";
		$del  = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=d&sid=${sid}\">". _GT_Archive ."</a>";

		if($status & STATUS_DELETED) {
			$stat = _GT_Archived;
			$test = $act = $done = $del = '&nbsp;';
			continue;
		} elseif($status & STATUS_DONE) {
			$stat = _GT_Ended;
			$test = $act = $done = '&nbsp;';
		} elseif($status & STATUS_ACTIVE) {
			$stat = _GT_Active;
			$test = $act = '&nbsp;';
		} elseif($status & STATUS_TEST) {
			$stat = _GT_Testing;
			$done = '&nbsp;';
			$test = "<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=status&op=m&sid=${sid}\">". _GT_Edit ."</a>";
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
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
