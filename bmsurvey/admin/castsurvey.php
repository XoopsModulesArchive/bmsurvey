<?php
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

if(
	(!defined('XOOPS_ROOT_PATH')) || 
	(!is_object($xoopsUser)) || 
	(!$xoopsUser->isAdmin()) ){
	exit();
}
$sid = isset($_POST['sid']) ? intval($_POST['sid']) : 0;

if ( $_POST['selectuser'] && $_POST['username'] ){
	$url = XOOPS_URL."/modules/".$xoopsModule->dirname()."/cast.php?key="
		.bmsurveyUtils::getXoopsModuleConfig('CAST_KEY')."&sid=".$sid."&admin=1&username=".htmlspecialchars( $_POST['username'], ENT_QUOTES );
	redirect_header($url,1,_AM_BMSURVEY_SENDQUESTION);

}

xoops_cp_header();
include 'adminmenu.php';
	echo "<div style='float: left; width:100%;'>";
	/*
	**	Question Section
	*/
	echo "<p>";
	echo "<form method='post' action='castsurvey.php'>";
	echo "<p><table width='100%' border='0' cellspacing='1' class='outer'>";
	echo "<tr><td class='head' colspan=3><b>"._AM_BMSURVEY_SENDQUESTION."</b></td></tr>";
	echo "<td class='odd' align='left'>"._AM_BMSURVEY_CHOSESURVEY."</td><td class='odd' ></td>";
	echo "<td class='odd' align='left'>"._AM_BMSURVEY_RESPONDENTS." "._AM_BMSURVEY_SURVEYID.":".$sid."</td></tr>";
	echo "<tr><td class='even' valign='top'>";

	echo "\n<select name='sid' size=20 style='width: 200px;'>";
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE status = 1 ORDER BY id";
	if (!$result = $xoopsDB->query($sql)){
		echo"</td></tr></table></div>";
		xoops_cp_footer();
		exit();
	}
	while ($row = $xoopsDB->fetchArray($result)){
		echo "<option value=".$row['id'];
		if ($row['id']==$sid) echo " selected='selected'";
		echo ">".$row['id'].": ".$row['title'] . "</option>\n";
	}
	echo "</td><td class='odd' align='center'></select><p><input type=submit name='confirm' value='"._AM_BMSURVEY_CONFIRM."' /></td>";

	echo "<td class='even' align='left' valign='top'>";
	$sql = "SELECT * FROM ".TABLE_RESPONDENT." WHERE survey_id=".$sid." ORDER BY username,email ASC";
	echo "<select name='username' size='20' style='width: 200px;'>";
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
	<?php
	echo "<input type=submit name='selectuser' value='"._AM_BMSURVEY_SENDQUESTION."' />";
	echo "</td></tr>";
	echo "<tr><td class='odd' colspan=3 align='center'>";
	if (!$sid) $sid="ALL";
	$url = XOOPS_URL."/modules/".$xoopsModule->dirname()."/cast.php?key=".bmsurveyUtils::getXoopsModuleConfig('CAST_KEY')."&sid=".$sid;
	echo "<a href='".$url."&admin=1'>"._AM_BMSURVEY_SENDQUESTIONNOW."</a>";
	echo "<p>";
	echo sprintf(_AM_BMSURVEY_SENDQUESTIONUSAGE,$url);
	echo "</tr></form></tr></table>";
	xoops_cp_footer();
?>
