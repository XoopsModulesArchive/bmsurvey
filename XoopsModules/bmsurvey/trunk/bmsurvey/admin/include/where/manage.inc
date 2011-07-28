
<?php

# $Id: manage.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	$base =& $GLOBALS['ESPCONFIG']['ME'];

?>
<h2><?php mb_echo('Management Interface'); ?></h2>
<p><?php
	if($GLOBALS['ESPCONFIG']['auth_design']) {
		if($xoopsUser->isAdmin())
			echo('<font color="yellow">'. mb_('Superuser') .'</font>');
		else
			mb_echo('Username');
		echo(': <tt>'. $xoopsUser->uid() .'</tt>');
	}
?></p>
<table border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="<?php echo($GLOBALS['ESPCONFIG']['active_bgcolor']); ?>" width="95%">
	<tr>
		<td>
			<!-- <font size="+1"><?php mb_echo('Choose a function'); ?>:</font> -->
		</td>
	</tr>
	<tr>
		<td>
			<table border="0" cellspacing="0" cellpadding="0" align="center"><tr><td>
			<ul>
				<li><a href="<?php echo("${base}?where=new");     ?>"><?php mb_echo('Create a New Survey'); ?></a></li>
				<li><a href="<?php echo("${base}?where=edit");    ?>"><?php mb_echo('Edit an Existing Survey'); ?></a></li>
				<li><a href="<?php echo("${base}?where=test");    ?>"><?php mb_echo('Test a Survey'); ?></a></li>
				<li><a href="<?php echo("${base}?where=copy");    ?>"><?php mb_echo('Copy an Existing Survey'); ?></a></li>
				<li><a href="<?php echo("${base}?where=status");  ?>"><?php mb_echo('Change the Status of a Survey'); ?></a>
					<?php mb_echo('(active/end/delete)'); ?></li>
<?php if($GLOBALS['ESPCONFIG']['auth_response']) { ?>
				<li><a href="<?php echo("${base}?where=access");  ?>"><?php mb_echo('Change Access To a Survey'); ?></a> (<?php mb_echo('Limit Respondents.'); ?>)</li>
<?php } ?>
				<li><a href="<?php echo("${base}?where=results"); ?>"><?php mb_echo('View Results from a Survey'); ?></a></li>
				<li><a href="<?php echo("${base}?where=results&type=cross"); ?>"><?php mb_echo('Cross Tabulate Survey Results'); ?></a></li>
				<li><a href="<?php echo("${base}?where=report");  ?>"><?php mb_echo('View a Survey Report'); ?></a></li>
				<li><a href="<?php echo("${base}?where=export");  ?>"><?php mb_echo('Export Data to CSV'); ?></a></li>
			</ul>

<?php if($GLOBALS['ESPCONFIG']['auth_design']) { ?>
			<ul>
<?php
	if($GLOBALS['ESPCONFIG']['auth_type'] != 'ldap') { ?>
				<li><a href="<?php echo("${base}?where=passwd");   ?>"><?php mb_echo('Change Your Password'); ?></a></li>
<?php
	}
	if($xoopsUser->isAdmin() ||
		count($_SESSION['acl']['puser']) > 0 ||
		count($_SESSION['acl']['pgroup']) > 0) {
?>
				<li><a href="<?php echo("${base}?where=designers");   ?>"><?php mb_echo('Manage Designer Accounts'); ?></a></li>
				<li><a href="<?php echo("${base}?where=respondents"); ?>"><?php mb_echo('Manage Respondent Accounts'); ?></a></li>
<?php	if($xoopsUser->isAdmin()) { ?>
				<li><a href="<?php echo("${base}?where=groups"); ?>"><?php mb_echo('Manage Groups'); ?></a></li>
<?php	}
	} ?>
			</ul>
<?php } /* AUTH_DESIGN */ ?>

			<ul>
<!--				<li><a href="<?php echo("${base}?where=TODO");    ?>"><?php mb_echo('View the list of things still to do'); ?></a>
					<?php mb_echo('(development goals)'); ?></li> -->
				<li><a href="<?php echo("${base}?where=guide");  ?>"><?php mb_echo('View the User &amp; Administrator Guide'); ?></a></li>
			</ul>

<?php if($GLOBALS['ESPCONFIG']['auth_design']) { ?>
			<ul>
				<li><a href="<?php echo("${base}?where=logout");  ?>"><?php mb_echo('Log out'); ?></a></li>
			</ul>
<?php } /* AUTH_DESIGN */ ?>
			</td></tr></table>
		</td>
	</tr>
</table>
&nbsp;
