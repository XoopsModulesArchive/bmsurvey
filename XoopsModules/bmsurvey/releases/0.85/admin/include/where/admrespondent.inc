
<?php

# $Id: admrespondent.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>
// <jflemer@acm.rpi.edu>

	$bg1 =& $ESPCONFIG['bgalt_color1'];
	$bg2 =& $ESPCONFIG['bgalt_color2'];

	$errstr = '';
	$u = '';
	$r = '';

	/* abort */
	if(isset($_POST['cancel'])) {
		unset($_POST['submit']);
		unset($_POST['delete']);
		include(esp_where('respondents'));
		return;
	}

	/* delete user */
	if(isset($_POST['delete'])) {
		unset($_POST['submit']);
		$u = _addslashes($_POST['u']);
		$r = _addslashes($_POST['r']);
		/* check acl */
		if($xoopsUser->isAdmin() ||
			in_array($r, $_SESSION['acl']['pgroup']) ||
			auth_no_access(_('to access this group'))) {
			$sql = "DELETE FROM ".TABLE_RESPONDENT." WHERE username='$u' AND realm='$r'";
			if(!mysql_query($sql) || mysql_affected_rows() < 1) {
				/* unsucessfull -- abort */
				$errstr .= mkerror(_('Cannot delete account.') .' ('.
					mysql_error() .')');
			}
		}
		if(empty($errstr)) {
			include(esp_where('respondents'));
			return;
		}
	}

	/* submitted */
	if(isset($_POST['submit'])) {
		$u         = _addslashes($_POST['u']);
		$r         = _addslashes($_POST['r']);
		$password  = _addslashes($_POST['password']);
		$fname     = _addslashes($_POST['fname']);
		$lname     = _addslashes($_POST['lname']);
		$email     = _addslashes($_POST['email']);
		$disabled  = _addslashes($_POST['disabled']);

		if(!empty($_POST['ex_year']) ||
				!empty($_POST['ex_month']) ||
				!empty($_POST['ex_day'])) {
			if(empty($_POST['ex_day'])) {
				$ex_day = 1;
			} else {
				$ex_day = intval($_POST['ex_day']);
			}
			if(empty($_POST['ex_month'])) {
				$ex_month = 1;
			} else {
				$ex_month = intval($_POST['ex_month']);
			}
			if(empty($_POST['ex_year'])) {
				$now = getdate(time());
				$ex_year = $now['year'];
			} else {
				$ex_year = intval($_POST['ex_year']);
				if($ex_year < 2000)
					$ex_year += 2000;
			}

			$expiration = sprintf("%04d%02d%02d%06d",
				$ex_year,$ex_month,$ex_day,0);
		} else {
			$expiration = "0";
			$ex_year    = '';
			$ex_month   = '';
			$ex_day     = '';
		}

		if($r == 'superuser' && $_SESSION['acl']['superuser'] != 'Y')
			$r = '';

		/* new user */
		$sql = "SELECT * FROM ".TABLE_RESPONDENT." WHERE username='$u' AND realm='$r'";
		$result = mysql_query($sql);
		if(mysql_num_rows($result) < 1) {
			if(empty($u) || empty($r) || empty($password)) {
				$errstr .= mkerror(_('Username, Password, and Group are required.'));
			} else {
				$sql = "INSERT INTO ".TABLE_RESPONDENT.
					" (username,realm,password) VALUES ('$u','$r',PASSWORD('$password'))";
				if(!mysql_query($sql)) {
					$u = '';
					$r = '';
					$errstr .= mkerror(_('Error adding account.') .' ('.
						mysql_error() .')');
				}
			}
		}
		mysql_free_result($result);

		/* change user data */
		if(!empty($u) && !empty($r)) {
			if(empty($disabled))
				$disabled = 'N';
			if (!empty($password))
				$password = "password=PASSWORD('$password'),";
			$sql = "UPDATE ".TABLE_RESPONDENT." SET
				$password
				fname='$fname',
				lname='$lname',
				email='$email',
				disabled='$disabled',
				changed=now(),
				expiration='$expiration'
			WHERE username='$u' AND realm='$r'";
			if(!mysql_query($sql)) {
				/* unsucessfull -- abort */
				$errstr .= mkerror(_('Cannot change account data.') .' ('.
					mysql_error() .')');
			}
		}

		if(empty($errstr)) {
			include(esp_where('respondents'));
			return;
		}
	} else if(isset($_GET['u']) && isset($_GET['r'])) {
		$u = _addslashes($_GET['u']);
		$r = _addslashes($_GET['r']);
	} else {
		$u = '';
		$r = '';
		$expiration = '';
		$disabled = 'N';
	}

	/* load ACL */
	if(!empty($u) && !empty($r)) {
		$sql = "SELECT * FROM ".TABLE_RESPONDENT." WHERE username='$u' AND realm='$r'";
		$result = mysql_query($sql);
		if($arr = mysql_fetch_array($result, MYSQL_ASSOC)) {
			foreach(array(
				'username', 'fname', 'lname', 'email',
				'realm', 'disabled', 'expiration') as $col)
				{ $$col = $arr[$col]; }
			$u =& $username;
			$r =& $realm;
			if(intval($expiration) > 0) {
				$ex_year  = substr($expiration,0,4);
				$ex_month = substr($expiration,4,2);
				$ex_day   = substr($expiration,6,2);
			} else {
				$ex_year  = '';
				$ex_month = '';
				$ex_day   = '';
			}
		} else {
			$errstr .= mkerror(_('Account not found.') .' ('.
				mysql_error() .')');
		}
	}

?>
<h2><?php mb_echo('Respondent Account Administration'); ?></h2>
<?php if(!empty($errstr)) echo("<p>$errstr</p>\n"); ?>
<input type="hidden" name="where" value="admrespondent">
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>">
	<tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Username') ."</th>\n");
	if(empty($u))
		$u = mktext('u', 16, 16, $GLOBALS);
	else
		$u = mkhidden('u', $GLOBALS) . "<tt>$u</tt>";
	echo("<td bgcolor=\"$bg1\">$u</td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Password') ."</th>\n");
	echo("<td bgcolor=\"$bg1\">". mkpass('password') ."</td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Group') ."</th>\n");
	if(empty($r)) {
		$r = '<select name="r"><option></option>';
		$groups = array();
		if($xoopsUser->isAdmin()) {
			$sql = "SELECT name FROM ".TABLE_REALM."";
			$result = mysql_query($sql);
			while( list($g) = mysql_fetch_row($result) ) {
				array_push($groups, $g);
			}
			mysql_free_result($result);
		} else {
			$groups =& $_SESSION['acl']['pgroup'];
		}
		foreach($groups as $g) {
			$r .= "<option value=\"$g\">$g</option>";
		}
		$r .= '</select>';
	} else {
		$r = mkhidden('r', $GLOBALS) . "<tt>$r</tt>";
	}
	echo("<td bgcolor=\"$bg1\"><tt>$r</tt></td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('First Name') ."</th>\n");
	echo("<td bgcolor=\"$bg1\">". mktext('fname', 16, 16, $GLOBALS) ."</td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Last Name') ."</th>\n");
	echo("<td bgcolor=\"$bg1\">". mktext('lname', 24, 24, $GLOBALS) ."</td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Email') ."</th>\n");
	echo("<td bgcolor=\"$bg1\">". mktext('email', 24, 64, $GLOBALS) ."</td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Expiration') ."</th>\n");
	echo("<td bgcolor=\"$bg1\">".
		mktext('ex_year', 4, 0, $GLOBALS) .' '.
		mktext('ex_month', 2, 0, $GLOBALS) .' '.
		mktext('ex_day', 2, 0, $GLOBALS) .' ('.
			_('year') .' '.
			_('month') .' '.
			_('day'). ")</td>\n");
?>
	</tr><tr>
<?php
	echo("<th bgcolor=\"$bg2\" align=\"right\">". mb_('Disabled') ."</th>\n");
	echo("<td bgcolor=\"$bg1\">". mkselect('disabled',array('Y' => mb_('Yes'), 'N' => mb_('No')), $GLOBALS) ."</td>\n");
?>
	</tr><tr>
		<th colspan="2" bgcolor="<?php echo($bg2); ?>">
			<input type="submit" name="submit" value="<?php mb_echo('Update'); ?>">&nbsp;
			<input type="submit" name="cancel" value="<?php mb_echo('Cancel'); ?>">&nbsp;
			<input type="submit" name="delete" value="<?php mb_echo('Delete'); ?>">
		</th>
	</tr>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
