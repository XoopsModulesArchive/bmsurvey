
<?php

# $Id: manage.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	$base =& $GLOBALS['ESPCONFIG']['ME'];

?>
<h2><?php echo(_GT_Management_Interface); ?></h2>
<p><?php
	if($GLOBALS['ESPCONFIG']['auth_design']) {
		if($xoopsUser->isAdmin())
			echo('<font color="yellow">'. _GT_Superuser .'</font>');
		else
			echo(_GT_Username);
		echo(': <tt>'. $xoopsUser->uid() .'</tt>');
	}
?></p>
<table border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="<?php echo($GLOBALS['ESPCONFIG']['active_bgcolor']); ?>" width="95%">
	<tr>
		<td>
			<!-- <font size="+1"><?php echo(_GT_Choose_a_function); ?>:</font> -->
		</td>
	</tr>
	<tr>
		<td>
			<table border="0" cellspacing="0" cellpadding="0" align="center"><tr><td>
			<ul>
				<li><a href="<?php echo("${base}?where=new");     ?>"><?php echo(_GT_Create_a_New_Survey); ?></a></li>
				<li><a href="<?php echo("${base}?where=edit");    ?>"><?php echo(_GT_Edit_an_Existing_Survey); ?></a></li>
				<li><a href="<?php echo("${base}?where=test");    ?>"><?php echo(_GT_Test_a_Survey); ?></a></li>
				<li><a href="<?php echo("${base}?where=copy");    ?>"><?php echo(_GT_Copy_an_Existing_Survey); ?></a></li>
				<li><a href="<?php echo("${base}?where=status");  ?>"><?php echo(_GT_Change_the_Status_of_a_Survey); ?></a>
					<?php echo(_GT_active_end_delete); ?></li>
<?php if($GLOBALS['ESPCONFIG']['auth_response']) { ?>
				<li><a href="<?php echo("${base}?where=access");  ?>"><?php echo(_GT_Change_Access_To_a_Survey); ?></a> (<?php echo(_GT_Limit_Respondents); ?>)</li>
<?php } ?>
				<li><a href="<?php echo("${base}?where=results"); ?>"><?php echo(_GT_View_Results_from_a_Survey); ?></a></li>
				<li><a href="<?php echo("${base}?where=results&type=cross"); ?>"><?php echo(_GT_Cross_Tabulate_Survey_Results); ?></a></li>
				<li><a href="<?php echo("${base}?where=report");  ?>"><?php echo(_GT_View_a_Survey_Report); ?></a></li>
				<li><a href="<?php echo("${base}?where=export");  ?>"><?php echo(_GT_Export_Data_to_CSV); ?></a></li>
			</ul>

<?php if($GLOBALS['ESPCONFIG']['auth_design']) { ?>
			<ul>
<?php
	if($GLOBALS['ESPCONFIG']['auth_type'] != 'ldap') { ?>
				<li><a href="<?php echo("${base}?where=passwd");   ?>"><?php echo(_GT_Change_Your_Password); ?></a></li>
<?php
	}
	if($xoopsUser->isAdmin() ||
		count($_SESSION['acl']['puser']) > 0 ||
		count($_SESSION['acl']['pgroup']) > 0) {
?>
				<li><a href="<?php echo("${base}?where=designers");   ?>"><?php echo(_GT_Manage_Designer_Accounts); ?></a></li>
				<li><a href="<?php echo("${base}?where=respondents"); ?>"><?php echo(_GT_Manage_Respondent_Accounts); ?></a></li>
<?php	if($xoopsUser->isAdmin()) { ?>
				<li><a href="<?php echo("${base}?where=groups"); ?>"><?php echo(_GT_Manage_Groups); ?></a></li>
<?php	}
	} ?>
			</ul>
<?php } /* AUTH_DESIGN */ ?>

			<ul>
<!--				<li><a href="<?php echo("${base}?where=TODO");    ?>"><?php echo(_GT_View_the_list_of_things_still_to_do); ?></a>
					<?php echo(_GT_development_goals); ?></li> -->
				<li><a href="<?php echo("${base}?where=guide");  ?>"><?php echo(_GT_View_the_User_Administrator_Guide); ?></a></li>
			</ul>

<?php if($GLOBALS['ESPCONFIG']['auth_design']) { ?>
			<ul>
				<li><a href="<?php echo("${base}?where=logout");  ?>"><?php echo(_GT_Log_out); ?></a></li>
			</ul>
<?php } /* AUTH_DESIGN */ ?>
			</td></tr></table>
		</td>
	</tr>
</table>
&nbsp;
