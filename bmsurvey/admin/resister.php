<?php
include '../../../include/cp_header.php';
include_once('../class/bmsurveyUtils.php');
if(
	(!defined('XOOPS_ROOT_PATH')) || 
	(!is_object($xoopsUser)) || 
	(!$xoopsUser->isAdmin()) ){
	exit();
}
if(isset($_POST['send'])) {
	$Respondent['email'] = isset($_POST['email']) ? ($_POST['email']) : "";
	$Respondent['subject'] = isset($_POST['subject']) ? ($_POST['subject']) : "";
	$Respondent['message'] = isset($_POST['message']) ? ($_POST['message']) : "";
	if($Respondent['email']){
		$xoopsMailer =& getMailer();
		$xoopsMailer->useMail();
		$xoopsMailer->setToEmails($Respondent['email']);
		$xoopsMailer->setFromEmail(bmsurveyUtils::getXoopsModuleConfig('MAILADDR'));
		$xoopsMailer->setFromName($xoopsConfig['sitename']);
		$xoopsMailer->setSubject($Respondent['subject']);
		$body = $Respondent['subject'] . "\r\n" . $Respondent['message'];
		$body .= "\n"._DATE." ".formatTimestamp(time(), 'm', $xoopsConfig['default_TZ']);
		$xoopsMailer->setBody($body);
		if ( !$xoopsMailer->send() )
			echo "Send failed to " . $Respondent['email'];
		else
			echo "Sent to " . $Respondent['email'];
	}
}
xoops_cp_header();
include 'adminmenu.php';
	echo "<div style='float: left; width:100%;'>";
	/*
	**	Invitation mail Section
	*/
	echo "<form method='post' action='resister.php'>";
	echo "<p><table width='100%' border='0' cellspacing='1' class='outer'>";
	echo "<tr><td class='head' colspan=2><b>"._AM_BMSURVEY_INVITATION."</b></td></tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_EMAIL."</td><td class='even'>";
	echo "<input type='text' name=email value='' /></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='right'>"._AM_BMSURVEY_SUBJECT."</td><td class='even'>";
	echo "<input type='text' name=subject value='M,NEW;"._AM_BMSURVEY_SUBJECT_NEW."' /></td>";
	echo "</tr><tr>";
	echo "<tr><td class='odd' align='right'>"._AM_BMSURVEY_MESSAGE."</td><td class='even'>";
 	echo "<textarea name='message' rows=6 cols=40>"._AM_BMSURVEY_MESSAGE_NEW."</textarea></td>";
	echo "</tr><tr>";
	echo "<td class='odd' align='center' colspan=2><input type=submit name='send' value='send' /></td>";
	echo "</tr></form></tr></table>";
	xoops_cp_footer();
?>
