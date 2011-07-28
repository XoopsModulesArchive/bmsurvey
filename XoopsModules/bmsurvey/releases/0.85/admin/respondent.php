<?php
// $Id: respondent.php,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $
//  ------------------------------------------------------------------------ //
//                Bluemoon.Multi-Survey                                      //
//                    Copyright (c) 2005 Yoshi.Sakai @ Bluemoon inc.         //
//                       <http://www.bluemooninc.biz/>                       //
// ------------------------------------------------------------------------- //
//  This program is free software; you can redistribute it and/or modify     //
//  it under the terms of the GNU General Public License as published by     //
//  the Free Software Foundation; either version 2 of the License, or        //
//  (at your option) any later version.                                      //
//                                                                           //
//  You may not change or alter any portion of this comment or credits       //
//  of supporting developers from this source code or any supporting         //
//  source code which is considered copyrighted (c) material of the          //
//  original comment or credit authors.                                      //
//                                                                           //
//  This program is distributed in the hope that it will be useful,          //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU General Public License for more details.                             //
//                                                                           //
//  You should have received a copy of the GNU General Public License        //
//  along with this program; if not, write to the Free Software              //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA //
//  ------------------------------------------------------------------------ //
include '../../../include/cp_header.php';
include_once('../class/bmsurveyUtils.php');

$debug = 0;
if ($debug){ echo "<p>_POST : "; print_r($_POST); }

if(
	(!defined('XOOPS_ROOT_PATH')) || 
	(!is_object($xoopsUser)) || 
	(!$xoopsUser->isAdmin()) ){
	exit();
}
if(isset($_POST['delete_ok'])) {
	$username = isset($_POST['username']) ? ($_POST['username']) : "";
	if($username){
		bmsurveyUtils::delete_respondent($username);
	}
}
if(isset($_POST['update']) || isset($_POST['new']) || isset($_POST['delete'])) {
	$respondent['username'] = isset($_POST['username']) ? ($_POST['username']) : "";
	$respondent['password'] = isset($_POST['password']) ? ($_POST['password']) : "";
	$respondent['email'] = isset($_POST['email']) ? ($_POST['email']) : "";
	$respondent['fname'] = isset($_POST['fname']) ? ($_POST['fname']) : "";
	$respondent['lname'] = isset($_POST['lname']) ? ($_POST['lname']) : "";
	$respondent['disabled'] = isset($_POST['disabled']) ? intval($_POST['disabled']) : "";
	$respondent['sid'] = isset($_POST['sid']) ? intval($_POST['sid']) : 0;
	$respondent['rid'] = isset($_POST['rid']) ? intval($_POST['rid']) : 0;
	$respondent['changed'] = isset($_POST['changed']) ? ($_POST['changed']) : "";
	$respondent['expiration'] = isset($_POST['expiration']) ? ($_POST['expiration']) : "";
	if ($debug){ echo "<p>respondent : "; print_r($respondent);}
	if($respondent['username']){
		bmsurveyUtils::update_respondent($respondent);
	}
}
xoops_cp_header();
include 'adminmenu.php';
echo "<div style='float: left; width:100%;'>";

if ( isset($_POST['delete']) ) {
	echo "<form method='post' action='respondent.php'>";
	echo "<h4>Delete username: ". $respondent['username']."</h4>";
	echo "<input type='hidden' name='username' value='".$respondent['username']."' />";
	echo "<input type=submit name='Cancel' value=Cancel />&nbsp<input type=submit name='delete_ok' value=OK />";
	xoops_cp_footer();
	exit();
} 
	/*
	**	Respondent Select Section
	*/
?>
	<table width='100%' border='0' cellspacing='1' class='outer'>
	<tr><td class="head" colspan="8"><b><?php echo _AM_BMSURVEY_RESPONDENTS; ?></b></td></tr>
	<tr><td rowspan='12'valign="top" class='even' align='center' width='20%'>
		<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
		<select name="username" size="20" style="width: 200px;">
		<?php
		$sql = "SELECT * FROM ".$xoopsDB->prefix("bmsurvey_respondent")." ORDER BY username,email ASC";
		if (!$result = $xoopsDB->query($sql)){
			echo"</td></tr></table></div>";
			xoops_cp_footer();
			exit();
		}
		while ($row = $xoopsDB->fetchArray($result)){
			?>
			<option value="<?php echo $row['username'] ?>"> <?php echo $row['username']." - ".$row['email'] ?> </option>
			<?php
		}
		?>
		</select>
		<p>
		<input type="submit" name="submit" value="<?php echo _AM_BMSURVEY_EDIT;?>" />
		</form>
	</td>
<?php
	/*
	**	Respondent Edit Section
	*/
	$targetUnm = isset($_POST['username']) ? $_POST['username'] : '';
	$opt['Y'] = $opt['N'] = "";
	if ($targetUnm){
		$respondent = bmsurveyUtils::get_Respondentinfo($targetUnm);
		$opt[$respondent['disabled']] = ' CHECKED';
	}
	$username = isset($respondent['username']) ? $respondent['username'] : "";
	$password = isset($respondent['password']) ? $respondent['password'] : "";
	$fname = isset($respondent['fname']) ? $respondent['fname'] : "";
	$lname = isset($respondent['lname']) ? $respondent['lname'] : "";
	$email = isset($respondent['email']) ? $respondent['email'] : "";
	$sid = isset($respondent['sid']) ? $respondent['sid'] : "";
	$rid = isset($respondent['rid']) ? $respondent['rid'] : "";
	$changed = isset($respondent['changed']) ? $respondent['changed'] : "";
	$expiration = isset($respondent['expiration']) ? $respondent['expiration'] : "";
	echo "<center>"._AM_BMSURVEY_RESPONDENT_USAGE."</center>";
	echo "<form method='post' action='respondent.php'>";
	echo "<tr><td class='odd' align='right'>"._AM_BMSURVEY_USERNAME."</td><td class='even'>";
		if(isset($_POST['new'])) {
			echo "<input type='text' name='username' value='".$username."' />";
		}else{
			echo $username."<input type='hidden' name='username' value='".$username."' />";
		}
	echo "</td></tr><tr>";
	echo "<td class='odd' align='right'>*"._AM_BMSURVEY_PASSWORD."</td><td class='even'>";
	echo "<input type='text' name='password' value='".$password ."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_FNAME."</td><td class='even'>";
	echo "<input type='text' name='fname' value='".$fname."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_LNAME."</td><td class='even'>";
	echo "<input type='text' name='lname' value='".$lname."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_EMAIL."</td><td class='even'>";
	echo "<input type='text' name='email' value='".htmlspecialchars($email)."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_DISABLED."</td><td class='even'>";
	echo "<input type=radio name='disabled' value=1 ".$opt['Y'].">YES</input>";
	echo "<input type=radio name='disabled' value=2 ".$opt['N'].">NO</input>";
	echo "</td></tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_SURVEYID."</td><td class='even'>";
	echo "<input type='number' name='sid' value='".$sid."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>*"._AM_BMSURVEY_RESPONSEID."</td><td class='even'>";
	echo "<input type='number' name='rid' value='".$rid."' />";
	if ( $sid && $rid ){
		$url = sprintf(XOOPS_URL."/modules/"
			.$xoopsModule->dirname()
			."/admin/manage.php?where=results&sid=%u&rid=%u",$sid,$rid);
		echo "&nbsp;<a href='".$url."'>"._AM_BMSURVEY_SEEARESULT."</a>";
	}
	echo "</td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>*"._AM_BMSURVEY_CHANGED."</td><td class='even'>";
	echo "<input type='date' name='changed' value='".$changed."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_EXPIRE."</td><td class='even'>";
	echo "<input type='date' name='expiration' value='".$expiration."' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='center' colspan=3>";
	echo "<input type=submit name='update' value='update' />&nbsp;&nbsp;";
	echo "<input type=submit name='new' value='new' />&nbsp;&nbsp;";
	echo "<input type=submit name='delete' value='delete' />";
	echo "</td></tr></form></tr></table>";
	xoops_cp_footer();
?>
